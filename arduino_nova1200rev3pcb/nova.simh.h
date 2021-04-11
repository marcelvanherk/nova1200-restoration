/* Nova simulator; derived from Simh - 
   reduced to Nova 800/1200 instructions only 
   
   Original from simhv39-0
   Copyright (c) 1993-2008, Robert M. Supnik
   Adapted to fit in Teensy by Marcel van Herk

   Prior to inclusion the following must be defined:
   MEMSIZE 16384
   M NovaMem
   AC NovaRegs
   PC NovaPC
   C NovaC
*/

/* Architectural constants */

#define SIGN            0100000                         /* sign */
#define DMASK           0177777                         /* data mask */
#define CBIT            (DMASK + 1)                     /* carry bit */
#define CDMASK          (CBIT | DMASK)                  /* carry + data */
#define A_IND           0100000

/* Reserved memory locations */

#define INT_SAV         0                               /* intr saved PC */
#define INT_JMP         1                               /* intr jmp @ */
#define STK_JMP         3                               /* stack jmp @ */
#define TRP_SAV         046                             /* trap saved PC */
#define TRP_JMP         047                             /* trap jmp @ */

#define AUTO_TOP        037                             /* top of autoindex  */
#define AUTO_DEC        030                             /* start autodec */
#define AUTO_INC        020                             /* start autoinc */


/* Instruction format */

#define I_OPR           0100000                         /* operate */
#define I_M_SRC         03                              /* OPR: src AC */
#define I_V_SRC         13
#define I_GETSRC(x)     (((x) >> I_V_SRC) & I_M_SRC)
#define I_M_DST         03                              /* dst AC */
#define I_V_DST         11
#define I_GETDST(x)     (((x) >> I_V_DST) & I_M_DST)
#define I_M_ALU         07                              /* OPR: ALU op */
#define I_V_ALU         8
#define I_GETALU(x)     (((x) >> I_V_ALU) & I_M_ALU)
#define I_M_SHF         03                              /* OPR: shift */
#define I_V_SHF         6
#define I_GETSHF(x)     (((x) >> I_V_SHF) & I_M_SHF)
#define I_M_CRY         03                              /* OPR: carry */
#define I_V_CRY         4
#define I_GETCRY(x)     (((x) >> I_V_CRY) & I_M_CRY)
#define I_V_NLD         3                               /* OPR: no load */
#define I_NLD           (1 << I_V_NLD)
#define I_M_SKP         07                              /* OPR: skip */
#define I_V_SKP         0
#define I_GETSKP(x)     (((x) >> I_V_SKP) & I_M_SKP)

#define I_M_OPAC        017                             /* MRF: opcode + AC */
#define I_V_OPAC        11
#define I_GETOPAC(x)    (((x) >> I_V_OPAC) & I_M_OPAC)
#define I_V_IND         10                              /* MRF: indirect */
#define I_IND           (1 << I_V_IND)
#define I_M_MODE        03                              /* MRF: mode */
#define I_V_MODE        8
#define I_GETMODE(x)    (((x) >> I_V_MODE) & I_M_MODE)
#define I_M_DISP        0377                            /* MRF: disp */
#define I_V_DISP        0
#define I_GETDISP(x)    (((x) >> I_V_DISP) & I_M_DISP)
#define DISPSIZE        (I_M_DISP + 1)                  /* page size */
#define DISPSIGN        (DISPSIZE >> 1)                 /* page sign */

#define I_M_IOT         07                              /* IOT: code */
#define I_V_IOT         8
#define I_GETIOT(x)     (((x) >> I_V_IOT) & I_M_IOT)
#define I_M_PULSE       03                              /* IOT pulse */
#define I_V_PULSE       6
#define I_GETPULSE(x)   (((x) >> I_V_PULSE) & I_M_PULSE)
#define I_M_DEV         077                             /* IOT: device */
#define I_V_DEV         0
#define I_GETDEV(x)     (((x) >> I_V_DEV) & I_M_DEV)

#define I_M_XOP         037                             /* XOP: code */
#define I_V_XOP         6
#define I_GETXOP(x)     (((x) >> I_V_XOP) & I_M_XOP)

/* IOT return codes */

#define IOT_V_REASON    16                              /* set reason */
#define IORETURN(f,v)   ((f)? (v): SCPE_OK)             /* stop on error */

/* IOT fields */

#define ioNIO           0                               /* opcode field */
#define ioDIA           1
#define ioDOA           2
#define ioDIB           3
#define ioDOB           4
#define ioDIC           5
#define ioDOC           6
#define ioSKP           7

#define iopN            0                               /* pulse field */
#define iopS            1
#define iopC            2
#define iopP            3

/* Device numbers */

#define DEV_LOW         010                             /* lowest intr dev */
#define DEV_HIGH        051                             /* highest intr dev */
#define DEV_MDV         001                             /* multiply/divide */
#define DEV_ECC         002                             /* ECC memory control */
#define DEV_MAP         003                             /* MMPU control */
#define DEV_TTI         010                             /* console input */
#define DEV_TTO         011                             /* console output */
#define DEV_PTR         012                             /* paper tape reader */
#define DEV_PTP         013                             /* paper tape punch */
#define DEV_CLK         014                             /* clock */
#define DEV_PLT         015                             /* plotter */
#define DEV_CDR         016                             /* card reader */
#define DEV_LPT         017                             /* line printer */
#define DEV_DSK         020                             /* fixed head disk */
#define DEV_MTA         022                             /* magtape */
#define DEV_DCM         024                             /* data comm mux */
#define DEV_ADCV        030                             /* A/D converter */
#define DEV_QTY         030                             /* 4060 multiplexor */
#define DEV_DKP         033                             /* disk pack */
#define DEV_CAS         034                             /* cassette */
#define DEV_ALM         034                             /* ALM/ULM multiplexor */
#define DEV_PIT         043                             /* programmable interval timer */
#define DEV_TTI1        050                             /* second console input */
#define DEV_TTO1        051                             /* second console output */
#define DEV_CPU         077                             /* CPU control */

// data types for Teensy

#define int32 int
#define uint32 unsigned int
#define t_stat int

/* I/O structure

   The NOVA I/O structure is tied together by dev_table, indexed by
   the device number.  Each entry in dev_table consists of

        mask            device mask for busy, done (simulator representation)
        pi              pi disable bit (hardware representation)
        routine         IOT action routine

   dev_table is populated at run time from the device information
   blocks in each device.
*/

struct ndev {
    int32       mask;                                   /* done/busy mask */
    int32       pi;                                     /* assigned pi bit */
    int32       (*routine)(int32, int32, int32);        /* dispatch routine */
    };

    
/* Device flags (simulator representation)

   Priority (for INTA) runs from low numbers to high
*/

#define INT_V_PIT       2                               /* PIT */
#define INT_V_DKP       3                               /* moving head disk */
#define INT_V_DSK       4                               /* fixed head disk */
#define INT_V_MTA       5                               /* magnetic tape */
#define INT_V_LPT       6                               /* line printer */
#define INT_V_CLK       7                               /* clock */
#define INT_V_PTR       8                               /* paper tape reader */
#define INT_V_PTP       9                               /* paper tape punch */
#define INT_V_PLT       10                              /* plotter */
#define INT_V_TTI       11                              /* keyboard */
#define INT_V_TTO       12                              /* terminal */
#define INT_V_TTI1      13                              /* second keyboard */
#define INT_V_TTO1      14                              /* second terminal */
#define INT_V_QTY       15                              /* QTY multiplexor */
#define INT_V_ALM       16                              /* ALM multiplexor */
#define INT_V_STK       17                              /* stack overflow */
#define INT_V_NO_ION_PENDING 18                         /* ion delay */
#define INT_V_ION       19                              /* interrupts on */
#define INT_V_TRAP      20                              /* trap instruction */

#define INT_PIT         (1 << INT_V_PIT)
#define INT_DKP         (1 << INT_V_DKP)
#define INT_DSK         (1 << INT_V_DSK)
#define INT_MTA         (1 << INT_V_MTA)
#define INT_LPT         (1 << INT_V_LPT)
#define INT_CLK         (1 << INT_V_CLK)
#define INT_PTR         (1 << INT_V_PTR)
#define INT_PTP         (1 << INT_V_PTP)
#define INT_PLT         (1 << INT_V_PLT)
#define INT_TTI         (1 << INT_V_TTI)
#define INT_TTO         (1 << INT_V_TTO)
#define INT_TTI1        (1 << INT_V_TTI1)
#define INT_TTO1        (1 << INT_V_TTO1)
#define INT_QTY         (1 << INT_V_QTY)
#define INT_ALM         (1 << INT_V_ALM)
#define INT_STK         (1 << INT_V_STK)
#define INT_NO_ION_PENDING (1 << INT_V_NO_ION_PENDING)
#define INT_ION         (1 << INT_V_ION)
#define INT_DEV         ((1 << INT_V_STK) - 1)          /* device ints */
#define INT_PENDING     INT_ION+INT_NO_ION_PENDING
#define	INT_TRAP	(1 << INT_V_TRAP)

/* PI disable bits */

#define PI_PIT          0001000
#define PI_DKP          0000400
#define PI_DSK          0000100
#define PI_MTA          0000040
#define PI_LPT          0000010
#define PI_CLK          0000004
#define PI_PTR          0000020
#define PI_PTP          0000004
#define PI_PLT          0000010
#define PI_QTY          0000002
#define PI_ALM          0000002
#define PI_TTI          0000002
#define PI_TTO          0000001
#define PI_TTI1         PI_TTI
#define PI_TTO1         PI_TTO
/* #define PI_CDR       0000040 */
/* #define PI_DCM       0100000 */
/* #define PI_CAS       0000040 */
/* #define PI_ADCV      0000002 */


        /*  Macros to clear/set BUSY/DONE/INTR bits  */

#define DEV_SET_BUSY( x )    dev_busy = dev_busy |   (x)
#define DEV_CLR_BUSY( x )    dev_busy = dev_busy & (~(x))
#define DEV_SET_DONE( x )    dev_done = dev_done |   (x)
#define DEV_CLR_DONE( x )    dev_done = dev_done & (~(x))
#define DEV_UPDATE_INTR      int_req = (int_req & ~INT_DEV) | (dev_done & ~dev_disable)

#define DEV_IS_BUSY( x )    (dev_busy & (x))
#define DEV_IS_DONE( x )    (dev_done & (x))

#define IND_STEP(x)     M[x] & A_IND;  /* return next level indicator */ \
                        if ( ((x) <= AUTO_TOP) && ((x) >= AUTO_INC) ) {   \
                            if ( (x) < AUTO_DEC ) {   \
                                M[x] = (M[x] + 1) & DMASK;  }  \
                            else {   \
                                M[x] = (M[x] - 1) & DMASK;  }}  \
                        x = M[x] & AMASK

#define PCQ_SIZE        64                              /* must be 2**n */
#define PCQ_MASK        (PCQ_SIZE - 1)
#define PCQ_ENTRY       pcq[pcq_p = (pcq_p - 1) & PCQ_MASK] = PC

int32 pcq[PCQ_SIZE];

#define INCREMENT_PC    PC = (PC + 1) & AMASK           /* increment PC */

struct ndev dev_table[64];

// these come from outside in the Teensy code

#if 0
//short M[MEMSIZE] = {0};
//int32 PC = 0;
//int32 AC[4] = {0};
//int32 C = 0;
#endif

int32 SR = 0;
int32 AMASK = 077777 ;                                  /* current memory address mask  */
int32 dev_done = 0;                                     /* device done flags */
int32 dev_busy = 0;                                     /* device busy flags */
int32 dev_disable = 0;                                  /* int disable flags */
int32 int_req = 0;                                      /* interrupt requests */
int32 ind_max = 65536;                                  /* iadr nest limit */
int32 stop_dev = 0;                                     /* stop on ill dev */
int32 sim_interval = 0;                                     /* stop on ill dev */
int32 saved_PC =0;
int32 pimask =0;
int32 pwr_low =0;

#define SCPE_OK 0
#define SCPE_IERR 1

#define STOP_RSRV       1                               /* must be 1 */
#define STOP_HALT       2                               /* HALT */
#define STOP_IBKPT      3                               /* breakpoint */
#define STOP_IND        4                               /* indirect loop */
#define STOP_IND_INT    5                               /* ind loop, intr or trap */

#define MEM_ADDR_OK(x)  (((uint32) (x)) < (uint32) MEMSIZE)

// Minimal IO system implementation for Teensy

void mask_out (int32 mask)
{
};

int32 tto (int32 pulse, int32 code, int32 AC);
int32 tti (int32 pulse, int32 code, int32 AC);
int32 ttobuf;
int32 ttibuf;

static bool has_devtab=false;
int build_devtab (void)
{ if (!has_devtab)
  { int i;
    for(i=0; i<64; i++)
    { dev_table[i].mask = 0;
      dev_table[i].pi = 0;
      dev_table[i].routine = NULL;
    }
    
    // populate TTI and TTO
    dev_table[DEV_TTO].routine = tto;
    dev_table[DEV_TTI].routine = tti;
    dev_table[DEV_TTO].mask = INT_TTO;
    dev_table[DEV_TTI].mask = INT_TTI;
    has_devtab=true;
  }
  return SCPE_OK;
};

int sim_process_event (void)
{ sim_interval=100;
  return 0;
};

void reset_all(int32 mask)
{ int_req=0;
  ttobuf=0;
  ttibuf=0;
  dev_busy=0;
  dev_done=0;
  has_devtab=false;
};

// Below modified from nova_tt.c
/* Terminal input: IOT routine */

int32 tti (int32 pulse, int32 code, int32 AC)
{
int32 iodata;

if (code == ioDIA)
    iodata = ttibuf & 0377;
else iodata = 0;

switch (pulse)
    {                                                   /* decode IR<8:9> */
  case iopS:                                            /* start */
    DEV_SET_BUSY( INT_TTI ) ;
    DEV_CLR_DONE( INT_TTI ) ;
    DEV_UPDATE_INTR ;
    break;

  case iopC:                                            /* clear */
    DEV_CLR_BUSY( INT_TTI ) ;
    DEV_CLR_DONE( INT_TTI ) ;
    DEV_UPDATE_INTR ;
    break;
    }                                                   /* end switch */

return iodata;
}

/* Terminal output: IOT routine */

int32 tto (int32 pulse, int32 code, int32 AC)
{
if (code == ioDOA)
    ttobuf = AC & 0377; 

switch (pulse)
    {                                                   /* decode IR<8:9> */
  case iopS:                                            /* start */
    DEV_SET_BUSY( INT_TTO ) ;
    DEV_CLR_DONE( INT_TTO ) ;
    DEV_UPDATE_INTR ;
    //sim_activate (&tto_unit, tto_unit.wait);            /* activate unit */
    break;

  case iopC:                                            /* clear */
    DEV_CLR_BUSY( INT_TTO ) ;
    DEV_CLR_DONE( INT_TTO ) ;
    DEV_UPDATE_INTR ;
    //sim_cancel (&tto_unit);                             /* deactivate unit */
    break;
    }                                                   /* end switch */

if (code == ioDOA) {
        int a=ttobuf&0x7f;
        Serialwrite(a);
        DEV_CLR_BUSY(INT_TTO);
        DEV_SET_DONE(INT_TTO);
        DEV_UPDATE_INTR ;
}

return 0;
}

/* Reset routine */

t_stat tto_reset ()
{
ttobuf = 0;                                       /* <not DG compatible!>  */
DEV_CLR_BUSY( INT_TTO ) ;
DEV_CLR_DONE( INT_TTO ) ;
DEV_UPDATE_INTR ;
//sim_cancel (&tto_unit);                                 /* deactivate unit */
return SCPE_OK;
}

// The simulator; added n_instr parameter to 
// allow single stepping in Teensy; 
// pass 0 to run, 1 to single step

t_stat sim_instr (int n_inst)
{
int32 PC, IR, i;
t_stat reason;
int n_instructions = n_inst;

/* Restore register state */

if (build_devtab () != SCPE_OK)                         /* build dispatch */
    return SCPE_IERR;
PC = saved_PC & AMASK;                                  /* load local PC */
C = C & CBIT;
mask_out (pimask);                                      /* reset int system */
reason = 0;

/* Main instruction fetch/decode loop */

while (reason == 0) {
    // process n_inst for Teensy
    if (n_inst) {
      n_instructions--;
      if (n_instructions<0) {
        reason=STOP_IBKPT;
        break;
      }
    }
                                   /* loop until halted */

    if (sim_interval <= 0) {                            /* check clock queue */
        if ( (reason = sim_process_event ()) )
            break;
        }

    if (int_req > INT_PENDING) {                        /* interrupt or exception? */
        int32 MA, indf;

        if (int_req & INT_TRAP) {                       /* trap instruction? */
            int_req = int_req & ~INT_TRAP ;             /* clear */
            //PCQ_ENTRY;                                  /* save old PC */
            M[TRP_SAV] = (PC - 1) & AMASK;
            MA = TRP_JMP;                               /* jmp @47 */
            }
        else {
            int_req = int_req & ~INT_ION;               /* intr off */
            //PCQ_ENTRY;                                  /* save old PC */
            M[INT_SAV] = PC;
            if (int_req & INT_STK) {                    /* stack overflow? */
                int_req = int_req & ~INT_STK;           /* clear */
                MA = STK_JMP;                           /* jmp @3 */
            }
            else {
                MA = INT_JMP;                                   /* intr: jmp @1 */
            }
        }

        {
        for (i = 0, indf = 1; indf && (i < ind_max); i++) {
            indf = IND_STEP (MA);                       /* indirect loop */
            }
        if (i >= ind_max) {
            reason = STOP_IND_INT;
            break;
            }
        }
    PC = MA;
    }                                                   /* end interrupt */

//    if (sim_brk_summ && sim_brk_test (PC, SWMASK ('E'))) { /* breakpoint? */
//        reason = STOP_IBKPT;                            /* stop simulation */
//        break;
//        }

    IR = M[PC];                                         /* fetch instr */
    //if ( hist_cnt )
    //    {
    //   hist_save( PC, IR ) ;			                /*  PC, int_req unchanged */
    //    }

    INCREMENT_PC ;

    int_req = int_req | INT_NO_ION_PENDING;             /* clear ION delay */
    sim_interval = sim_interval - 1;

/* Operate instruction */

    if (IR & I_OPR) {                                   /* operate? */
        int32 src, srcAC, dstAC;

        srcAC = I_GETSRC (IR);                          /* get reg decodes */
        dstAC = I_GETDST (IR);
        switch (I_GETCRY (IR)) {                        /* decode carry */
        case 0:                                         /* load */
            src = AC[srcAC] | C;
            break;
        case 1:                                         /* clear */
            src = AC[srcAC];
            break;
        case 2:                                         /* set */
            src = AC[srcAC] | CBIT;
            break;
        case 3:                                         /* complement */
            src = AC[srcAC] | (C ^ CBIT);
            break;
            }                                           /* end switch carry */

        switch (I_GETALU (IR)) {                        /* decode ALU */
        case 0:                                         /* COM */
            src = src ^ DMASK;
            break;
        case 1:                                         /* NEG */
            src = ((src ^ DMASK) + 1) & CDMASK;
            break;
        case 2:                                         /* MOV */
            break;
        case 3:                                         /* INC */
            src = (src + 1) & CDMASK;
            break;
        case 4:                                         /* ADC */
            src = ((src ^ DMASK) + AC[dstAC]) & CDMASK;
            break;
        case 5:                                         /* SUB */
            src = ((src ^ DMASK) + AC[dstAC] + 1) & CDMASK;
            break;
        case 6:                                         /* ADD */
            src = (src + AC[dstAC]) & CDMASK;
            break;
        case 7:                                         /* AND */
            src = src & (AC[dstAC] | CBIT);
            break;
            }                                           /* end switch oper */

        switch (I_GETSHF (IR)) {                        /* decode shift */
        case 0:                                         /* nop */
            break;
        case 1:                                         /* L */
            src = ((src << 1) | (src >> 16)) & CDMASK;
            break;
        case 2:                                         /* R */
            src = ((src >> 1) | (src << 16)) & CDMASK;
            break;
        case 3:                                         /* S */
            src = ((src & 0377) << 8) | ((src >> 8) & 0377) |
                (src & CBIT);
            break;
            }                                           /* end switch shift */

        switch (I_GETSKP (IR)) {                        /* decode skip */
        case 0:                                         /* nop */
            break;
        case 1:                                         /* SKP */
            INCREMENT_PC ;
            break;
        case 2:                                         /* SZC */
            if (src < CBIT)
                INCREMENT_PC ;
            break;
        case 3:                                         /* SNC */
            if (src >= CBIT)
                INCREMENT_PC ;
            break;
        case 4:                                         /* SZR */
            if ((src & DMASK) == 0)
                INCREMENT_PC ;
            break;
        case 5:                                         /* SNR */
            if ((src & DMASK) != 0)
                INCREMENT_PC ;
            break;
        case 6:                                         /* SEZ */
            if (src <= CBIT)
                INCREMENT_PC ;
            break;
        case 7:                                         /* SBN */
            if (src > CBIT)
                INCREMENT_PC ;
            break;
            }                                           /* end switch skip */
        if ((IR & I_NLD) == 0) {                        /* load? */
            AC[dstAC] = src & DMASK;
            C = src & CBIT;
            }                                           /* end if load */
        }                                               /* end if operate */

/* Memory reference instructions */

    else if (IR < 060000) {                             /* mem ref? */
        int32 src, MA, indf;

        MA = I_GETDISP (IR);                            /* get disp */
        switch (I_GETMODE (IR)) {                       /* decode mode */
        case 0:                                         /* page zero */
            break;
        case 1:                                         /* PC relative */
            if (MA & DISPSIGN)
                MA = 0177400 | MA;
            MA = (MA + PC - 1) & AMASK;
            break;
        case 2:                                         /* AC2 relative */
            if (MA & DISPSIGN)
                MA = 0177400 | MA;
            MA = (MA + AC[2]) & AMASK;
            break;
        case 3:                                         /* AC3 relative */
            if (MA & DISPSIGN)
                MA = 0177400 | MA;
            MA = (MA + AC[3]) & AMASK;
            break;
            }                                           /* end switch mode */

        if ( (indf = IR & I_IND) ) {                    /* indirect? */
                {
                 for (i = 0; indf && (i < ind_max); i++) {   /* count */
                    indf = IND_STEP (MA);               /* resolve indirect */
                }
                if (i >= ind_max) {                     /* too many? */
                    reason = STOP_IND;
                    break;
                }
            }
        }

        switch (I_GETOPAC (IR)) {                       /* decode op + AC */
        case 001:                                       /* JSR */
            AC[3] = PC;
        case 000:                                       /* JMP */
            //PCQ_ENTRY;
            PC = MA;
            break;
        case 002:                                       /* ISZ */
            src = (M[MA] + 1) & DMASK;
            if (MEM_ADDR_OK(MA))
                M[MA] = src;
            if (src == 0)
                INCREMENT_PC ;
            break;
        case 003:                                       /* DSZ */
            src = (M[MA] - 1) & DMASK;
            if (MEM_ADDR_OK(MA))
                M[MA] = src;
            if (src == 0)
                INCREMENT_PC ;
            break;
        case 004:                                       /* LDA 0 */
            AC[0] = M[MA];
            break;
        case 005:                                       /* LDA 1 */
            AC[1] = M[MA];
            break;
        case 006:                                       /* LDA 2 */
            AC[2] = M[MA];
            break;
        case 007:                                       /* LDA 3 */
            AC[3] = M[MA];
            break;
        case 010:                                       /* STA 0 */
            if (MEM_ADDR_OK(MA))
                M[MA] = AC[0];
            break;
        case 011:                                       /* STA 1 */
            if (MEM_ADDR_OK(MA))
                M[MA] = AC[1];
            break;
        case 012:                                       /* STA 2 */
            if (MEM_ADDR_OK(MA))
                M[MA] = AC[2];
            break;
        case 013:                                       /* STA 3 */
            if (MEM_ADDR_OK(MA))
                M[MA] = AC[3];
            break;
            }                                           /* end switch */
        }                                               /* end mem ref */

/* IOT instruction */

    else {                                              /* IOT */
        int32 dstAC, pulse, code, device, iodata;

        dstAC = I_GETDST (IR);                          /* decode fields */
        code = I_GETIOT (IR);
        pulse = I_GETPULSE (IR);
        device = I_GETDEV (IR);
        if (code == ioSKP) {                            /* IO skip? */
            switch (pulse) {                            /* decode IR<8:9> */

            case 0:                                     /* skip if busy */
                if ((device == DEV_CPU)? (int_req & INT_ION) != 0:
                    (dev_busy & dev_table[device].mask) != 0)
                    INCREMENT_PC ;
                break;

            case 1:                                     /* skip if not busy */
                if ((device == DEV_CPU)? (int_req & INT_ION) == 0:
                    (dev_busy & dev_table[device].mask) == 0)
                    INCREMENT_PC ;
                break;

            case 2:                                     /* skip if done */
                if ((device == DEV_CPU)? pwr_low != 0:
                    (dev_done & dev_table[device].mask) != 0)
                    INCREMENT_PC ;
                break;

            case 3:                                     /* skip if not done */
                if ((device == DEV_CPU)? pwr_low == 0:
                    (dev_done & dev_table[device].mask) == 0)
                    INCREMENT_PC ;
                break;
                }                                       /* end switch */
            }                                           /* end IO skip */

        else if (device == DEV_CPU) {                   /* CPU control */
            switch (code) {                             /* decode IR<5:7> */

            case ioNIO:                                     /* NIOP <x> CPU ? */
              if ( pulse == iopP )
                break ;

            case ioDIA:                                 /* read switches */
                AC[dstAC] = SR;
                break;

            case ioDIB:                                 /* int ack */
                AC[dstAC] = 0;
                DEV_UPDATE_INTR ;
                iodata = int_req & (-int_req);
                for (i = DEV_LOW; i <= DEV_HIGH; i++)  {
                    if (iodata & dev_table[i].mask) {
                        AC[dstAC] = i;
                        break;
                        }
                    }
                break;

            case ioDOB:                                 /* mask out */
                mask_out (pimask = AC[dstAC]);
                break;

            case ioDIC:                                 /* io reset */
                reset_all (0);                          /* reset devices */
                mask_out( 0 ) ;                         /* clear all device masks  */
                AMASK = 077777 ;                        /* reset memory mode */
                break;

            case ioDOC:                                 /* halt */
                reason = STOP_HALT;
                break;
                }                                       /* end switch code */

            switch (pulse) {                            /* decode IR<8:9> */

            case iopS:                                  /* ion */
                int_req = (int_req | INT_ION) & ~INT_NO_ION_PENDING;
                break;

            case iopC:                                  /* iof */
                int_req = int_req & ~INT_ION;
                break;
                }                                       /* end switch pulse */
            }                                           /* end CPU control */

        else if (dev_table[device].routine) {           /* normal device */
            iodata = dev_table[device].routine (pulse, code, AC[dstAC]);
            reason = iodata >> IOT_V_REASON;
            if (code & 1)
                AC[dstAC] = iodata & 0177777;
            }

/* bkr, 2007-May-30
 *    if device does not exist certain I/O instructions will still
 *    return data: DIA/B/C will return idle data bus value and
 *    SKPBZ/SKPDZ will sense zero value (and will therefore skip).
 *
 *    Perform these non-supported device functions only if 'stop_dev'
 *    is zero (i.e. I/O access trap is not in effect).
 */
    else if ( stop_dev == 0 )
        {
        switch (code)                                   /* decode IR<5:7> */
            {
        case ioDIA:
        case ioDIB:
        case ioDIC:
            AC[dstAC] = 0 ;  /*  idle I/O bus data  */
            break;

        case ioSKP:
            /*  (This should have been caught in previous CPU skip code)  */
            if ( (pulse == 1 /* SKPBZ */) || (pulse == 3 /* SKPDZ */) )
                {
                INCREMENT_PC ;
                }
            }    /*  end of 'switch'  */
        }    /*  end of handling non-existant device  */
      else reason = stop_dev;
      }                                                 /* end if IOT */
    }                                                   /* end while */

/* Simulation halted */

saved_PC = PC;
//pcq_r->qptr = pcq_p;                                    /* update pc q ptr */
return ( reason ) ;
}
