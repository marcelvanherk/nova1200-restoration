#include <stdio.h>

/////////////////////// disassembler ////////////////////////
// part of disassembler - print shift in operate instruction

void PrintShift(unsigned int v) {
  int s = (v&0xc0)>>6;
  if (s==0) printf("");
  if (s==1) printf("L");
  if (s==2) printf("R");
  if (s==3) printf("S"); // byte swap
}

// part of disassembler - print carry op in operate instruction
void PrintCarry(unsigned int v) {
  int s = (v&0x3)>>4;
  if (s==0) printf("");
  if (s==1) printf("Z");
  if (s==2) printf("O");
  if (s==3) printf("C");
}

// part of disassembler - print noload in operate instruction
void PrintNoload(unsigned int v) {
  int s = (v&0x8)>>3;
  if (s==1) printf("#");
}

// part of disassembler - print skip part of operate instruction
void PrintSkip(unsigned int v) {
  int s=v&7;
  if (s==1) printf(" skp");
  if (s==2) printf(" szc");
  if (s==3) printf(" snc");
  if (s==4) printf(" szr");
  if (s==5) printf(" snr");
  if (s==6) printf(" sez");
  if (s==7) printf(" sbn");
}

// part of disassembler - print target of memory access instruction
void PrintTarget(unsigned int v, int a) {
  int mode=(v&0x700)>>8;
  if (mode==0) { printf(" "); printf("%x", v&255); }
  if (mode==1) printf(" pc"); 
  if (mode==2) printf(" ac2"); 
  if (mode==3) printf(" ac3");
  if (mode>=4) printf(" @");
  if (mode==4) { printf("%x", v&255); }
  if (mode==5) printf("pc"); 
  if (mode==6) printf("ac2"); 
  if (mode==7) printf("ac3");
  if (mode==1 || mode==2 || mode==3 || mode==5 || mode==6 || mode==7) 
  { if (v&0x80)
    { printf("-"); printf("%x", 128-(v&127));
      if (mode==1) printf(" ; %x", a-(128-(v&127)));
      if (mode==5) printf(" ; @%x", a-(128-(v&127)));
    }
    else
    { printf("+"); printf("%x", v&127); 
      if (mode==1) printf(" ; %x", a+(v&127));
      if (mode==5) printf(" ; @%x", a+(v&127));
    }
  }
}

// part of disassembler - print transfer and control in IOT instruction
void PrintIOT(unsigned int v) {
  int tr = (v&0x700)>>8;
  int sk = (v&0xc0)>>6;
  if (tr==0) printf("NIO");
  if (tr==1) printf("DIA");
  if (tr==2) printf("DOA");
  if (tr==3) printf("DIB");
  if (tr==4) printf("DOB");
  if (tr==5) printf("DIC");
  if (tr==6) printf("DOC");
  if (tr==7)
  { printf("SKP");
    if (sk==0) printf("BN");
    if (sk==1) printf("BZ");
    if (sk==2) printf("DN");
    if (sk==3) printf("DZ");
  }
  else	// control part
  { if (sk==0) printf("");
    if (sk==1) printf("S");
    if (sk==2) printf("C");
    if (sk==3) printf("P");
  }
}

// Disassemble entire instruction and print to 
void PrintDisas(unsigned int v, int a) {
  unsigned int mref = (v>>11)&0x1f;
  unsigned int opcode = (v>>8)&0x7;

  // cpu control instructions
  if      ((v&0xff3f)==0x653f)  printf("IORST"); //dicc0
  else if ((v&0xff3f)==0x663f)  printf("HALT");//doc0
  else if ((v&0xe73f)==0x613f) {printf("READS");printf("%x", mref&3);}//dia#
  else if (v==0x607f) printf("INTEN"); // nios0
  else if (v==0x60bf) printf("INTDS"); // nioc0
  else if (v==0x673f) printf("SKPBN CPU");
  else if (v==0x677f) printf("SKPBZ CPU");
  else if (v==0x67bf) printf("SKPDN CPU");
  else if (v==0x67ff) printf("SKPDZ CPU");
  else if ((v&0xe73f)==0x643f) {printf("MSKO");printf("%x", mref&3);}//dob#
  else if ((v&0xe73f)==0x633f) {printf("INTA");printf("%x", mref&3);}//dib#

  // data access instructions
  else if (mref==0) { printf("JMP"); PrintTarget(v, a); }
  else if (mref==1) { printf("JMS"); PrintTarget(v, a); }
  else if (mref==2) { printf("ISZ"); PrintTarget(v, a); }
  else if (mref==3) { printf("DSZ"); PrintTarget(v, a); }
  else if ((mref&0x1c)==4) { printf("LDA"); printf(" %x", mref&3); PrintTarget(v, a); }
  else if ((mref&0x1c)==8) { printf("STA"); printf(" %x", mref&3); PrintTarget(v, a); }

  // IOT instructions
  else if ((mref&0x1c)==12)
  { PrintIOT(v); 
    printf(" %x", mref&3); 
    printf(" %x", v&0x3f); 
  }

  // operate instructions
  else if ((mref&0x10)==16)
  { if (opcode==0)  printf("CMP");
    if (opcode==1)  printf("NEG");
    if (opcode==2)  printf("MOV");
    if (opcode==3)  printf("INC");
    if (opcode==4)  printf("ADC");
    if (opcode==5)  printf("SUB");
    if (opcode==6)  printf("ADD");
    if (opcode==7)  printf("AND");
    PrintNoload(v);
    PrintShift(v);
    PrintCarry(v);
    printf(" %x", (mref&12)>>2); // source
    printf(" %x", mref&3);	     // destination
    PrintSkip(v);
  }
  else 
  { printf("?"); printf("%04x", v); } // unknown instruction
}

void main(void)
{ FILE *f;
  char line[160];
  int a, b;

  f = fopen("C:\\Users\\marcel\\Desktop\\nova1200\\nova_fulldump.txt", "rt");
  do
  { fgets(line, 160, f);
    sscanf(line, "%x:%x", &a, &b);
    printf("%04x %04x   ", a, b);
    PrintDisas(b, a);
    printf("\n");
  }
  while(!feof(f));
  close(f);
}