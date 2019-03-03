EESchema Schematic File Version 4
LIBS:nova1200-cache
EELAYER 26 0
EELAYER END
$Descr A3 16535 11693
encoding utf-8
Sheet 1 1
Title "Computer controlled front panel for Nova 1200 computer"
Date "2019-03-02"
Rev "2"
Comp "Marcel van Herk"
Comment1 "Add pinheader for data switches"
Comment2 "Add pinheader for control switches"
Comment3 "Idea: add extra register on SEL7"
Comment4 "Swapped EXEC and DEFER; added Teensy jumper"
$EndDescr
$Comp
L 74xx:74LS257 U1
U 1 1 5C27731E
P 1300 1600
F 0 "U1" H 1300 2678 50  0000 C CNN
F 1 "74LS257" H 1300 2587 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W10.16mm" H 1300 1600 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS257" H 1300 1600 50  0001 C CNN
	1    1300 1600
	-1   0    0    -1  
$EndComp
$Comp
L 74xx:74LS173 U5
U 1 1 5C277461
P 1350 4100
F 0 "U5" H 1350 5178 50  0000 C CNN
F 1 "74LS173" H 1350 5087 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W10.16mm" H 1350 4100 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS173" H 1350 4100 50  0001 C CNN
	1    1350 4100
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x26_Odd_Even J1
U 1 1 5C278B61
P 1150 7700
F 0 "J1" H 1200 9117 50  0000 C CNN
F 1 "Conn_02x26_Odd_Even" H 1200 9026 50  0000 C CNN
F 2 "Connector:nova_fp" H 1150 7700 50  0001 C CNN
F 3 "~" H 1150 7700 50  0001 C CNN
	1    1150 7700
	1    0    0    -1  
$EndComp
$Comp
L MCU_Module:Arduino_Nano_v3.x A1
U 1 1 5C278C2E
P 13400 6600
F 0 "A1" H 13400 5514 50  0000 C CNN
F 1 "Arduino_Nano_v3.x" H 13400 5423 50  0000 C CNN
F 2 "Module:Arduino_Nano" H 13550 5650 50  0001 L CNN
F 3 "http://www.mouser.com/pdfdocs/Gravitech_Arduino_Nano3_0.pdf" H 13400 5600 50  0001 C CNN
	1    13400 6600
	1    0    0    -1  
$EndComp
Text Label 1450 6500 0    50   ~ 0
VCC
Text Label 800  1000 2    50   ~ 0
D0
Text Label 800  1300 2    50   ~ 0
D1
Text Label 800  1600 2    50   ~ 0
D2
Text Label 800  1900 2    50   ~ 0
D3
NoConn ~ 7650 7350
Text Label 8650 6550 0    50   ~ 0
A0
Text Label 8650 6650 0    50   ~ 0
A1
Text Label 8650 6750 0    50   ~ 0
A2
Text Label 8650 6850 0    50   ~ 0
SELR1
Text Label 7650 6550 2    50   ~ 0
SEL0
Text Label 7650 6650 2    50   ~ 0
SEL1
Text Label 7650 6750 2    50   ~ 0
SEL2
Text Label 7650 6850 2    50   ~ 0
SEL3
Text Label 7650 6950 2    50   ~ 0
SEL4
Text Label 7650 7050 2    50   ~ 0
SEL5
Text Label 7650 7150 2    50   ~ 0
SEL6
Text Label 7650 7250 2    50   ~ 0
SEL7
Text Label 850  4500 2    50   ~ 0
SEL0
Text Label 850  3500 2    50   ~ 0
D0
Text Label 850  3600 2    50   ~ 0
D1
Text Label 850  3700 2    50   ~ 0
D2
Text Label 850  3800 2    50   ~ 0
D3
Text Label 12900 6200 2    50   ~ 0
D0
Text Label 12900 6300 2    50   ~ 0
D1
Text Label 12900 6400 2    50   ~ 0
D2
Text Label 12900 6500 2    50   ~ 0
D3
Text Label 12900 6600 2    50   ~ 0
A0
Text Label 12900 6700 2    50   ~ 0
A1
Text Label 12900 6800 2    50   ~ 0
SELR1
Text Label 12900 6900 2    50   ~ 0
SELR2
Text Label 12900 7000 2    50   ~ 0
A2
Text Label 12900 7100 2    50   ~ 0
A0'
Text Label 12900 7200 2    50   ~ 0
ENLCD1
Text Label 12900 7300 2    50   ~ 0
ENLCD2
Text Label 13900 6600 0    50   ~ 0
KEYL
Text Label 13900 6700 0    50   ~ 0
KEYM
Text Label 13900 6800 0    50   ~ 0
LCD4
Text Label 13900 6900 0    50   ~ 0
LCD5
Text Label 13900 7000 0    50   ~ 0
LCD6
Text Label 13900 7100 0    50   ~ 0
LCD7
Text Label 13900 7200 0    50   ~ 0
KEYP
NoConn ~ 13900 7300
Text Label 1800 2200 0    50   ~ 0
A0'
Text Label 950  6600 2    50   ~ 0
ND15
Text Label 950  6700 2    50   ~ 0
ND14
Text Label 950  6800 2    50   ~ 0
NA13
Text Label 950  6900 2    50   ~ 0
ND12
Text Label 950  7000 2    50   ~ 0
NA11
Text Label 950  7100 2    50   ~ 0
ND9
Text Label 950  7200 2    50   ~ 0
NA9
Text Label 950  7300 2    50   ~ 0
NA7
Text Label 950  6500 2    50   ~ 0
GND
Text Label 950  7400 2    50   ~ 0
ND6
Text Label 950  7500 2    50   ~ 0
NA5
Text Label 950  7600 2    50   ~ 0
NA14
Text Label 950  7700 2    50   ~ 0
ND2
Text Label 950  7800 2    50   ~ 0
NA1
Text Label 950  7900 2    50   ~ 0
NA2
Text Label 950  8000 2    50   ~ 0
NA4
Text Label 950  8100 2    50   ~ 0
GND
Text Label 950  8200 2    50   ~ 0
NA8
NoConn ~ 950  8300
Text Label 950  8400 2    50   ~ 0
RST
Text Label 950  8500 2    50   ~ 0
CONREQ
Text Label 950  8600 2    50   ~ 0
CONINST
Text Label 950  8700 2    50   ~ 0
PL
Text Label 950  8800 2    50   ~ 0
ISTP
Text Label 950  8900 2    50   ~ 0
ION
Text Label 950  9000 2    50   ~ 0
RUN
Text Label 1450 6600 0    50   ~ 0
NA15
Text Label 1450 6700 0    50   ~ 0
ND13
Text Label 1450 6800 0    50   ~ 0
NA12
Text Label 1450 6900 0    50   ~ 0
ND11
Text Label 1450 7000 0    50   ~ 0
ND10
NoConn ~ 1450 7100
Text Label 1450 7200 0    50   ~ 0
ND8
Text Label 1450 7300 0    50   ~ 0
NA6
Text Label 1450 7400 0    50   ~ 0
ND5
Text Label 1450 7500 0    50   ~ 0
ND4
Text Label 1450 7600 0    50   ~ 0
NA3
Text Label 1450 7700 0    50   ~ 0
ND0
Text Label 1450 7800 0    50   ~ 0
GND
Text Label 1450 7900 0    50   ~ 0
ND1
Text Label 1450 8100 0    50   ~ 0
ND3
Text Label 1450 8200 0    50   ~ 0
NA10
Text Label 1450 8300 0    50   ~ 0
STOP
Text Label 1450 8400 0    50   ~ 0
CONTDATA
Text Label 1450 8500 0    50   ~ 0
CON+
Text Label 1450 8600 0    50   ~ 0
MSTP
Text Label 1450 8000 0    50   ~ 0
ND7
Text Label 1450 8700 0    50   ~ 0
CARRY
Text Label 1450 8800 0    50   ~ 0
FETCH
Text Label 1450 8900 0    50   ~ 0
EXEC
Text Label 1450 9000 0    50   ~ 0
DEFER
Text Notes 1500 7100 0    50   ~ 0
vlamp
NoConn ~ 7650 9100
Text Label 8650 8300 0    50   ~ 0
A0
Text Label 8650 8400 0    50   ~ 0
A1
Text Label 8650 8500 0    50   ~ 0
A2
Text Label 8650 8600 0    50   ~ 0
SELR2
Text Label 7650 8300 2    50   ~ 0
SEL8
Text Label 7650 8400 2    50   ~ 0
SEL9
Text Label 7650 8500 2    50   ~ 0
SEL10
Text Label 7650 8600 2    50   ~ 0
SEL11
Text Label 7650 8700 2    50   ~ 0
SEL12
Text Label 7650 8800 2    50   ~ 0
SEL13
Text Label 7650 8900 2    50   ~ 0
SEL14
Text Label 7650 9000 2    50   ~ 0
SEL15
$Comp
L 74xx:74LS173 U6
U 1 1 5C27C653
P 2700 4100
F 0 "U6" H 2700 5178 50  0000 C CNN
F 1 "74LS173" H 2700 5087 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W10.16mm" H 2700 4100 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS173" H 2700 4100 50  0001 C CNN
	1    2700 4100
	1    0    0    -1  
$EndComp
Text Label 2200 4500 2    50   ~ 0
SEL1
Text Label 2200 3500 2    50   ~ 0
D0
Text Label 2200 3600 2    50   ~ 0
D1
Text Label 2200 3700 2    50   ~ 0
D2
Text Label 2200 3800 2    50   ~ 0
D3
$Comp
L 74xx:74LS173 U7
U 1 1 5C27C686
P 4100 4100
F 0 "U7" H 4100 5178 50  0000 C CNN
F 1 "74LS173" H 4100 5087 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W10.16mm" H 4100 4100 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS173" H 4100 4100 50  0001 C CNN
	1    4100 4100
	1    0    0    -1  
$EndComp
Text Label 3600 4500 2    50   ~ 0
SEL2
Text Label 3600 3500 2    50   ~ 0
D0
Text Label 3600 3600 2    50   ~ 0
D1
Text Label 3600 3700 2    50   ~ 0
D2
Text Label 3600 3800 2    50   ~ 0
D3
Text Label 1850 3500 0    50   ~ 0
ND15
Text Label 1850 3600 0    50   ~ 0
ND14
Text Label 1850 3700 0    50   ~ 0
ND13
Text Label 1850 3800 0    50   ~ 0
ND12
Text Label 3200 3800 0    50   ~ 0
ND8
Text Label 3200 3700 0    50   ~ 0
ND9
Text Label 3200 3600 0    50   ~ 0
ND10
Text Label 3200 3500 0    50   ~ 0
ND11
Text Label 4600 3500 0    50   ~ 0
ND15
Text Label 4600 3600 0    50   ~ 0
ND14
Text Label 4600 3700 0    50   ~ 0
ND13
Text Label 4600 3800 0    50   ~ 0
ND12
$Comp
L 74xx:74LS173 U8
U 1 1 5C27D937
P 5500 4100
F 0 "U8" H 5500 5178 50  0000 C CNN
F 1 "74LS173" H 5500 5087 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W10.16mm" H 5500 4100 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS173" H 5500 4100 50  0001 C CNN
	1    5500 4100
	1    0    0    -1  
$EndComp
Text Label 5000 4500 2    50   ~ 0
SEL3
Text Label 5000 3500 2    50   ~ 0
D0
Text Label 5000 3600 2    50   ~ 0
D1
Text Label 5000 3700 2    50   ~ 0
D2
Text Label 5000 3800 2    50   ~ 0
D3
Text Label 6000 3800 0    50   ~ 0
ND8
Text Label 6000 3700 0    50   ~ 0
ND9
Text Label 6000 3600 0    50   ~ 0
ND10
Text Label 6000 3500 0    50   ~ 0
ND11
$Comp
L 74xx:74LS173 U9
U 1 1 5C27DAC7
P 7000 4100
F 0 "U9" H 7000 5178 50  0000 C CNN
F 1 "74LS173" H 7000 5087 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W10.16mm" H 7000 4100 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS173" H 7000 4100 50  0001 C CNN
	1    7000 4100
	1    0    0    -1  
$EndComp
Text Label 6500 4500 2    50   ~ 0
SEL8
Text Label 6500 3500 2    50   ~ 0
D0
Text Label 6500 3600 2    50   ~ 0
D1
Text Label 6500 3700 2    50   ~ 0
D2
Text Label 6500 3800 2    50   ~ 0
D3
$Comp
L 74xx:74LS173 U10
U 1 1 5C27DAD3
P 8400 4100
F 0 "U10" H 8400 5178 50  0000 C CNN
F 1 "74LS173" H 8400 5087 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W10.16mm" H 8400 4100 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS173" H 8400 4100 50  0001 C CNN
	1    8400 4100
	1    0    0    -1  
$EndComp
Text Label 7900 4500 2    50   ~ 0
SEL9
Text Label 7900 3500 2    50   ~ 0
D0
Text Label 7900 3600 2    50   ~ 0
D1
Text Label 7900 3700 2    50   ~ 0
D2
Text Label 7900 3800 2    50   ~ 0
D3
$Comp
L 74xx:74LS173 U11
U 1 1 5C27DADF
P 9800 4100
F 0 "U11" H 9800 5178 50  0000 C CNN
F 1 "74LS173" H 9800 5087 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W10.16mm" H 9800 4100 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS173" H 9800 4100 50  0001 C CNN
	1    9800 4100
	1    0    0    -1  
$EndComp
Text Label 9300 4500 2    50   ~ 0
SEL14
Text Label 9300 3500 2    50   ~ 0
D0
Text Label 9300 3600 2    50   ~ 0
D1
Text Label 9300 3700 2    50   ~ 0
D2
Text Label 9300 3800 2    50   ~ 0
D3
Text Label 7500 3500 0    50   ~ 0
ND7
Text Label 7500 3600 0    50   ~ 0
ND6
Text Label 7500 3700 0    50   ~ 0
ND5
Text Label 7500 3800 0    50   ~ 0
ND4
Text Label 8900 3800 0    50   ~ 0
ND0
Text Label 8900 3700 0    50   ~ 0
ND1
Text Label 8900 3600 0    50   ~ 0
ND2
Text Label 8900 3500 0    50   ~ 0
ND3
$Comp
L 74xx:74LS173 U12
U 1 1 5C27DAF7
P 11200 4100
F 0 "U12" H 11200 5178 50  0000 C CNN
F 1 "74LS173" H 11200 5087 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W10.16mm" H 11200 4100 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS173" H 11200 4100 50  0001 C CNN
	1    11200 4100
	1    0    0    -1  
$EndComp
Text Label 10700 4500 2    50   ~ 0
SEL15
Text Label 10700 3500 2    50   ~ 0
D0
Text Label 10700 3600 2    50   ~ 0
D1
Text Label 10700 3700 2    50   ~ 0
D2
Text Label 10700 3800 2    50   ~ 0
D3
Text Label 10300 3500 0    50   ~ 0
ND7
Text Label 10300 3600 0    50   ~ 0
ND6
Text Label 10300 3700 0    50   ~ 0
ND5
Text Label 10300 3800 0    50   ~ 0
ND4
Text Label 11700 3800 0    50   ~ 0
ND0
Text Label 11700 3700 0    50   ~ 0
ND1
Text Label 11700 3600 0    50   ~ 0
ND2
Text Label 11700 3500 0    50   ~ 0
ND3
$Comp
L 74xx:74LS173 U13
U 1 1 5C27DDFD
P 13250 3850
F 0 "U13" H 13250 4928 50  0000 C CNN
F 1 "74LS173" H 13250 4837 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W10.16mm" H 13250 3850 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS173" H 13250 3850 50  0001 C CNN
	1    13250 3850
	1    0    0    -1  
$EndComp
Text Label 12750 4250 2    50   ~ 0
SEL10
Text Label 12750 3250 2    50   ~ 0
D0
Text Label 12750 3350 2    50   ~ 0
D1
Text Label 12750 3450 2    50   ~ 0
D2
Text Label 12750 3550 2    50   ~ 0
D3
Text Label 14300 4250 2    50   ~ 0
SEL11
Text Label 14300 3250 2    50   ~ 0
D0
Text Label 14300 3350 2    50   ~ 0
D1
Text Label 14300 3450 2    50   ~ 0
D2
Text Label 14300 3550 2    50   ~ 0
D3
Text Label 13750 3250 0    50   ~ 0
ISTP
Text Label 13750 3350 0    50   ~ 0
PL
Text Label 13750 3450 0    50   ~ 0
RST
Text Label 15300 3250 0    50   ~ 0
MSTP
Text Label 15300 3350 0    50   ~ 0
CON+
Text Label 15300 3450 0    50   ~ 0
CONREQ
Text Label 15300 3550 0    50   ~ 0
STOP
Text Label 12750 4050 2    50   ~ 0
GND
Text Label 12750 4150 2    50   ~ 0
GND
Text Label 14300 4050 2    50   ~ 0
GND
Text Label 14300 4150 2    50   ~ 0
GND
Text Label 12750 3750 2    50   ~ 0
GND
Text Label 12750 3850 2    50   ~ 0
GND
Text Label 14300 3750 2    50   ~ 0
GND
Text Label 14300 3850 2    50   ~ 0
GND
$Comp
L 74xx:74LS257 U2
U 1 1 5C280002
P 2650 1600
F 0 "U2" H 2650 2678 50  0000 C CNN
F 1 "74LS257" H 2650 2587 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W10.16mm" H 2650 1600 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS257" H 2650 1600 50  0001 C CNN
	1    2650 1600
	-1   0    0    -1  
$EndComp
Text Label 2150 1000 2    50   ~ 0
D0
Text Label 2150 1300 2    50   ~ 0
D1
Text Label 2150 1600 2    50   ~ 0
D2
Text Label 2150 1900 2    50   ~ 0
D3
Text Label 3150 2200 0    50   ~ 0
A0'
$Comp
L 74xx:74LS257 U3
U 1 1 5C28003A
P 4100 1600
F 0 "U3" H 4100 2678 50  0000 C CNN
F 1 "74LS257" H 4100 2587 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W10.16mm" H 4100 1600 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS257" H 4100 1600 50  0001 C CNN
	1    4100 1600
	-1   0    0    -1  
$EndComp
Text Label 3600 1000 2    50   ~ 0
D0
Text Label 3600 1300 2    50   ~ 0
D1
Text Label 3600 1600 2    50   ~ 0
D2
Text Label 3600 1900 2    50   ~ 0
D3
Text Label 4600 2200 0    50   ~ 0
A0'
$Comp
L 74xx:74LS257 U4
U 1 1 5C280069
P 5500 1600
F 0 "U4" H 5500 2678 50  0000 C CNN
F 1 "74LS257" H 5500 2587 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W10.16mm" H 5500 1600 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS257" H 5500 1600 50  0001 C CNN
	1    5500 1600
	-1   0    0    -1  
$EndComp
Text Label 5000 1000 2    50   ~ 0
D0
Text Label 5000 1300 2    50   ~ 0
D1
Text Label 5000 1600 2    50   ~ 0
D2
Text Label 5000 1900 2    50   ~ 0
D3
Text Label 6000 2200 0    50   ~ 0
A0'
$Comp
L 74xx:74LS257 U15
U 1 1 5C2802C8
P 13450 1650
F 0 "U15" H 13450 2728 50  0000 C CNN
F 1 "74LS257" H 13450 2637 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W10.16mm" H 13450 1650 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS257" H 13450 1650 50  0001 C CNN
	1    13450 1650
	-1   0    0    -1  
$EndComp
Text Label 12950 1050 2    50   ~ 0
D0
Text Label 12950 1350 2    50   ~ 0
D1
Text Label 12950 1650 2    50   ~ 0
D2
Text Label 12950 1950 2    50   ~ 0
D3
Text Label 13950 2250 0    50   ~ 0
A0'
Text Label 850  4000 2    50   ~ 0
CONTDAT
Text Label 850  4100 2    50   ~ 0
CONTDAT
Text Label 2200 4000 2    50   ~ 0
CONTDAT
Text Label 2200 4100 2    50   ~ 0
CONTDAT
Text Label 6500 4000 2    50   ~ 0
CONTDAT
Text Label 6500 4100 2    50   ~ 0
CONTDAT
Text Label 7900 4000 2    50   ~ 0
CONTDAT
Text Label 7900 4100 2    50   ~ 0
CONTDAT
Text Label 3600 4000 2    50   ~ 0
CONINST
Text Label 3600 4100 2    50   ~ 0
CONINST
Text Label 5000 4000 2    50   ~ 0
CONINST
Text Label 5000 4100 2    50   ~ 0
CONINST
Text Label 9300 4000 2    50   ~ 0
CONINST
Text Label 9300 4100 2    50   ~ 0
CONINST
Text Label 10700 4000 2    50   ~ 0
CONINST
Text Label 10700 4100 2    50   ~ 0
CONINST
Text Label 1800 2300 0    50   ~ 0
SEL4
Text Label 3150 2300 0    50   ~ 0
SEL5
Text Label 6000 2300 0    50   ~ 0
SEL13
Text Label 13950 2350 0    50   ~ 0
SEL6
Text Label 4600 2300 0    50   ~ 0
SEL12
Text Label 6500 4700 2    50   ~ 0
GND
Text Label 7900 4700 2    50   ~ 0
GND
Text Label 9300 4700 2    50   ~ 0
GND
Text Label 10700 4700 2    50   ~ 0
GND
Text Label 5000 4300 2    50   ~ 0
GND
Text Label 5000 4400 2    50   ~ 0
GND
Text Label 3600 4300 2    50   ~ 0
GND
Text Label 3600 4400 2    50   ~ 0
GND
Text Label 2200 4300 2    50   ~ 0
GND
Text Label 2200 4400 2    50   ~ 0
GND
Text Label 850  4300 2    50   ~ 0
GND
Text Label 850  4400 2    50   ~ 0
GND
Text Label 6500 4300 2    50   ~ 0
GND
Text Label 6500 4400 2    50   ~ 0
GND
Text Label 7900 4300 2    50   ~ 0
GND
Text Label 7900 4400 2    50   ~ 0
GND
Text Label 9300 4300 2    50   ~ 0
GND
Text Label 9300 4400 2    50   ~ 0
GND
Text Label 10700 4300 2    50   ~ 0
GND
Text Label 10700 4400 2    50   ~ 0
GND
Text Notes 850  8300 2    50   ~ 0
rsten
Text Notes 14050 4700 2    50   ~ 0
control\nsignals
Text Notes 4850 4900 2    50   ~ 0
INST L\n(optional)
Text Notes 2100 4900 2    50   ~ 0
DATA L
Text Notes 7700 5050 2    50   ~ 0
DATA H
Text Notes 10600 4950 2    50   ~ 0
INST H
$Comp
L Device:LED_ALT D1
U 1 1 5C283219
P 2800 6050
F 0 "D1" H 2791 6266 50  0000 C CNN
F 1 "LED_ALT" H 2791 6175 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm_Clear" H 2800 6050 50  0001 C CNN
F 3 "~" H 2800 6050 50  0001 C CNN
	1    2800 6050
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 5C28331B
P 3100 6050
F 0 "R1" V 2893 6050 50  0000 C CNN
F 1 "4K7" V 2984 6050 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 3030 6050 50  0001 C CNN
F 3 "~" H 3100 6050 50  0001 C CNN
	1    3100 6050
	0    1    1    0   
$EndComp
Text Label 3250 6050 0    50   ~ 0
VCC
Text Label 2650 6050 2    50   ~ 0
ND15
$Comp
L Device:LED_ALT D2
U 1 1 5C283C49
P 2800 6350
F 0 "D2" H 2791 6566 50  0000 C CNN
F 1 "LED_ALT" H 2791 6475 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm_Clear" H 2800 6350 50  0001 C CNN
F 3 "~" H 2800 6350 50  0001 C CNN
	1    2800 6350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 5C283C50
P 3100 6350
F 0 "R2" V 2893 6350 50  0000 C CNN
F 1 "4K7" V 2984 6350 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 3030 6350 50  0001 C CNN
F 3 "~" H 3100 6350 50  0001 C CNN
	1    3100 6350
	0    1    1    0   
$EndComp
Text Label 3250 6350 0    50   ~ 0
VCC
Text Label 2650 6350 2    50   ~ 0
ND14
$Comp
L Device:LED_ALT D3
U 1 1 5C283C88
P 2800 6600
F 0 "D3" H 2791 6816 50  0000 C CNN
F 1 "LED_ALT" H 2791 6725 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm_Clear" H 2800 6600 50  0001 C CNN
F 3 "~" H 2800 6600 50  0001 C CNN
	1    2800 6600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 5C283C8F
P 3100 6600
F 0 "R3" V 2893 6600 50  0000 C CNN
F 1 "4K7" V 2984 6600 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 3030 6600 50  0001 C CNN
F 3 "~" H 3100 6600 50  0001 C CNN
	1    3100 6600
	0    1    1    0   
$EndComp
Text Label 3250 6600 0    50   ~ 0
VCC
Text Label 2650 6600 2    50   ~ 0
ND13
$Comp
L Device:LED_ALT D4
U 1 1 5C283CD9
P 2800 6850
F 0 "D4" H 2791 7066 50  0000 C CNN
F 1 "LED_ALT" H 2791 6975 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm_Clear" H 2800 6850 50  0001 C CNN
F 3 "~" H 2800 6850 50  0001 C CNN
	1    2800 6850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R4
U 1 1 5C283CE0
P 3100 6850
F 0 "R4" V 2893 6850 50  0000 C CNN
F 1 "4k7" V 2984 6850 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 3030 6850 50  0001 C CNN
F 3 "~" H 3100 6850 50  0001 C CNN
	1    3100 6850
	0    1    1    0   
$EndComp
Text Label 3250 6850 0    50   ~ 0
VCC
Text Label 2650 6850 2    50   ~ 0
ND12
$Comp
L Device:LED_ALT D5
U 1 1 5C283D1C
P 2800 7100
F 0 "D5" H 2791 7316 50  0000 C CNN
F 1 "LED_ALT" H 2791 7225 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm_Clear" H 2800 7100 50  0001 C CNN
F 3 "~" H 2800 7100 50  0001 C CNN
	1    2800 7100
	1    0    0    -1  
$EndComp
$Comp
L Device:R R5
U 1 1 5C283D23
P 3100 7100
F 0 "R5" V 2893 7100 50  0000 C CNN
F 1 "4K7" V 2984 7100 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 3030 7100 50  0001 C CNN
F 3 "~" H 3100 7100 50  0001 C CNN
	1    3100 7100
	0    1    1    0   
$EndComp
Text Label 3250 7100 0    50   ~ 0
VCC
Text Label 2650 7100 2    50   ~ 0
ND11
$Comp
L Device:LED_ALT D6
U 1 1 5C283D61
P 2800 7350
F 0 "D6" H 2791 7566 50  0000 C CNN
F 1 "LED_ALT" H 2791 7475 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm_Clear" H 2800 7350 50  0001 C CNN
F 3 "~" H 2800 7350 50  0001 C CNN
	1    2800 7350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R6
U 1 1 5C283D68
P 3100 7350
F 0 "R6" V 2893 7350 50  0000 C CNN
F 1 "4K7" V 2984 7350 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 3030 7350 50  0001 C CNN
F 3 "~" H 3100 7350 50  0001 C CNN
	1    3100 7350
	0    1    1    0   
$EndComp
Text Label 3250 7350 0    50   ~ 0
VCC
Text Label 2650 7350 2    50   ~ 0
ND10
$Comp
L Device:LED_ALT D7
U 1 1 5C283DA8
P 2800 7600
F 0 "D7" H 2791 7816 50  0000 C CNN
F 1 "LED_ALT" H 2791 7725 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm_Clear" H 2800 7600 50  0001 C CNN
F 3 "~" H 2800 7600 50  0001 C CNN
	1    2800 7600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R7
U 1 1 5C283DAF
P 3100 7600
F 0 "R7" V 2893 7600 50  0000 C CNN
F 1 "4K7" V 2984 7600 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 3030 7600 50  0001 C CNN
F 3 "~" H 3100 7600 50  0001 C CNN
	1    3100 7600
	0    1    1    0   
$EndComp
Text Label 3250 7600 0    50   ~ 0
VCC
Text Label 2650 7600 2    50   ~ 0
ND9
$Comp
L Device:LED_ALT D8
U 1 1 5C283DF1
P 2800 7850
F 0 "D8" H 2791 8066 50  0000 C CNN
F 1 "LED_ALT" H 2791 7975 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm_Clear" H 2800 7850 50  0001 C CNN
F 3 "~" H 2800 7850 50  0001 C CNN
	1    2800 7850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R8
U 1 1 5C283DF8
P 3100 7850
F 0 "R8" V 2893 7850 50  0000 C CNN
F 1 "4K7" V 2984 7850 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 3030 7850 50  0001 C CNN
F 3 "~" H 3100 7850 50  0001 C CNN
	1    3100 7850
	0    1    1    0   
$EndComp
Text Label 3250 7850 0    50   ~ 0
VCC
Text Label 2650 7850 2    50   ~ 0
ND8
$Comp
L Device:LED_ALT D9
U 1 1 5C283E3C
P 2800 8100
F 0 "D9" H 2791 8316 50  0000 C CNN
F 1 "LED_ALT" H 2791 8225 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm_Clear" H 2800 8100 50  0001 C CNN
F 3 "~" H 2800 8100 50  0001 C CNN
	1    2800 8100
	1    0    0    -1  
$EndComp
$Comp
L Device:R R9
U 1 1 5C283E43
P 3100 8100
F 0 "R9" V 2893 8100 50  0000 C CNN
F 1 "4K7" V 2984 8100 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 3030 8100 50  0001 C CNN
F 3 "~" H 3100 8100 50  0001 C CNN
	1    3100 8100
	0    1    1    0   
$EndComp
Text Label 3250 8100 0    50   ~ 0
VCC
Text Label 2650 8100 2    50   ~ 0
ND7
$Comp
L Device:LED_ALT D10
U 1 1 5C283E89
P 2800 8350
F 0 "D10" H 2791 8566 50  0000 C CNN
F 1 "LED_ALT" H 2791 8475 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm_Clear" H 2800 8350 50  0001 C CNN
F 3 "~" H 2800 8350 50  0001 C CNN
	1    2800 8350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R10
U 1 1 5C283E90
P 3100 8350
F 0 "R10" V 2893 8350 50  0000 C CNN
F 1 "4K7" V 2984 8350 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 3030 8350 50  0001 C CNN
F 3 "~" H 3100 8350 50  0001 C CNN
	1    3100 8350
	0    1    1    0   
$EndComp
Text Label 3250 8350 0    50   ~ 0
VCC
Text Label 2650 8350 2    50   ~ 0
ND6
$Comp
L Device:LED_ALT D11
U 1 1 5C283ED8
P 2800 8600
F 0 "D11" H 2791 8816 50  0000 C CNN
F 1 "LED_ALT" H 2791 8725 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm_Clear" H 2800 8600 50  0001 C CNN
F 3 "~" H 2800 8600 50  0001 C CNN
	1    2800 8600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R11
U 1 1 5C283EDF
P 3100 8600
F 0 "R11" V 2893 8600 50  0000 C CNN
F 1 "4K7" V 2984 8600 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 3030 8600 50  0001 C CNN
F 3 "~" H 3100 8600 50  0001 C CNN
	1    3100 8600
	0    1    1    0   
$EndComp
Text Label 3250 8600 0    50   ~ 0
VCC
Text Label 2650 8600 2    50   ~ 0
ND5
$Comp
L Device:LED_ALT D12
U 1 1 5C283F29
P 2800 8850
F 0 "D12" H 2791 9066 50  0000 C CNN
F 1 "LED_ALT" H 2791 8975 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm_Clear" H 2800 8850 50  0001 C CNN
F 3 "~" H 2800 8850 50  0001 C CNN
	1    2800 8850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R12
U 1 1 5C283F30
P 3100 8850
F 0 "R12" V 2893 8850 50  0000 C CNN
F 1 "4K7" V 2984 8850 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 3030 8850 50  0001 C CNN
F 3 "~" H 3100 8850 50  0001 C CNN
	1    3100 8850
	0    1    1    0   
$EndComp
Text Label 3250 8850 0    50   ~ 0
VCC
Text Label 2650 8850 2    50   ~ 0
ND4
$Comp
L Device:LED_ALT D13
U 1 1 5C283F7C
P 2800 9100
F 0 "D13" H 2791 9316 50  0000 C CNN
F 1 "LED_ALT" H 2791 9225 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm_Clear" H 2800 9100 50  0001 C CNN
F 3 "~" H 2800 9100 50  0001 C CNN
	1    2800 9100
	1    0    0    -1  
$EndComp
$Comp
L Device:R R13
U 1 1 5C283F83
P 3100 9100
F 0 "R13" V 2893 9100 50  0000 C CNN
F 1 "4K7" V 2984 9100 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 3030 9100 50  0001 C CNN
F 3 "~" H 3100 9100 50  0001 C CNN
	1    3100 9100
	0    1    1    0   
$EndComp
Text Label 3250 9100 0    50   ~ 0
VCC
Text Label 2650 9100 2    50   ~ 0
ND3
$Comp
L Device:LED_ALT D14
U 1 1 5C283FD1
P 2800 9350
F 0 "D14" H 2791 9566 50  0000 C CNN
F 1 "LED_ALT" H 2791 9475 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm_Clear" H 2800 9350 50  0001 C CNN
F 3 "~" H 2800 9350 50  0001 C CNN
	1    2800 9350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R14
U 1 1 5C283FD8
P 3100 9350
F 0 "R14" V 2893 9350 50  0000 C CNN
F 1 "4K7" V 2984 9350 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 3030 9350 50  0001 C CNN
F 3 "~" H 3100 9350 50  0001 C CNN
	1    3100 9350
	0    1    1    0   
$EndComp
Text Label 3250 9350 0    50   ~ 0
VCC
Text Label 2650 9350 2    50   ~ 0
ND2
$Comp
L Device:LED_ALT D15
U 1 1 5C284028
P 2800 9600
F 0 "D15" H 2791 9816 50  0000 C CNN
F 1 "LED_ALT" H 2791 9725 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm_Clear" H 2800 9600 50  0001 C CNN
F 3 "~" H 2800 9600 50  0001 C CNN
	1    2800 9600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R15
U 1 1 5C28402F
P 3100 9600
F 0 "R15" V 2893 9600 50  0000 C CNN
F 1 "4K7" V 2984 9600 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 3030 9600 50  0001 C CNN
F 3 "~" H 3100 9600 50  0001 C CNN
	1    3100 9600
	0    1    1    0   
$EndComp
Text Label 3250 9600 0    50   ~ 0
VCC
Text Label 2650 9600 2    50   ~ 0
ND1
$Comp
L Device:LED_ALT D16
U 1 1 5C2850CB
P 2800 9850
F 0 "D16" H 2791 10066 50  0000 C CNN
F 1 "LED_ALT" H 2791 9975 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm_Clear" H 2800 9850 50  0001 C CNN
F 3 "~" H 2800 9850 50  0001 C CNN
	1    2800 9850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R16
U 1 1 5C2850D2
P 3100 9850
F 0 "R16" V 2893 9850 50  0000 C CNN
F 1 "4K7" V 2984 9850 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 3030 9850 50  0001 C CNN
F 3 "~" H 3100 9850 50  0001 C CNN
	1    3100 9850
	0    1    1    0   
$EndComp
Text Label 3250 9850 0    50   ~ 0
VCC
Text Label 2650 9850 2    50   ~ 0
ND0
Text Label 13500 7600 3    50   ~ 0
GND
Text Label 13400 7600 3    50   ~ 0
GND
$Comp
L 74xx:74LS42 U17
U 1 1 5C27C11D
P 8150 8700
F 0 "U17" H 8150 9578 50  0000 C CNN
F 1 "74LS42" H 8150 9487 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W10.16mm" H 8150 8700 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS42" H 8150 8700 50  0001 C CNN
	1    8150 8700
	-1   0    0    -1  
$EndComp
$Comp
L 74xx:74LS173 U14
U 1 1 5C2886C8
P 14800 3850
F 0 "U14" H 14800 4928 50  0000 C CNN
F 1 "74LS173" H 14800 4837 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W10.16mm" H 14800 3850 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS173" H 14800 3850 50  0001 C CNN
	1    14800 3850
	1    0    0    -1  
$EndComp
Text Label 14300 4450 2    50   ~ 0
GND
Text Label 12750 4450 2    50   ~ 0
GND
Text Label 5000 4700 2    50   ~ 0
GND
Text Label 3600 4700 2    50   ~ 0
GND
Text Label 2200 4700 2    50   ~ 0
GND
Text Label 850  4700 2    50   ~ 0
GND
$Comp
L Device:LED_ALT D17
U 1 1 5C289CA6
P 3950 6050
F 0 "D17" H 3941 6266 50  0000 C CNN
F 1 "LED_ALT" H 3941 6175 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm_Clear" H 3950 6050 50  0001 C CNN
F 3 "~" H 3950 6050 50  0001 C CNN
	1    3950 6050
	1    0    0    -1  
$EndComp
$Comp
L Device:R R17
U 1 1 5C289CAD
P 4250 6050
F 0 "R17" V 4043 6050 50  0000 C CNN
F 1 "4K7" V 4134 6050 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 4180 6050 50  0001 C CNN
F 3 "~" H 4250 6050 50  0001 C CNN
	1    4250 6050
	0    1    1    0   
$EndComp
Text Label 4400 6050 0    50   ~ 0
VCC
Text Label 3800 6050 2    50   ~ 0
NA15
$Comp
L Device:LED_ALT D18
U 1 1 5C289CB6
P 3950 6350
F 0 "D18" H 3941 6566 50  0000 C CNN
F 1 "LED_ALT" H 3941 6475 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm_Clear" H 3950 6350 50  0001 C CNN
F 3 "~" H 3950 6350 50  0001 C CNN
	1    3950 6350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R18
U 1 1 5C289CBD
P 4250 6350
F 0 "R18" V 4043 6350 50  0000 C CNN
F 1 "4K7" V 4134 6350 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 4180 6350 50  0001 C CNN
F 3 "~" H 4250 6350 50  0001 C CNN
	1    4250 6350
	0    1    1    0   
$EndComp
Text Label 4400 6350 0    50   ~ 0
VCC
Text Label 3800 6350 2    50   ~ 0
NA14
$Comp
L Device:LED_ALT D19
U 1 1 5C289CC6
P 3950 6600
F 0 "D19" H 3941 6816 50  0000 C CNN
F 1 "LED_ALT" H 3941 6725 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm_Clear" H 3950 6600 50  0001 C CNN
F 3 "~" H 3950 6600 50  0001 C CNN
	1    3950 6600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R19
U 1 1 5C289CCD
P 4250 6600
F 0 "R19" V 4043 6600 50  0000 C CNN
F 1 "4K7" V 4134 6600 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 4180 6600 50  0001 C CNN
F 3 "~" H 4250 6600 50  0001 C CNN
	1    4250 6600
	0    1    1    0   
$EndComp
Text Label 4400 6600 0    50   ~ 0
VCC
Text Label 3800 6600 2    50   ~ 0
NA13
$Comp
L Device:LED_ALT D20
U 1 1 5C289CD6
P 3950 6850
F 0 "D20" H 3941 7066 50  0000 C CNN
F 1 "LED_ALT" H 3941 6975 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm_Clear" H 3950 6850 50  0001 C CNN
F 3 "~" H 3950 6850 50  0001 C CNN
	1    3950 6850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R20
U 1 1 5C289CDD
P 4250 6850
F 0 "R20" V 4043 6850 50  0000 C CNN
F 1 "4K7" V 4134 6850 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 4180 6850 50  0001 C CNN
F 3 "~" H 4250 6850 50  0001 C CNN
	1    4250 6850
	0    1    1    0   
$EndComp
Text Label 4400 6850 0    50   ~ 0
VCC
Text Label 3800 6850 2    50   ~ 0
NA12
$Comp
L Device:LED_ALT D21
U 1 1 5C289CE6
P 3950 7100
F 0 "D21" H 3941 7316 50  0000 C CNN
F 1 "LED_ALT" H 3941 7225 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm_Clear" H 3950 7100 50  0001 C CNN
F 3 "~" H 3950 7100 50  0001 C CNN
	1    3950 7100
	1    0    0    -1  
$EndComp
$Comp
L Device:R R21
U 1 1 5C289CED
P 4250 7100
F 0 "R21" V 4043 7100 50  0000 C CNN
F 1 "4K7" V 4134 7100 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 4180 7100 50  0001 C CNN
F 3 "~" H 4250 7100 50  0001 C CNN
	1    4250 7100
	0    1    1    0   
$EndComp
Text Label 4400 7100 0    50   ~ 0
VCC
Text Label 3800 7100 2    50   ~ 0
NA11
$Comp
L Device:LED_ALT D22
U 1 1 5C289CF6
P 3950 7350
F 0 "D22" H 3941 7566 50  0000 C CNN
F 1 "LED_ALT" H 3941 7475 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm_Clear" H 3950 7350 50  0001 C CNN
F 3 "~" H 3950 7350 50  0001 C CNN
	1    3950 7350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R22
U 1 1 5C289CFD
P 4250 7350
F 0 "R22" V 4043 7350 50  0000 C CNN
F 1 "4K7" V 4134 7350 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 4180 7350 50  0001 C CNN
F 3 "~" H 4250 7350 50  0001 C CNN
	1    4250 7350
	0    1    1    0   
$EndComp
Text Label 4400 7350 0    50   ~ 0
VCC
Text Label 3800 7350 2    50   ~ 0
NA10
$Comp
L Device:LED_ALT D23
U 1 1 5C289D06
P 3950 7600
F 0 "D23" H 3941 7816 50  0000 C CNN
F 1 "LED_ALT" H 3941 7725 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm_Clear" H 3950 7600 50  0001 C CNN
F 3 "~" H 3950 7600 50  0001 C CNN
	1    3950 7600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R23
U 1 1 5C289D0D
P 4250 7600
F 0 "R23" V 4043 7600 50  0000 C CNN
F 1 "4K7" V 4134 7600 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 4180 7600 50  0001 C CNN
F 3 "~" H 4250 7600 50  0001 C CNN
	1    4250 7600
	0    1    1    0   
$EndComp
Text Label 4400 7600 0    50   ~ 0
VCC
Text Label 3800 7600 2    50   ~ 0
NA9
$Comp
L Device:LED_ALT D24
U 1 1 5C289D16
P 3950 7850
F 0 "D24" H 3941 8066 50  0000 C CNN
F 1 "LED_ALT" H 3941 7975 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm_Clear" H 3950 7850 50  0001 C CNN
F 3 "~" H 3950 7850 50  0001 C CNN
	1    3950 7850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R24
U 1 1 5C289D1D
P 4250 7850
F 0 "R24" V 4043 7850 50  0000 C CNN
F 1 "4K7" V 4134 7850 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 4180 7850 50  0001 C CNN
F 3 "~" H 4250 7850 50  0001 C CNN
	1    4250 7850
	0    1    1    0   
$EndComp
Text Label 4400 7850 0    50   ~ 0
VCC
Text Label 3800 7850 2    50   ~ 0
NA8
$Comp
L Device:LED_ALT D25
U 1 1 5C289D26
P 3950 8100
F 0 "D25" H 3941 8316 50  0000 C CNN
F 1 "LED_ALT" H 3941 8225 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm_Clear" H 3950 8100 50  0001 C CNN
F 3 "~" H 3950 8100 50  0001 C CNN
	1    3950 8100
	1    0    0    -1  
$EndComp
$Comp
L Device:R R25
U 1 1 5C289D2D
P 4250 8100
F 0 "R25" V 4043 8100 50  0000 C CNN
F 1 "4K7" V 4134 8100 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 4180 8100 50  0001 C CNN
F 3 "~" H 4250 8100 50  0001 C CNN
	1    4250 8100
	0    1    1    0   
$EndComp
Text Label 4400 8100 0    50   ~ 0
VCC
Text Label 3800 8100 2    50   ~ 0
NA7
$Comp
L Device:LED_ALT D26
U 1 1 5C289D36
P 3950 8350
F 0 "D26" H 3941 8566 50  0000 C CNN
F 1 "LED_ALT" H 3941 8475 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm_Clear" H 3950 8350 50  0001 C CNN
F 3 "~" H 3950 8350 50  0001 C CNN
	1    3950 8350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R26
U 1 1 5C289D3D
P 4250 8350
F 0 "R26" V 4043 8350 50  0000 C CNN
F 1 "4K7" V 4134 8350 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 4180 8350 50  0001 C CNN
F 3 "~" H 4250 8350 50  0001 C CNN
	1    4250 8350
	0    1    1    0   
$EndComp
Text Label 4400 8350 0    50   ~ 0
VCC
Text Label 3800 8350 2    50   ~ 0
NA6
$Comp
L Device:LED_ALT D27
U 1 1 5C289D46
P 3950 8600
F 0 "D27" H 3941 8816 50  0000 C CNN
F 1 "LED_ALT" H 3941 8725 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm_Clear" H 3950 8600 50  0001 C CNN
F 3 "~" H 3950 8600 50  0001 C CNN
	1    3950 8600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R27
U 1 1 5C289D4D
P 4250 8600
F 0 "R27" V 4043 8600 50  0000 C CNN
F 1 "4K7" V 4134 8600 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 4180 8600 50  0001 C CNN
F 3 "~" H 4250 8600 50  0001 C CNN
	1    4250 8600
	0    1    1    0   
$EndComp
Text Label 4400 8600 0    50   ~ 0
VCC
Text Label 3800 8600 2    50   ~ 0
NA5
$Comp
L Device:LED_ALT D28
U 1 1 5C289D56
P 3950 8850
F 0 "D28" H 3941 9066 50  0000 C CNN
F 1 "LED_ALT" H 3941 8975 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm_Clear" H 3950 8850 50  0001 C CNN
F 3 "~" H 3950 8850 50  0001 C CNN
	1    3950 8850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R28
U 1 1 5C289D5D
P 4250 8850
F 0 "R28" V 4043 8850 50  0000 C CNN
F 1 "4K7" V 4134 8850 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 4180 8850 50  0001 C CNN
F 3 "~" H 4250 8850 50  0001 C CNN
	1    4250 8850
	0    1    1    0   
$EndComp
Text Label 4400 8850 0    50   ~ 0
VCC
Text Label 3800 8850 2    50   ~ 0
NA4
$Comp
L Device:LED_ALT D29
U 1 1 5C289D66
P 3950 9100
F 0 "D29" H 3941 9316 50  0000 C CNN
F 1 "LED_ALT" H 3941 9225 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm_Clear" H 3950 9100 50  0001 C CNN
F 3 "~" H 3950 9100 50  0001 C CNN
	1    3950 9100
	1    0    0    -1  
$EndComp
$Comp
L Device:R R29
U 1 1 5C289D6D
P 4250 9100
F 0 "R29" V 4043 9100 50  0000 C CNN
F 1 "4K7" V 4134 9100 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 4180 9100 50  0001 C CNN
F 3 "~" H 4250 9100 50  0001 C CNN
	1    4250 9100
	0    1    1    0   
$EndComp
Text Label 4400 9100 0    50   ~ 0
VCC
Text Label 3800 9100 2    50   ~ 0
NA3
$Comp
L Device:LED_ALT D30
U 1 1 5C289D76
P 3950 9350
F 0 "D30" H 3941 9566 50  0000 C CNN
F 1 "LED_ALT" H 3941 9475 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm_Clear" H 3950 9350 50  0001 C CNN
F 3 "~" H 3950 9350 50  0001 C CNN
	1    3950 9350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R30
U 1 1 5C289D7D
P 4250 9350
F 0 "R30" V 4043 9350 50  0000 C CNN
F 1 "4K7" V 4134 9350 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 4180 9350 50  0001 C CNN
F 3 "~" H 4250 9350 50  0001 C CNN
	1    4250 9350
	0    1    1    0   
$EndComp
Text Label 4400 9350 0    50   ~ 0
VCC
Text Label 3800 9350 2    50   ~ 0
NA2
$Comp
L Device:LED_ALT D31
U 1 1 5C289D86
P 3950 9600
F 0 "D31" H 3941 9816 50  0000 C CNN
F 1 "LED_ALT" H 3941 9725 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm_Clear" H 3950 9600 50  0001 C CNN
F 3 "~" H 3950 9600 50  0001 C CNN
	1    3950 9600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R31
U 1 1 5C289D8D
P 4250 9600
F 0 "R31" V 4043 9600 50  0000 C CNN
F 1 "4K7" V 4134 9600 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 4180 9600 50  0001 C CNN
F 3 "~" H 4250 9600 50  0001 C CNN
	1    4250 9600
	0    1    1    0   
$EndComp
Text Label 4400 9600 0    50   ~ 0
VCC
Text Label 3800 9600 2    50   ~ 0
NA1
$Comp
L Device:LED_ALT D32
U 1 1 5C289D96
P 3950 10050
F 0 "D32" H 3941 10266 50  0000 C CNN
F 1 "LED_ALT" H 3941 10175 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm_Clear" H 3950 10050 50  0001 C CNN
F 3 "~" H 3950 10050 50  0001 C CNN
	1    3950 10050
	1    0    0    -1  
$EndComp
$Comp
L Device:R R38
U 1 1 5C289D9D
P 4250 10050
F 0 "R38" V 4043 10050 50  0000 C CNN
F 1 "4K7" V 4134 10050 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 4180 10050 50  0001 C CNN
F 3 "~" H 4250 10050 50  0001 C CNN
	1    4250 10050
	0    1    1    0   
$EndComp
Text Label 4400 10050 0    50   ~ 0
VCC
Text Label 3800 10050 2    50   ~ 0
CARRY
Text Notes 3500 5900 2    50   ~ 0
lsb
Text Notes 3550 10100 2    50   ~ 0
msb
Text Label 1800 1000 0    50   ~ 0
ND15
Text Label 1800 1100 0    50   ~ 0
NA15
Text Label 1800 1300 0    50   ~ 0
ND14
Text Label 1800 1400 0    50   ~ 0
NA14
Text Label 1800 1600 0    50   ~ 0
ND13
Text Label 1800 1700 0    50   ~ 0
NA13
Text Label 1800 1900 0    50   ~ 0
ND12
Text Label 1800 2000 0    50   ~ 0
NA12
Text Label 3150 1000 0    50   ~ 0
ND11
Text Label 3150 1100 0    50   ~ 0
NA11
Text Label 3150 1300 0    50   ~ 0
ND10
Text Label 3150 1400 0    50   ~ 0
NA10
Text Label 3150 1600 0    50   ~ 0
ND9
Text Label 3150 1700 0    50   ~ 0
NA9
Text Label 3150 1900 0    50   ~ 0
ND8
Text Label 3150 2000 0    50   ~ 0
NA8
Text Label 4600 1000 0    50   ~ 0
ND7
Text Label 4600 1300 0    50   ~ 0
ND6
Text Label 4600 1600 0    50   ~ 0
ND5
Text Label 4600 1900 0    50   ~ 0
ND4
Text Label 4600 1100 0    50   ~ 0
NA7
Text Label 4600 1400 0    50   ~ 0
NA6
Text Label 4600 1700 0    50   ~ 0
NA5
Text Label 4600 2000 0    50   ~ 0
NA4
Text Label 6000 2000 0    50   ~ 0
CARRY
Text Label 6000 1000 0    50   ~ 0
ND3
Text Label 6000 1100 0    50   ~ 0
NA3
Text Label 6000 1300 0    50   ~ 0
ND2
Text Label 6000 1400 0    50   ~ 0
NA2
Text Label 6000 1600 0    50   ~ 0
ND1
Text Label 6000 1700 0    50   ~ 0
NA1
Text Label 6000 1900 0    50   ~ 0
ND0
Text Label 13950 1050 0    50   ~ 0
RUN
Text Label 13950 1150 0    50   ~ 0
ION
Text Label 13950 1350 0    50   ~ 0
FETCH
Text Label 13950 1450 0    50   ~ 0
DEFER
Text Label 13950 1650 0    50   ~ 0
EXEC
NoConn ~ 13950 1750
NoConn ~ 13950 1950
NoConn ~ 13950 2050
Text Notes 2400 2650 2    50   ~ 0
Read\nDATA/ADDR LSB 8-15
Text Notes 5100 2650 2    50   ~ 0
Read\nDATA/ADDR MSB 0-7
Text Notes 14250 2650 2    50   ~ 0
Read\nSTATUS
$Comp
L Device:LED_ALT D33
U 1 1 5C29C65B
P 5200 6100
F 0 "D33" H 5191 6316 50  0000 C CNN
F 1 "LED_ALT" H 5191 6225 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm_Clear" H 5200 6100 50  0001 C CNN
F 3 "~" H 5200 6100 50  0001 C CNN
	1    5200 6100
	1    0    0    -1  
$EndComp
$Comp
L Device:R R33
U 1 1 5C29C662
P 5500 6100
F 0 "R33" V 5293 6100 50  0000 C CNN
F 1 "4K7" V 5384 6100 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 5430 6100 50  0001 C CNN
F 3 "~" H 5500 6100 50  0001 C CNN
	1    5500 6100
	0    1    1    0   
$EndComp
Text Label 5650 6100 0    50   ~ 0
VCC
Text Label 5050 6100 2    50   ~ 0
ION
$Comp
L Device:LED_ALT D34
U 1 1 5C29C66B
P 5200 6400
F 0 "D34" H 5191 6616 50  0000 C CNN
F 1 "LED_ALT" H 5191 6525 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm_Clear" H 5200 6400 50  0001 C CNN
F 3 "~" H 5200 6400 50  0001 C CNN
	1    5200 6400
	1    0    0    -1  
$EndComp
$Comp
L Device:R R34
U 1 1 5C29C672
P 5500 6400
F 0 "R34" V 5293 6400 50  0000 C CNN
F 1 "4K7" V 5384 6400 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 5430 6400 50  0001 C CNN
F 3 "~" H 5500 6400 50  0001 C CNN
	1    5500 6400
	0    1    1    0   
$EndComp
Text Label 5650 6400 0    50   ~ 0
VCC
Text Label 5050 6400 2    50   ~ 0
RUN
$Comp
L Device:LED_ALT D35
U 1 1 5C29C67B
P 5200 6650
F 0 "D35" H 5191 6866 50  0000 C CNN
F 1 "LED_ALT" H 5191 6775 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm_Clear" H 5200 6650 50  0001 C CNN
F 3 "~" H 5200 6650 50  0001 C CNN
	1    5200 6650
	1    0    0    -1  
$EndComp
$Comp
L Device:R R35
U 1 1 5C29C682
P 5500 6650
F 0 "R35" V 5293 6650 50  0000 C CNN
F 1 "4K7" V 5384 6650 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 5430 6650 50  0001 C CNN
F 3 "~" H 5500 6650 50  0001 C CNN
	1    5500 6650
	0    1    1    0   
$EndComp
Text Label 5650 6650 0    50   ~ 0
VCC
Text Label 5050 6650 2    50   ~ 0
FETCH
$Comp
L Device:LED_ALT D36
U 1 1 5C29C68B
P 5200 6900
F 0 "D36" H 5191 7116 50  0000 C CNN
F 1 "LED_ALT" H 5191 7025 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm_Clear" H 5200 6900 50  0001 C CNN
F 3 "~" H 5200 6900 50  0001 C CNN
	1    5200 6900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R36
U 1 1 5C29C692
P 5500 6900
F 0 "R36" V 5293 6900 50  0000 C CNN
F 1 "4K7" V 5384 6900 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 5430 6900 50  0001 C CNN
F 3 "~" H 5500 6900 50  0001 C CNN
	1    5500 6900
	0    1    1    0   
$EndComp
Text Label 5650 6900 0    50   ~ 0
VCC
Text Label 5050 6900 2    50   ~ 0
DEFER
$Comp
L Device:LED_ALT D37
U 1 1 5C29C69B
P 5200 7150
F 0 "D37" H 5191 7366 50  0000 C CNN
F 1 "LED_ALT" H 5191 7275 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm_Clear" H 5200 7150 50  0001 C CNN
F 3 "~" H 5200 7150 50  0001 C CNN
	1    5200 7150
	1    0    0    -1  
$EndComp
$Comp
L Device:R R37
U 1 1 5C29C6A2
P 5500 7150
F 0 "R37" V 5293 7150 50  0000 C CNN
F 1 "4K7" V 5384 7150 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 5430 7150 50  0001 C CNN
F 3 "~" H 5500 7150 50  0001 C CNN
	1    5500 7150
	0    1    1    0   
$EndComp
Text Label 5650 7150 0    50   ~ 0
VCC
Text Label 5050 7150 2    50   ~ 0
EXEC
NoConn ~ 7650 7450
NoConn ~ 7650 9200
NoConn ~ 13900 6100
NoConn ~ 13900 6000
NoConn ~ 13500 5600
NoConn ~ 13300 5600
NoConn ~ 12900 6000
NoConn ~ 12900 6100
$Comp
L Connector_Generic:Conn_02x05_Top_Bottom J2
U 1 1 5C2D46A1
P 15150 5850
F 0 "J2" H 15200 6267 50  0000 C CNN
F 1 "Conn_02x05_Top_Bottom" H 15200 6176 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x05_P2.54mm_Horizontal" H 15150 5850 50  0001 C CNN
F 3 "~" H 15150 5850 50  0001 C CNN
	1    15150 5850
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x05_Top_Bottom J3
U 1 1 5C2D4787
P 15100 6950
F 0 "J3" H 15150 7367 50  0000 C CNN
F 1 "Conn_02x05_Top_Bottom" H 15150 7276 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x05_P2.54mm_Horizontal" H 15100 6950 50  0001 C CNN
F 3 "~" H 15100 6950 50  0001 C CNN
	1    15100 6950
	1    0    0    -1  
$EndComp
Text Label 14950 5650 2    50   ~ 0
GND
Text Label 14900 6750 2    50   ~ 0
GND
Text Label 14950 5750 2    50   ~ 0
A0
Text Label 14900 6850 2    50   ~ 0
A0
Text Label 14950 5850 2    50   ~ 0
LCD4
Text Label 14950 5950 2    50   ~ 0
LCD6
Text Label 14950 6050 2    50   ~ 0
KEYP
Text Label 14900 6950 2    50   ~ 0
LCD4
Text Label 14900 7050 2    50   ~ 0
LCD6
Text Label 14900 7150 2    50   ~ 0
KEYP
Text Label 15450 5650 0    50   ~ 0
VCC
Text Label 15400 6750 0    50   ~ 0
VCC
Text Label 15450 5750 0    50   ~ 0
ENLCD1
Text Label 15450 5850 0    50   ~ 0
LCD5
Text Label 15450 5950 0    50   ~ 0
LCD7
Text Label 15450 6050 0    50   ~ 0
ENLCD2
Text Label 15400 6850 0    50   ~ 0
ENLCD1
Text Label 15400 6950 0    50   ~ 0
LCD5
Text Label 15400 7050 0    50   ~ 0
LCD7
Text Label 15400 7150 0    50   ~ 0
ENLCD2
Text Label 1300 2600 0    50   ~ 0
GND
Text Label 2650 2600 0    50   ~ 0
GND
Text Label 4100 2600 0    50   ~ 0
GND
Text Label 5500 2600 0    50   ~ 0
GND
Text Label 13450 2650 0    50   ~ 0
GND
Text Label 14800 4750 0    50   ~ 0
GND
Text Label 13250 4750 0    50   ~ 0
GND
Text Label 11200 5000 0    50   ~ 0
GND
Text Label 9800 5000 0    50   ~ 0
GND
Text Label 8400 5000 0    50   ~ 0
GND
Text Label 7000 5000 0    50   ~ 0
GND
Text Label 5500 5000 0    50   ~ 0
GND
Text Label 4100 5000 0    50   ~ 0
GND
Text Label 2700 5000 0    50   ~ 0
GND
Text Label 1350 5000 0    50   ~ 0
GND
Text Label 8150 7750 0    50   ~ 0
GND
Text Label 8150 9500 0    50   ~ 0
GND
Text Label 1300 700  0    50   ~ 0
VCC
Text Label 2650 700  0    50   ~ 0
VCC
Text Label 4100 700  0    50   ~ 0
VCC
Text Label 5500 700  0    50   ~ 0
VCC
Text Label 1350 3200 0    50   ~ 0
VCC
Text Label 2700 3200 0    50   ~ 0
VCC
Text Label 4100 3200 0    50   ~ 0
VCC
Text Label 5500 3200 0    50   ~ 0
VCC
Text Label 7000 3200 0    50   ~ 0
VCC
Text Label 8400 3200 0    50   ~ 0
VCC
Text Label 9800 3200 0    50   ~ 0
VCC
Text Label 11200 3200 0    50   ~ 0
VCC
Text Label 13450 750  0    50   ~ 0
VCC
Text Label 13250 2950 0    50   ~ 0
VCC
Text Label 14800 2950 0    50   ~ 0
VCC
Text Label 8150 6250 0    50   ~ 0
VCC
Text Label 8150 8000 0    50   ~ 0
VCC
Text Label 1300 2600 0    50   ~ 0
GND
Text Label 2700 5000 0    50   ~ 0
GND
NoConn ~ 13900 6400
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 5C2FB205
P 1050 5700
F 0 "#FLG0101" H 1050 5775 50  0001 C CNN
F 1 "PWR_FLAG" H 1050 5874 50  0000 C CNN
F 2 "" H 1050 5700 50  0001 C CNN
F 3 "~" H 1050 5700 50  0001 C CNN
	1    1050 5700
	1    0    0    -1  
$EndComp
Text Label 1050 5700 0    50   ~ 0
GND
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 5C2FB478
P 1550 5700
F 0 "#FLG0102" H 1550 5775 50  0001 C CNN
F 1 "PWR_FLAG" H 1550 5874 50  0000 C CNN
F 2 "" H 1550 5700 50  0001 C CNN
F 3 "~" H 1550 5700 50  0001 C CNN
	1    1550 5700
	1    0    0    -1  
$EndComp
Text Label 1550 5700 0    50   ~ 0
VCC
Text Label 14100 5600 0    50   ~ 0
VCC
$Comp
L Connector_Generic:Conn_02x01 J4
U 1 1 5C300834
P 13800 5600
F 0 "J4" H 13850 5817 50  0000 C CNN
F 1 "Conn_02x01" H 13850 5726 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 13800 5600 50  0001 C CNN
F 3 "~" H 13800 5600 50  0001 C CNN
	1    13800 5600
	1    0    0    -1  
$EndComp
Text Notes 12750 5350 0    50   ~ 0
Remove jumper when Arduino connected to PC
Text Notes 15300 6450 2    50   ~ 0
Keypad
$Comp
L Switch:SW_SPST SW1
U 1 1 5C2B92B6
P 6300 6100
F 0 "SW1" H 6300 6335 50  0000 C CNN
F 1 "SW_SPST" H 6300 6244 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 6300 6100 50  0001 C CNN
F 3 "" H 6300 6100 50  0001 C CNN
	1    6300 6100
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPST SW2
U 1 1 5C2B9477
P 6300 6400
F 0 "SW2" H 6300 6635 50  0000 C CNN
F 1 "SW_SPST" H 6300 6544 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 6300 6400 50  0001 C CNN
F 3 "" H 6300 6400 50  0001 C CNN
	1    6300 6400
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPST SW3
U 1 1 5C2B958A
P 6300 6700
F 0 "SW3" H 6300 6935 50  0000 C CNN
F 1 "SW_SPST" H 6300 6844 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 6300 6700 50  0001 C CNN
F 3 "" H 6300 6700 50  0001 C CNN
	1    6300 6700
	1    0    0    -1  
$EndComp
Text Label 6100 6100 2    50   ~ 0
ND15
Text Label 6100 6400 2    50   ~ 0
ND14
Text Label 6100 6700 2    50   ~ 0
ND13
$Comp
L 74xx:74LS06 U19
U 2 1 5C2BB290
P 6800 6400
F 0 "U19" H 6800 6083 50  0000 C CNN
F 1 "74LS06" H 6800 6174 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W10.16mm" H 6800 6400 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS06" H 6800 6400 50  0001 C CNN
	2    6800 6400
	-1   0    0    1   
$EndComp
$Comp
L 74xx:74LS06 U19
U 3 1 5C2BB38B
P 6800 6700
F 0 "U19" H 6800 6383 50  0000 C CNN
F 1 "74LS06" H 6800 6474 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W10.16mm" H 6800 6700 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS06" H 6800 6700 50  0001 C CNN
	3    6800 6700
	-1   0    0    1   
$EndComp
$Comp
L Switch:SW_SPST SW4
U 1 1 5C2BB9ED
P 6300 7000
F 0 "SW4" H 6300 7235 50  0000 C CNN
F 1 "SW_SPST" H 6300 7144 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 6300 7000 50  0001 C CNN
F 3 "" H 6300 7000 50  0001 C CNN
	1    6300 7000
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPST SW5
U 1 1 5C2BB9F4
P 6300 7300
F 0 "SW5" H 6300 7535 50  0000 C CNN
F 1 "SW_SPST" H 6300 7444 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 6300 7300 50  0001 C CNN
F 3 "" H 6300 7300 50  0001 C CNN
	1    6300 7300
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPST SW6
U 1 1 5C2BB9FB
P 6300 7600
F 0 "SW6" H 6300 7835 50  0000 C CNN
F 1 "SW_SPST" H 6300 7744 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 6300 7600 50  0001 C CNN
F 3 "" H 6300 7600 50  0001 C CNN
	1    6300 7600
	1    0    0    -1  
$EndComp
Text Label 6100 7000 2    50   ~ 0
ND12
Text Label 6100 7300 2    50   ~ 0
ND11
Text Label 6100 7600 2    50   ~ 0
ND10
$Comp
L 74xx:74LS06 U19
U 4 1 5C2BBA05
P 6800 7000
F 0 "U19" H 6800 6683 50  0000 C CNN
F 1 "74LS06" H 6800 6774 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W10.16mm" H 6800 7000 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS06" H 6800 7000 50  0001 C CNN
	4    6800 7000
	-1   0    0    1   
$EndComp
$Comp
L 74xx:74LS06 U19
U 5 1 5C2BBA0C
P 6800 7300
F 0 "U19" H 6800 6983 50  0000 C CNN
F 1 "74LS06" H 6800 7074 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W10.16mm" H 6800 7300 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS06" H 6800 7300 50  0001 C CNN
	5    6800 7300
	-1   0    0    1   
$EndComp
$Comp
L 74xx:74LS06 U19
U 6 1 5C2BBA13
P 6800 7600
F 0 "U19" H 6800 7283 50  0000 C CNN
F 1 "74LS06" H 6800 7374 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W10.16mm" H 6800 7600 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS06" H 6800 7600 50  0001 C CNN
	6    6800 7600
	-1   0    0    1   
$EndComp
$Comp
L Switch:SW_SPST SW7
U 1 1 5C2BEB6B
P 6300 7900
F 0 "SW7" H 6300 8135 50  0000 C CNN
F 1 "SW_SPST" H 6300 8044 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 6300 7900 50  0001 C CNN
F 3 "" H 6300 7900 50  0001 C CNN
	1    6300 7900
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPST SW8
U 1 1 5C2BEB72
P 6300 8200
F 0 "SW8" H 6300 8435 50  0000 C CNN
F 1 "SW_SPST" H 6300 8344 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 6300 8200 50  0001 C CNN
F 3 "" H 6300 8200 50  0001 C CNN
	1    6300 8200
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPST SW9
U 1 1 5C2BEB79
P 6300 8500
F 0 "SW9" H 6300 8735 50  0000 C CNN
F 1 "SW_SPST" H 6300 8644 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 6300 8500 50  0001 C CNN
F 3 "" H 6300 8500 50  0001 C CNN
	1    6300 8500
	1    0    0    -1  
$EndComp
Text Label 6100 7900 2    50   ~ 0
ND9
Text Label 6100 8200 2    50   ~ 0
ND8
Text Label 6100 8500 2    50   ~ 0
ND7
$Comp
L 74xx:74LS06 U20
U 1 1 5C2BEB83
P 6800 7900
F 0 "U20" H 6800 7583 50  0000 C CNN
F 1 "74LS06" H 6800 7674 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W10.16mm" H 6800 7900 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS06" H 6800 7900 50  0001 C CNN
	1    6800 7900
	-1   0    0    1   
$EndComp
$Comp
L 74xx:74LS06 U20
U 2 1 5C2BEB8A
P 6800 8200
F 0 "U20" H 6800 7883 50  0000 C CNN
F 1 "74LS06" H 6800 7974 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W10.16mm" H 6800 8200 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS06" H 6800 8200 50  0001 C CNN
	2    6800 8200
	-1   0    0    1   
$EndComp
$Comp
L 74xx:74LS06 U20
U 3 1 5C2BEB91
P 6800 8500
F 0 "U20" H 6800 8183 50  0000 C CNN
F 1 "74LS06" H 6800 8274 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W10.16mm" H 6800 8500 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS06" H 6800 8500 50  0001 C CNN
	3    6800 8500
	-1   0    0    1   
$EndComp
$Comp
L Switch:SW_SPST SW10
U 1 1 5C2BEB98
P 6300 8800
F 0 "SW10" H 6300 9035 50  0000 C CNN
F 1 "SW_SPST" H 6300 8944 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 6300 8800 50  0001 C CNN
F 3 "" H 6300 8800 50  0001 C CNN
	1    6300 8800
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPST SW11
U 1 1 5C2BEB9F
P 6300 9100
F 0 "SW11" H 6300 9335 50  0000 C CNN
F 1 "SW_SPST" H 6300 9244 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 6300 9100 50  0001 C CNN
F 3 "" H 6300 9100 50  0001 C CNN
	1    6300 9100
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPST SW12
U 1 1 5C2BEBA6
P 6300 9400
F 0 "SW12" H 6300 9635 50  0000 C CNN
F 1 "SW_SPST" H 6300 9544 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 6300 9400 50  0001 C CNN
F 3 "" H 6300 9400 50  0001 C CNN
	1    6300 9400
	1    0    0    -1  
$EndComp
Text Label 6100 8800 2    50   ~ 0
ND6
Text Label 6100 9100 2    50   ~ 0
ND5
Text Label 6100 9400 2    50   ~ 0
ND4
$Comp
L 74xx:74LS06 U20
U 4 1 5C2BEBB0
P 6800 8800
F 0 "U20" H 6800 8483 50  0000 C CNN
F 1 "74LS06" H 6800 8574 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W10.16mm" H 6800 8800 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS06" H 6800 8800 50  0001 C CNN
	4    6800 8800
	-1   0    0    1   
$EndComp
$Comp
L 74xx:74LS06 U20
U 5 1 5C2BEBB7
P 6800 9100
F 0 "U20" H 6800 8783 50  0000 C CNN
F 1 "74LS06" H 6800 8874 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W10.16mm" H 6800 9100 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS06" H 6800 9100 50  0001 C CNN
	5    6800 9100
	-1   0    0    1   
$EndComp
$Comp
L 74xx:74LS06 U20
U 6 1 5C2BEBBE
P 6800 9400
F 0 "U20" H 6800 9083 50  0000 C CNN
F 1 "74LS06" H 6800 9174 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W10.16mm" H 6800 9400 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS06" H 6800 9400 50  0001 C CNN
	6    6800 9400
	-1   0    0    1   
$EndComp
$Comp
L Switch:SW_SPST SW13
U 1 1 5C2BF9E2
P 6300 9700
F 0 "SW13" H 6300 9935 50  0000 C CNN
F 1 "SW_SPST" H 6300 9844 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 6300 9700 50  0001 C CNN
F 3 "" H 6300 9700 50  0001 C CNN
	1    6300 9700
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPST SW14
U 1 1 5C2BF9E9
P 6300 10000
F 0 "SW14" H 6300 10235 50  0000 C CNN
F 1 "SW_SPST" H 6300 10144 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 6300 10000 50  0001 C CNN
F 3 "" H 6300 10000 50  0001 C CNN
	1    6300 10000
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPST SW15
U 1 1 5C2BF9F0
P 6300 10300
F 0 "SW15" H 6300 10535 50  0000 C CNN
F 1 "SW_SPST" H 6300 10444 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 6300 10300 50  0001 C CNN
F 3 "" H 6300 10300 50  0001 C CNN
	1    6300 10300
	1    0    0    -1  
$EndComp
Text Label 6100 9700 2    50   ~ 0
ND3
Text Label 6100 10000 2    50   ~ 0
ND2
Text Label 6100 10300 2    50   ~ 0
ND1
$Comp
L 74xx:74LS06 U21
U 1 1 5C2BF9FA
P 6800 9700
F 0 "U21" H 6800 9383 50  0000 C CNN
F 1 "74LS06" H 6800 9474 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W10.16mm" H 6800 9700 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS06" H 6800 9700 50  0001 C CNN
	1    6800 9700
	-1   0    0    1   
$EndComp
$Comp
L 74xx:74LS06 U21
U 2 1 5C2BFA01
P 6800 10000
F 0 "U21" H 6800 9683 50  0000 C CNN
F 1 "74LS06" H 6800 9774 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W10.16mm" H 6800 10000 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS06" H 6800 10000 50  0001 C CNN
	2    6800 10000
	-1   0    0    1   
$EndComp
$Comp
L 74xx:74LS06 U21
U 3 1 5C2BFA08
P 6800 10300
F 0 "U21" H 6800 9983 50  0000 C CNN
F 1 "74LS06" H 6800 10074 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W10.16mm" H 6800 10300 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS06" H 6800 10300 50  0001 C CNN
	3    6800 10300
	-1   0    0    1   
$EndComp
$Comp
L Switch:SW_SPST SW16
U 1 1 5C2BFA0F
P 6300 10600
F 0 "SW16" H 6300 10835 50  0000 C CNN
F 1 "SW_SPST" H 6300 10744 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 6300 10600 50  0001 C CNN
F 3 "" H 6300 10600 50  0001 C CNN
	1    6300 10600
	1    0    0    -1  
$EndComp
Text Label 6100 10600 2    50   ~ 0
ND0
$Comp
L 74xx:74LS06 U21
U 4 1 5C2BFA27
P 6800 10600
F 0 "U21" H 6800 10283 50  0000 C CNN
F 1 "74LS06" H 6800 10374 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W10.16mm" H 6800 10600 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS06" H 6800 10600 50  0001 C CNN
	4    6800 10600
	-1   0    0    1   
$EndComp
Text Label 13750 3550 0    50   ~ 0
ENSWREG
Wire Wire Line
	7100 10600 7100 10300
Connection ~ 7100 6700
Wire Wire Line
	7100 6700 7100 6400
Connection ~ 7100 7000
Wire Wire Line
	7100 7000 7100 6700
Connection ~ 7100 7300
Wire Wire Line
	7100 7300 7100 7000
Connection ~ 7100 7600
Wire Wire Line
	7100 7600 7100 7300
Connection ~ 7100 7900
Wire Wire Line
	7100 7900 7100 7600
Connection ~ 7100 8200
Wire Wire Line
	7100 8200 7100 7900
Connection ~ 7100 8500
Wire Wire Line
	7100 8500 7100 8200
Connection ~ 7100 8800
Wire Wire Line
	7100 8800 7100 8500
Connection ~ 7100 9100
Wire Wire Line
	7100 9100 7100 8800
Connection ~ 7100 9400
Wire Wire Line
	7100 9400 7100 9100
Connection ~ 7100 9700
Wire Wire Line
	7100 9700 7100 9400
Connection ~ 7100 10000
Wire Wire Line
	7100 10000 7100 9700
Connection ~ 7100 10300
Wire Wire Line
	7100 10300 7100 10000
$Comp
L Switch:SW_SPDT_MSM SW17
U 1 1 5C2F090E
P 9550 7750
F 0 "SW17" H 9550 8035 50  0000 C CNN
F 1 "SW_SPDT_MSM" H 9550 7944 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 9550 7750 50  0001 C CNN
F 3 "" H 9550 7750 50  0001 C CNN
	1    9550 7750
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPDT_MSM SW18
U 1 1 5C2F0F5C
P 10100 7750
F 0 "SW18" H 10100 8035 50  0000 C CNN
F 1 "SW_SPDT_MSM" H 10100 7944 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 10100 7750 50  0001 C CNN
F 3 "" H 10100 7750 50  0001 C CNN
	1    10100 7750
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPDT_MSM SW19
U 1 1 5C2F1527
P 10600 7750
F 0 "SW19" H 10600 8035 50  0000 C CNN
F 1 "SW_SPDT_MSM" H 10600 7944 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 10600 7750 50  0001 C CNN
F 3 "" H 10600 7750 50  0001 C CNN
	1    10600 7750
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPDT_MSM SW20
U 1 1 5C2F152E
P 11150 7750
F 0 "SW20" H 11150 8035 50  0000 C CNN
F 1 "SW_SPDT_MSM" H 11150 7944 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 11150 7750 50  0001 C CNN
F 3 "" H 11150 7750 50  0001 C CNN
	1    11150 7750
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPDT_MSM SW21
U 1 1 5C2F2039
P 11700 7750
F 0 "SW21" H 11700 8035 50  0000 C CNN
F 1 "SW_SPDT_MSM" H 11700 7944 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 11700 7750 50  0001 C CNN
F 3 "" H 11700 7750 50  0001 C CNN
	1    11700 7750
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R40
U 1 1 5C2F246C
P 9950 8000
F 0 "R40" V 9754 8000 50  0000 C CNN
F 1 "1K" V 9845 8000 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" H 9950 8000 50  0001 C CNN
F 3 "~" H 9950 8000 50  0001 C CNN
	1    9950 8000
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R43
U 1 1 5C2F265F
P 10500 8000
F 0 "R43" V 10304 8000 50  0000 C CNN
F 1 "1K" V 10395 8000 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" H 10500 8000 50  0001 C CNN
F 3 "~" H 10500 8000 50  0001 C CNN
	1    10500 8000
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R46
U 1 1 5C2F28B3
P 11000 8000
F 0 "R46" V 10804 8000 50  0000 C CNN
F 1 "1K" V 10895 8000 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" H 11000 8000 50  0001 C CNN
F 3 "~" H 11000 8000 50  0001 C CNN
	1    11000 8000
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R48
U 1 1 5C2F29EC
P 11600 8000
F 0 "R48" V 11404 8000 50  0000 C CNN
F 1 "1K" V 11495 8000 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" H 11600 8000 50  0001 C CNN
F 3 "~" H 11600 8000 50  0001 C CNN
	1    11600 8000
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R39
U 1 1 5C2F2D68
P 9950 7650
F 0 "R39" V 9754 7650 50  0000 C CNN
F 1 "1K" V 9845 7650 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" H 9950 7650 50  0001 C CNN
F 3 "~" H 9950 7650 50  0001 C CNN
	1    9950 7650
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R41
U 1 1 5C2F2D6F
P 10450 7650
F 0 "R41" V 10254 7650 50  0000 C CNN
F 1 "1K" V 10345 7650 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" H 10450 7650 50  0001 C CNN
F 3 "~" H 10450 7650 50  0001 C CNN
	1    10450 7650
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R45
U 1 1 5C2F2D7D
P 11000 7650
F 0 "R45" V 10804 7650 50  0000 C CNN
F 1 "1K" V 10895 7650 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" H 11000 7650 50  0001 C CNN
F 3 "~" H 11000 7650 50  0001 C CNN
	1    11000 7650
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R47
U 1 1 5C2F2D84
P 11600 7650
F 0 "R47" V 11404 7650 50  0000 C CNN
F 1 "1K" V 11495 7650 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" H 11600 7650 50  0001 C CNN
F 3 "~" H 11600 7650 50  0001 C CNN
	1    11600 7650
	0    1    1    0   
$EndComp
Wire Wire Line
	11900 7850 11900 8000
Wire Wire Line
	11900 8000 11700 8000
Wire Wire Line
	11500 8000 11350 8000
Wire Wire Line
	11350 7850 11350 8000
Connection ~ 11350 8000
Wire Wire Line
	11350 8000 11100 8000
Wire Wire Line
	10900 8000 10800 8000
Wire Wire Line
	10800 7850 10800 8000
Connection ~ 10800 8000
Wire Wire Line
	10800 8000 10600 8000
Wire Wire Line
	10400 8000 10300 8000
$Comp
L Device:R_Small R32
U 1 1 5C2F8132
P 9400 8000
F 0 "R32" V 9204 8000 50  0000 C CNN
F 1 "1K" V 9295 8000 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" H 9400 8000 50  0001 C CNN
F 3 "~" H 9400 8000 50  0001 C CNN
	1    9400 8000
	0    1    1    0   
$EndComp
Wire Wire Line
	9850 8000 9750 8000
Wire Wire Line
	9300 8000 9150 8000
Wire Wire Line
	9150 8000 9150 7650
Wire Wire Line
	9150 7650 9750 7650
Wire Wire Line
	9850 7650 9750 7650
Connection ~ 9750 7650
Wire Wire Line
	10050 7650 10300 7650
Wire Wire Line
	10300 7650 10350 7650
Connection ~ 10300 7650
Wire Wire Line
	10550 7650 10800 7650
Wire Wire Line
	10800 7650 10900 7650
Connection ~ 10800 7650
Wire Wire Line
	11100 7650 11350 7650
Wire Wire Line
	10300 7850 10300 8000
Connection ~ 10300 8000
Wire Wire Line
	10300 8000 10050 8000
Wire Wire Line
	9750 7850 9750 8000
Connection ~ 9750 8000
Wire Wire Line
	9750 8000 9500 8000
Wire Wire Line
	11700 7650 11900 7650
Wire Wire Line
	11500 7650 11350 7650
Connection ~ 11350 7650
Wire Wire Line
	12000 8000 11900 8000
Connection ~ 11900 8000
$Comp
L Device:R_Small R50
U 1 1 5C3056ED
P 12100 8000
F 0 "R50" V 11904 8000 50  0000 C CNN
F 1 "1K" V 11995 8000 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" H 12100 8000 50  0001 C CNN
F 3 "~" H 12100 8000 50  0001 C CNN
	1    12100 8000
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R49
U 1 1 5C3060B2
P 12100 7650
F 0 "R49" V 11904 7650 50  0000 C CNN
F 1 "1K" V 11995 7650 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" H 12100 7650 50  0001 C CNN
F 3 "~" H 12100 7650 50  0001 C CNN
	1    12100 7650
	0    1    1    0   
$EndComp
Wire Wire Line
	11900 7650 12000 7650
Connection ~ 11900 7650
Text Label 12200 8000 0    50   ~ 0
GND
Text Label 12200 7650 0    50   ~ 0
AREF
$Comp
L Device:R_Small R51
U 1 1 5C309420
P 12100 8150
F 0 "R51" V 11904 8150 50  0000 C CNN
F 1 "1M" V 11995 8150 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" H 12100 8150 50  0001 C CNN
F 3 "~" H 12100 8150 50  0001 C CNN
	1    12100 8150
	0    1    1    0   
$EndComp
Text Label 12200 8150 0    50   ~ 0
GND
Wire Wire Line
	12000 8150 11600 8150
Wire Wire Line
	9250 8150 9250 7750
Wire Wire Line
	9250 7750 9350 7750
Wire Wire Line
	9900 7750 9800 7750
Wire Wire Line
	9800 7750 9800 8150
Connection ~ 9800 8150
Wire Wire Line
	9800 8150 9250 8150
Wire Wire Line
	10400 7750 10350 7750
Wire Wire Line
	10350 7750 10350 8150
Connection ~ 10350 8150
Wire Wire Line
	10350 8150 9800 8150
Wire Wire Line
	11500 7750 11400 7750
Wire Wire Line
	11400 7750 11400 8150
Connection ~ 11400 8150
Wire Wire Line
	11400 8150 10850 8150
Text Label 11800 8350 0    50   ~ 0
KEYL
Wire Wire Line
	11600 8350 11600 8150
Connection ~ 11600 8150
Wire Wire Line
	11600 8150 11400 8150
$Comp
L Switch:SW_SPDT_MSM SW22
U 1 1 5C31186B
P 9550 8800
F 0 "SW22" H 9550 9085 50  0000 C CNN
F 1 "SW_SPDT_MSM" H 9550 8994 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 9550 8800 50  0001 C CNN
F 3 "" H 9550 8800 50  0001 C CNN
	1    9550 8800
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPDT_MSM SW23
U 1 1 5C311872
P 10100 8800
F 0 "SW23" H 10100 9085 50  0000 C CNN
F 1 "SW_SPDT_MSM" H 10100 8994 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 10100 8800 50  0001 C CNN
F 3 "" H 10100 8800 50  0001 C CNN
	1    10100 8800
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPDT_MSM SW24
U 1 1 5C311879
P 10600 8800
F 0 "SW24" H 10600 9085 50  0000 C CNN
F 1 "SW_SPDT_MSM" H 10600 8994 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 10600 8800 50  0001 C CNN
F 3 "" H 10600 8800 50  0001 C CNN
	1    10600 8800
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPDT_MSM SW25
U 1 1 5C311880
P 11150 8800
F 0 "SW25" H 11150 9085 50  0000 C CNN
F 1 "SW_SPDT_MSM" H 11150 8994 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 11150 8800 50  0001 C CNN
F 3 "" H 11150 8800 50  0001 C CNN
	1    11150 8800
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPDT_MSM SW26
U 1 1 5C311887
P 11700 8800
F 0 "SW26" H 11700 9085 50  0000 C CNN
F 1 "SW_SPDT_MSM" H 11700 8994 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 11700 8800 50  0001 C CNN
F 3 "" H 11700 8800 50  0001 C CNN
	1    11700 8800
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R54
U 1 1 5C31188E
P 9950 9050
F 0 "R54" V 9754 9050 50  0000 C CNN
F 1 "1K" V 9845 9050 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" H 9950 9050 50  0001 C CNN
F 3 "~" H 9950 9050 50  0001 C CNN
	1    9950 9050
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R57
U 1 1 5C311895
P 10500 9050
F 0 "R57" V 10304 9050 50  0000 C CNN
F 1 "1K" V 10395 9050 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" H 10500 9050 50  0001 C CNN
F 3 "~" H 10500 9050 50  0001 C CNN
	1    10500 9050
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R60
U 1 1 5C3118A3
P 11000 9050
F 0 "R60" V 10804 9050 50  0000 C CNN
F 1 "1K" V 10895 9050 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" H 11000 9050 50  0001 C CNN
F 3 "~" H 11000 9050 50  0001 C CNN
	1    11000 9050
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R62
U 1 1 5C3118AA
P 11600 9050
F 0 "R62" V 11404 9050 50  0000 C CNN
F 1 "1K" V 11495 9050 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" H 11600 9050 50  0001 C CNN
F 3 "~" H 11600 9050 50  0001 C CNN
	1    11600 9050
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R53
U 1 1 5C3118B1
P 9950 8700
F 0 "R53" V 9754 8700 50  0000 C CNN
F 1 "1K" V 9845 8700 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" H 9950 8700 50  0001 C CNN
F 3 "~" H 9950 8700 50  0001 C CNN
	1    9950 8700
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R55
U 1 1 5C3118B8
P 10450 8700
F 0 "R55" V 10254 8700 50  0000 C CNN
F 1 "1K" V 10345 8700 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" H 10450 8700 50  0001 C CNN
F 3 "~" H 10450 8700 50  0001 C CNN
	1    10450 8700
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R59
U 1 1 5C3118C6
P 11000 8700
F 0 "R59" V 10804 8700 50  0000 C CNN
F 1 "1K" V 10895 8700 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" H 11000 8700 50  0001 C CNN
F 3 "~" H 11000 8700 50  0001 C CNN
	1    11000 8700
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R61
U 1 1 5C3118CD
P 11600 8700
F 0 "R61" V 11404 8700 50  0000 C CNN
F 1 "1K" V 11495 8700 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" H 11600 8700 50  0001 C CNN
F 3 "~" H 11600 8700 50  0001 C CNN
	1    11600 8700
	0    1    1    0   
$EndComp
Wire Wire Line
	11900 8900 11900 9050
Wire Wire Line
	11900 9050 11700 9050
Wire Wire Line
	11500 9050 11350 9050
Wire Wire Line
	11350 8900 11350 9050
Connection ~ 11350 9050
Wire Wire Line
	11350 9050 11100 9050
Wire Wire Line
	10900 9050 10800 9050
Wire Wire Line
	10800 8900 10800 9050
Connection ~ 10800 9050
Wire Wire Line
	10800 9050 10600 9050
Wire Wire Line
	10400 9050 10300 9050
$Comp
L Device:R_Small R52
U 1 1 5C3118E1
P 9400 9050
F 0 "R52" V 9204 9050 50  0000 C CNN
F 1 "1K" V 9295 9050 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" H 9400 9050 50  0001 C CNN
F 3 "~" H 9400 9050 50  0001 C CNN
	1    9400 9050
	0    1    1    0   
$EndComp
Wire Wire Line
	9850 9050 9750 9050
Wire Wire Line
	9300 9050 9150 9050
Wire Wire Line
	9150 9050 9150 8700
Wire Wire Line
	9150 8700 9750 8700
Wire Wire Line
	9850 8700 9750 8700
Connection ~ 9750 8700
Wire Wire Line
	10050 8700 10300 8700
Wire Wire Line
	10300 8700 10350 8700
Connection ~ 10300 8700
Wire Wire Line
	10550 8700 10800 8700
Wire Wire Line
	10800 8700 10900 8700
Connection ~ 10800 8700
Wire Wire Line
	11100 8700 11350 8700
Wire Wire Line
	10300 8900 10300 9050
Connection ~ 10300 9050
Wire Wire Line
	10300 9050 10050 9050
Wire Wire Line
	9750 8900 9750 9050
Connection ~ 9750 9050
Wire Wire Line
	9750 9050 9500 9050
Wire Wire Line
	11700 8700 11900 8700
Wire Wire Line
	11500 8700 11350 8700
Connection ~ 11350 8700
Wire Wire Line
	12000 9050 11900 9050
Connection ~ 11900 9050
$Comp
L Device:R_Small R64
U 1 1 5C311902
P 12100 9050
F 0 "R64" V 11904 9050 50  0000 C CNN
F 1 "1K" V 11995 9050 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" H 12100 9050 50  0001 C CNN
F 3 "~" H 12100 9050 50  0001 C CNN
	1    12100 9050
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R63
U 1 1 5C311909
P 12100 8700
F 0 "R63" V 11904 8700 50  0000 C CNN
F 1 "1K" V 11995 8700 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" H 12100 8700 50  0001 C CNN
F 3 "~" H 12100 8700 50  0001 C CNN
	1    12100 8700
	0    1    1    0   
$EndComp
Wire Wire Line
	11900 8700 12000 8700
Connection ~ 11900 8700
Text Label 12200 9050 0    50   ~ 0
GND
Text Label 12200 8700 0    50   ~ 0
AREF
$Comp
L Device:R_Small R65
U 1 1 5C311914
P 12100 9200
F 0 "R65" V 11904 9200 50  0000 C CNN
F 1 "1M" V 11995 9200 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" H 12100 9200 50  0001 C CNN
F 3 "~" H 12100 9200 50  0001 C CNN
	1    12100 9200
	0    1    1    0   
$EndComp
Text Label 12200 9200 0    50   ~ 0
GND
Wire Wire Line
	12000 9200 11600 9200
Wire Wire Line
	9250 9200 9250 8800
Wire Wire Line
	9250 8800 9350 8800
Wire Wire Line
	9900 8800 9800 8800
Wire Wire Line
	9800 8800 9800 9200
Connection ~ 9800 9200
Wire Wire Line
	9800 9200 9250 9200
Wire Wire Line
	10400 8800 10350 8800
Wire Wire Line
	10350 8800 10350 9200
Connection ~ 10350 9200
Wire Wire Line
	10350 9200 9800 9200
Wire Wire Line
	11500 8800 11400 8800
Wire Wire Line
	11400 8800 11400 9200
Connection ~ 11400 9200
Wire Wire Line
	11400 9200 10850 9200
Text Label 11800 9400 0    50   ~ 0
KEYM
Wire Wire Line
	11600 9400 11600 9200
Connection ~ 11600 9200
Wire Wire Line
	11600 9200 11400 9200
NoConn ~ 9650 7750
NoConn ~ 10200 7750
NoConn ~ 10700 7750
NoConn ~ 11250 7750
NoConn ~ 11800 7750
NoConn ~ 11800 8800
NoConn ~ 11250 8800
NoConn ~ 10700 8800
NoConn ~ 10200 8800
NoConn ~ 9650 8800
Connection ~ 7100 6400
Wire Wire Line
	10150 6500 10150 6250
Wire Wire Line
	7100 6400 7100 6100
$Comp
L 74xx:74LS06 U19
U 1 1 5C2BB08C
P 6800 6100
F 0 "U19" H 6800 5783 50  0000 C CNN
F 1 "74LS06" H 6800 5874 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W10.16mm" H 6800 6100 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS06" H 6800 6100 50  0001 C CNN
	1    6800 6100
	-1   0    0    1   
$EndComp
Wire Wire Line
	10950 7750 10850 7750
Wire Wire Line
	10850 7750 10850 8150
Connection ~ 10850 8150
Wire Wire Line
	10850 8150 10350 8150
Wire Wire Line
	10850 8800 10850 9200
Wire Wire Line
	10850 8800 10950 8800
Connection ~ 10950 8800
Wire Wire Line
	10950 8800 11000 8800
Connection ~ 10850 9200
Wire Wire Line
	10850 9200 10350 9200
$Comp
L 74xx:74LS02 U18
U 3 1 5C374369
P 10150 5950
F 0 "U18" V 10104 6138 50  0000 L CNN
F 1 "74LS02" V 10195 6138 50  0000 L CNN
F 2 "Package_DIP:DIP-14_W10.16mm" H 10150 5950 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74ls02" H 10150 5950 50  0001 C CNN
	3    10150 5950
	0    1    1    0   
$EndComp
$Comp
L 74xx:74LS02 U18
U 2 1 5C374587
P 10550 5500
F 0 "U18" V 10504 5688 50  0000 L CNN
F 1 "74LS02" V 10595 5688 50  0000 L CNN
F 2 "Package_DIP:DIP-14_W10.16mm" H 10550 5500 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74ls02" H 10550 5500 50  0001 C CNN
	2    10550 5500
	0    1    1    0   
$EndComp
$Comp
L 74xx:74LS02 U18
U 4 1 5C37490D
P 10550 6100
F 0 "U18" V 10504 6288 50  0000 L CNN
F 1 "74LS02" V 10595 6288 50  0000 L CNN
F 2 "Package_DIP:DIP-14_W10.16mm" H 10550 6100 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74ls02" H 10550 6100 50  0001 C CNN
	4    10550 6100
	0    1    1    0   
$EndComp
Connection ~ 10550 5800
Wire Wire Line
	10550 5800 10650 5800
Wire Wire Line
	10450 5800 10550 5800
$Comp
L 74xx:74LS02 U18
U 1 1 5C386413
P 9750 5550
F 0 "U18" H 9750 5875 50  0000 C CNN
F 1 "74LS02" H 9750 5784 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W10.16mm" H 9750 5550 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74ls02" H 9750 5550 50  0001 C CNN
	1    9750 5550
	1    0    0    -1  
$EndComp
Text Label 10650 5200 0    50   ~ 0
CONTDATA
Text Label 10250 5650 0    50   ~ 0
CONTDATA
Text Label 10450 5200 2    50   ~ 0
ENSWREG
$Comp
L 74xx:74LS42 U16
U 1 1 5C2773A8
P 8150 6950
F 0 "U16" H 8150 7828 50  0000 C CNN
F 1 "74LS42" H 8150 7737 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W10.16mm" H 8150 6950 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS42" H 8150 6950 50  0001 C CNN
	1    8150 6950
	-1   0    0    -1  
$EndComp
Wire Wire Line
	7100 5950 7100 6100
Connection ~ 7100 6100
Text Label 10550 6400 0    50   ~ 0
CONTDAT
Wire Wire Line
	10450 5200 9450 5200
Wire Wire Line
	10050 5550 10050 5650
Wire Wire Line
	9450 5200 9450 5450
Connection ~ 9450 5450
Wire Wire Line
	9450 5450 9450 5650
Wire Wire Line
	9600 5950 9600 6500
Wire Wire Line
	9600 6500 10150 6500
Text Label 1900 750  0    50   ~ 0
GND
Wire Wire Line
	1700 750  1300 750 
Wire Wire Line
	1300 750  1300 700 
Text Label 3300 750  0    50   ~ 0
GND
Wire Wire Line
	3100 750  2650 750 
Wire Wire Line
	2650 750  2650 700 
Text Label 4750 750  0    50   ~ 0
GND
Text Label 5950 750  0    50   ~ 0
GND
Text Label 14100 800  0    50   ~ 0
GND
Text Label 15400 3000 0    50   ~ 0
GND
Text Label 13900 3000 0    50   ~ 0
GND
Text Label 11850 3250 0    50   ~ 0
GND
Text Label 10450 3250 0    50   ~ 0
GND
Text Label 9050 3250 0    50   ~ 0
GND
Text Label 7650 3200 0    50   ~ 0
GND
Text Label 6150 3200 0    50   ~ 0
GND
Text Label 4750 3150 0    50   ~ 0
GND
Text Label 3350 3200 0    50   ~ 0
GND
Text Label 2000 3200 0    50   ~ 0
GND
Wire Wire Line
	1350 3200 1800 3200
Wire Wire Line
	3150 3200 2700 3200
Wire Wire Line
	4550 3150 4100 3150
Wire Wire Line
	4100 3150 4100 3200
Wire Wire Line
	5950 3200 5500 3200
Wire Wire Line
	7450 3200 7000 3200
Wire Wire Line
	8850 3250 8400 3250
Wire Wire Line
	8400 3250 8400 3200
Wire Wire Line
	10250 3250 9800 3250
Wire Wire Line
	9800 3250 9800 3200
Wire Wire Line
	11650 3250 11200 3250
Wire Wire Line
	11200 3250 11200 3200
Wire Wire Line
	13700 3000 13250 3000
Wire Wire Line
	13250 3000 13250 2950
Wire Wire Line
	15200 3000 14800 3000
Wire Wire Line
	14800 3000 14800 2950
Wire Wire Line
	13900 800  13450 800 
Wire Wire Line
	13450 800  13450 750 
Wire Wire Line
	5750 750  5500 750 
Wire Wire Line
	5500 750  5500 700 
Wire Wire Line
	4550 750  4100 750 
Wire Wire Line
	4100 750  4100 700 
Text Notes 6350 10850 0    50   ~ 0
Data switches
Text Notes 9900 8300 0    50   ~ 0
LSB control switches
Text Notes 9900 9400 0    50   ~ 0
MSB control switches
Wire Wire Line
	7100 5950 9600 5950
$Comp
L Connector_Generic:Conn_01x09 J6
U 1 1 5C312CC3
P 15000 8650
F 0 "J6" H 15080 8692 50  0000 L CNN
F 1 "Conn_01x09" H 15080 8601 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x09_P2.54mm_Vertical" H 15000 8650 50  0001 C CNN
F 3 "~" H 15000 8650 50  0001 C CNN
	1    15000 8650
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R42
U 1 1 5C31913E
P 14700 8350
F 0 "R42" V 14504 8350 50  0000 C CNN
F 1 "4K7" V 14595 8350 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" H 14700 8350 50  0001 C CNN
F 3 "~" H 14700 8350 50  0001 C CNN
	1    14700 8350
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R44
U 1 1 5C3193F5
P 14700 8550
F 0 "R44" V 14504 8550 50  0000 C CNN
F 1 "4K7" V 14595 8550 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" H 14700 8550 50  0001 C CNN
F 3 "~" H 14700 8550 50  0001 C CNN
	1    14700 8550
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R56
U 1 1 5C319592
P 14700 8750
F 0 "R56" V 14504 8750 50  0000 C CNN
F 1 "4K7" V 14595 8750 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" H 14700 8750 50  0001 C CNN
F 3 "~" H 14700 8750 50  0001 C CNN
	1    14700 8750
	0    1    1    0   
$EndComp
$Comp
L Connector_Generic:Conn_02x09_Odd_Even J5
U 1 1 5C32B280
P 13500 8650
F 0 "J5" H 13550 9267 50  0000 C CNN
F 1 "Conn_02x09_Odd_Even" H 13550 9176 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x09_P2.54mm_Vertical" H 13500 8650 50  0001 C CNN
F 3 "~" H 13500 8650 50  0001 C CNN
	1    13500 8650
	1    0    0    -1  
$EndComp
Text Label 13300 8250 2    50   ~ 0
LCD7
Text Label 13300 8350 2    50   ~ 0
LCD5
Text Label 13800 8250 0    50   ~ 0
LCD6
Text Label 13800 8350 0    50   ~ 0
LCD4
Text Label 13300 8450 2    50   ~ 0
GND
Text Label 13300 8550 2    50   ~ 0
GND
Text Label 13800 8450 0    50   ~ 0
GND
Text Label 13800 8550 0    50   ~ 0
GND
Text Label 13300 8650 2    50   ~ 0
ENLCD1
Text Label 13800 8650 0    50   ~ 0
GND
Text Label 13300 8750 2    50   ~ 0
A0
Text Label 13300 8850 2    50   ~ 0
GND
Text Label 13800 8850 0    50   ~ 0
VCC
$Comp
L Device:R_Small R66
U 1 1 5C31F748
P 13900 8750
F 0 "R66" V 13704 8750 50  0000 C CNN
F 1 "1K" V 13795 8750 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" H 13900 8750 50  0001 C CNN
F 3 "~" H 13900 8750 50  0001 C CNN
	1    13900 8750
	0    1    1    0   
$EndComp
Text Label 13300 8950 2    50   ~ 0
ENLCD2
NoConn ~ 13800 8950
Text Label 13800 9050 0    50   ~ 0
GND
$Comp
L Device:R_Small R58
U 1 1 5C32B7F2
P 13200 9050
F 0 "R58" V 13004 9050 50  0000 C CNN
F 1 "100" V 13095 9050 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" H 13200 9050 50  0001 C CNN
F 3 "~" H 13200 9050 50  0001 C CNN
	1    13200 9050
	0    1    1    0   
$EndComp
Text Label 13100 9050 2    50   ~ 0
VCC
Text Notes 13800 9200 2    50   ~ 0
ON-BOARD LCD
Text Notes 15250 9200 2    50   ~ 0
ON BOARD KEYPAD
Text Label 14800 8650 2    50   ~ 0
LCD5
Text Label 14800 8450 2    50   ~ 0
LCD4
Text Label 14850 8250 2    50   ~ 0
GND
Wire Wire Line
	14600 8350 14600 8250
Wire Wire Line
	14600 8250 14800 8250
Connection ~ 14800 8250
Wire Wire Line
	14800 8250 14850 8250
Text Label 14800 8850 2    50   ~ 0
LCD6
Text Label 14800 8950 2    50   ~ 0
LCD7
Wire Wire Line
	14800 8350 14800 8400
Wire Wire Line
	14800 8400 14600 8400
Wire Wire Line
	14600 8400 14600 8500
Connection ~ 14800 8350
Wire Wire Line
	14800 8550 14800 8600
Wire Wire Line
	14800 8600 14600 8600
Wire Wire Line
	14600 8600 14600 8750
Connection ~ 14800 8550
Wire Wire Line
	14400 8500 14450 8500
Connection ~ 14600 8500
Wire Wire Line
	14600 8500 14600 8550
Text Label 14400 8500 2    50   ~ 0
KEYP
NoConn ~ 14800 9050
Text Label 14000 8750 0    50   ~ 0
GND
$Comp
L Mechanical:MountingHole_Pad PAD1
U 1 1 5C327AEA
P 6600 850
F 0 "PAD1" H 6700 901 50  0000 L CNN
F 1 "MountingHole_Pad" H 6700 810 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad" H 6600 850 50  0001 C CNN
F 3 "~" H 6600 850 50  0001 C CNN
	1    6600 850 
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad PAD2
U 1 1 5C327CCE
P 6600 1150
F 0 "PAD2" H 6700 1201 50  0000 L CNN
F 1 "MountingHole_Pad" H 6700 1110 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad" H 6600 1150 50  0001 C CNN
F 3 "~" H 6600 1150 50  0001 C CNN
	1    6600 1150
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad PAD3
U 1 1 5C327E87
P 6600 1450
F 0 "PAD3" H 6700 1501 50  0000 L CNN
F 1 "MountingHole_Pad" H 6700 1410 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad" H 6600 1450 50  0001 C CNN
F 3 "~" H 6600 1450 50  0001 C CNN
	1    6600 1450
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad PAD4
U 1 1 5C328026
P 6600 1750
F 0 "PAD4" H 6700 1801 50  0000 L CNN
F 1 "MountingHole_Pad" H 6700 1710 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad" H 6600 1750 50  0001 C CNN
F 3 "~" H 6600 1750 50  0001 C CNN
	1    6600 1750
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad PAD5
U 1 1 5C3281C7
P 6600 2100
F 0 "PAD5" H 6700 2151 50  0000 L CNN
F 1 "MountingHole_Pad" H 6700 2060 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad" H 6600 2100 50  0001 C CNN
F 3 "~" H 6600 2100 50  0001 C CNN
	1    6600 2100
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad PAD6
U 1 1 5C328389
P 6600 2400
F 0 "PAD6" H 6700 2451 50  0000 L CNN
F 1 "MountingHole_Pad" H 6700 2360 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad" H 6600 2400 50  0001 C CNN
F 3 "~" H 6600 2400 50  0001 C CNN
	1    6600 2400
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad PAD7
U 1 1 5C32856B
P 7850 850
F 0 "PAD7" H 7950 901 50  0000 L CNN
F 1 "MountingHole_Pad" H 7950 810 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad" H 7850 850 50  0001 C CNN
F 3 "~" H 7850 850 50  0001 C CNN
	1    7850 850 
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad PAD8
U 1 1 5C328743
P 7850 1150
F 0 "PAD8" H 7950 1201 50  0000 L CNN
F 1 "MountingHole_Pad" H 7950 1110 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad" H 7850 1150 50  0001 C CNN
F 3 "~" H 7850 1150 50  0001 C CNN
	1    7850 1150
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad PAD9
U 1 1 5C3288F6
P 7850 1450
F 0 "PAD9" H 7950 1501 50  0000 L CNN
F 1 "MountingHole_Pad" H 7950 1410 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad" H 7850 1450 50  0001 C CNN
F 3 "~" H 7850 1450 50  0001 C CNN
	1    7850 1450
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad PAD10
U 1 1 5C328A95
P 7850 1750
F 0 "PAD10" H 7950 1801 50  0000 L CNN
F 1 "MountingHole_Pad" H 7950 1710 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad" H 7850 1750 50  0001 C CNN
F 3 "~" H 7850 1750 50  0001 C CNN
	1    7850 1750
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad PAD11
U 1 1 5C328C34
P 9300 850
F 0 "PAD11" H 9400 901 50  0000 L CNN
F 1 "MountingHole_Pad" H 9400 810 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.2mm_M2_Pad" H 9300 850 50  0001 C CNN
F 3 "~" H 9300 850 50  0001 C CNN
	1    9300 850 
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad PAD12
U 1 1 5C328DDB
P 9300 1150
F 0 "PAD12" H 9400 1201 50  0000 L CNN
F 1 "MountingHole_Pad" H 9400 1110 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.2mm_M2_Pad" H 9300 1150 50  0001 C CNN
F 3 "~" H 9300 1150 50  0001 C CNN
	1    9300 1150
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad PAD13
U 1 1 5C328FA3
P 9300 1450
F 0 "PAD13" H 9400 1501 50  0000 L CNN
F 1 "MountingHole_Pad" H 9400 1410 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.2mm_M2_Pad" H 9300 1450 50  0001 C CNN
F 3 "~" H 9300 1450 50  0001 C CNN
	1    9300 1450
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad PAD14
U 1 1 5C329199
P 9300 1750
F 0 "PAD14" H 9400 1801 50  0000 L CNN
F 1 "MountingHole_Pad" H 9400 1710 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.2mm_M2_Pad" H 9300 1750 50  0001 C CNN
F 3 "~" H 9300 1750 50  0001 C CNN
	1    9300 1750
	1    0    0    -1  
$EndComp
Text Label 6600 950  2    50   ~ 0
GND
Text Label 6600 1250 2    50   ~ 0
GND
Text Label 6600 1550 2    50   ~ 0
GND
Text Label 7850 1550 2    50   ~ 0
GND
Text Label 6600 1850 2    50   ~ 0
GND
Text Label 9300 950  2    50   ~ 0
GND
Text Label 9300 1550 2    50   ~ 0
GND
Text Label 7850 950  2    50   ~ 0
GND
$Comp
L 74xx:74LS06 U19
U 7 1 5C392B98
P 7900 2450
F 0 "U19" H 8130 2496 50  0000 L CNN
F 1 "74LS06" H 8130 2405 50  0000 L CNN
F 2 "Package_DIP:DIP-14_W10.16mm" H 7900 2450 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS06" H 7900 2450 50  0001 C CNN
	7    7900 2450
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74LS06 U20
U 7 1 5C39930C
P 8600 2450
F 0 "U20" H 8830 2496 50  0000 L CNN
F 1 "74LS06" H 8830 2405 50  0000 L CNN
F 2 "Package_DIP:DIP-14_W10.16mm" H 8600 2450 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS06" H 8600 2450 50  0001 C CNN
	7    8600 2450
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74LS06 U21
U 7 1 5C39BDA2
P 9350 2450
F 0 "U21" H 9580 2496 50  0000 L CNN
F 1 "74LS06" H 9580 2405 50  0000 L CNN
F 2 "Package_DIP:DIP-14_W10.16mm" H 9350 2450 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS06" H 9350 2450 50  0001 C CNN
	7    9350 2450
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74LS02 U18
U 5 1 5C39CE0C
P 10200 2450
F 0 "U18" H 10430 2496 50  0000 L CNN
F 1 "74LS02" H 10430 2405 50  0000 L CNN
F 2 "Package_DIP:DIP-14_W10.16mm" H 10200 2450 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74ls02" H 10200 2450 50  0001 C CNN
	5    10200 2450
	1    0    0    -1  
$EndComp
Text Label 7900 2950 2    50   ~ 0
GND
Text Label 8600 2950 2    50   ~ 0
GND
Text Label 9350 2950 2    50   ~ 0
GND
Text Label 10200 2950 2    50   ~ 0
GND
Text Label 7900 1950 2    50   ~ 0
VCC
Text Label 8600 1950 2    50   ~ 0
VCC
Text Label 9350 1950 2    50   ~ 0
VCC
Text Label 10200 1950 2    50   ~ 0
VCC
$Comp
L Device:CP1_Small C2
U 1 1 5C3CBEEE
P 1900 3200
F 0 "C2" V 2128 3200 50  0000 C CNN
F 1 "CP1_Small" V 2037 3200 50  0000 C CNN
F 2 "Capacitor_THT:CP_Radial_Tantal_D5.0mm_P5.00mm" H 1900 3200 50  0001 C CNN
F 3 "~" H 1900 3200 50  0001 C CNN
	1    1900 3200
	0    -1   -1   0   
$EndComp
$Comp
L Device:CP1_Small C4
U 1 1 5C3CC367
P 3250 3200
F 0 "C4" V 3478 3200 50  0000 C CNN
F 1 "CP1_Small" V 3387 3200 50  0000 C CNN
F 2 "Capacitor_THT:CP_Radial_Tantal_D5.0mm_P5.00mm" H 3250 3200 50  0001 C CNN
F 3 "~" H 3250 3200 50  0001 C CNN
	1    3250 3200
	0    -1   -1   0   
$EndComp
$Comp
L Device:CP1_Small C6
U 1 1 5C3CC7B0
P 4650 3150
F 0 "C6" V 4878 3150 50  0000 C CNN
F 1 "CP1_Small" V 4787 3150 50  0000 C CNN
F 2 "Capacitor_THT:CP_Radial_Tantal_D5.0mm_P5.00mm" H 4650 3150 50  0001 C CNN
F 3 "~" H 4650 3150 50  0001 C CNN
	1    4650 3150
	0    -1   -1   0   
$EndComp
$Comp
L Device:CP1_Small C8
U 1 1 5C3CED65
P 6050 3200
F 0 "C8" V 6278 3200 50  0000 C CNN
F 1 "CP1_Small" V 6187 3200 50  0000 C CNN
F 2 "Capacitor_THT:CP_Radial_Tantal_D5.0mm_P5.00mm" H 6050 3200 50  0001 C CNN
F 3 "~" H 6050 3200 50  0001 C CNN
	1    6050 3200
	0    -1   -1   0   
$EndComp
$Comp
L Device:CP1_Small C9
U 1 1 5C3D0896
P 7550 3200
F 0 "C9" V 7778 3200 50  0000 C CNN
F 1 "CP1_Small" V 7687 3200 50  0000 C CNN
F 2 "Capacitor_THT:CP_Radial_Tantal_D5.0mm_P5.00mm" H 7550 3200 50  0001 C CNN
F 3 "~" H 7550 3200 50  0001 C CNN
	1    7550 3200
	0    -1   -1   0   
$EndComp
$Comp
L Device:CP1_Small C10
U 1 1 5C3D2CF1
P 8950 3250
F 0 "C10" V 9178 3250 50  0000 C CNN
F 1 "CP1_Small" V 9087 3250 50  0000 C CNN
F 2 "Capacitor_THT:CP_Radial_Tantal_D5.0mm_P5.00mm" H 8950 3250 50  0001 C CNN
F 3 "~" H 8950 3250 50  0001 C CNN
	1    8950 3250
	0    -1   -1   0   
$EndComp
$Comp
L Device:CP1_Small C11
U 1 1 5C3D2EAE
P 10350 3250
F 0 "C11" V 10578 3250 50  0000 C CNN
F 1 "CP1_Small" V 10487 3250 50  0000 C CNN
F 2 "Capacitor_THT:CP_Radial_Tantal_D5.0mm_P5.00mm" H 10350 3250 50  0001 C CNN
F 3 "~" H 10350 3250 50  0001 C CNN
	1    10350 3250
	0    -1   -1   0   
$EndComp
$Comp
L Device:CP1_Small C12
U 1 1 5C3D6FA5
P 11750 3250
F 0 "C12" V 11978 3250 50  0000 C CNN
F 1 "CP1_Small" V 11887 3250 50  0000 C CNN
F 2 "Capacitor_THT:CP_Radial_Tantal_D5.0mm_P5.00mm" H 11750 3250 50  0001 C CNN
F 3 "~" H 11750 3250 50  0001 C CNN
	1    11750 3250
	0    -1   -1   0   
$EndComp
$Comp
L Device:CP1_Small C13
U 1 1 5C3D947E
P 13800 3000
F 0 "C13" V 14028 3000 50  0000 C CNN
F 1 "CP1_Small" V 13937 3000 50  0000 C CNN
F 2 "Capacitor_THT:CP_Radial_Tantal_D5.0mm_P5.00mm" H 13800 3000 50  0001 C CNN
F 3 "~" H 13800 3000 50  0001 C CNN
	1    13800 3000
	0    -1   -1   0   
$EndComp
$Comp
L Device:CP1_Small C14
U 1 1 5C3DCF59
P 14000 800
F 0 "C14" V 14228 800 50  0000 C CNN
F 1 "CP1_Small" V 14137 800 50  0000 C CNN
F 2 "Capacitor_THT:CP_Radial_Tantal_D5.0mm_P5.00mm" H 14000 800 50  0001 C CNN
F 3 "~" H 14000 800 50  0001 C CNN
	1    14000 800 
	0    -1   -1   0   
$EndComp
$Comp
L Device:CP1_Small C7
U 1 1 5C3E18D4
P 5850 750
F 0 "C7" V 6078 750 50  0000 C CNN
F 1 "CP1_Small" V 5987 750 50  0000 C CNN
F 2 "Capacitor_THT:CP_Radial_Tantal_D5.0mm_P5.00mm" H 5850 750 50  0001 C CNN
F 3 "~" H 5850 750 50  0001 C CNN
	1    5850 750 
	0    -1   -1   0   
$EndComp
$Comp
L Device:CP1_Small C5
U 1 1 5C3E1AA1
P 4650 750
F 0 "C5" V 4878 750 50  0000 C CNN
F 1 "CP1_Small" V 4787 750 50  0000 C CNN
F 2 "Capacitor_THT:CP_Radial_Tantal_D5.0mm_P5.00mm" H 4650 750 50  0001 C CNN
F 3 "~" H 4650 750 50  0001 C CNN
	1    4650 750 
	0    -1   -1   0   
$EndComp
$Comp
L Device:CP1_Small C3
U 1 1 5C3E47DA
P 3200 750
F 0 "C3" V 3428 750 50  0000 C CNN
F 1 "CP1_Small" V 3337 750 50  0000 C CNN
F 2 "Capacitor_THT:CP_Radial_Tantal_D5.0mm_P5.00mm" H 3200 750 50  0001 C CNN
F 3 "~" H 3200 750 50  0001 C CNN
	1    3200 750 
	0    -1   -1   0   
$EndComp
$Comp
L Device:CP1_Small C1
U 1 1 5C3E66B7
P 1800 750
F 0 "C1" V 2028 750 50  0000 C CNN
F 1 "CP1_Small" V 1937 750 50  0000 C CNN
F 2 "Capacitor_THT:CP_Radial_Tantal_D5.0mm_P5.00mm" H 1800 750 50  0001 C CNN
F 3 "~" H 1800 750 50  0001 C CNN
	1    1800 750 
	0    -1   -1   0   
$EndComp
$Comp
L Device:CP1_Small C15
U 1 1 5C436CC5
P 15300 3000
F 0 "C15" V 15528 3000 50  0000 C CNN
F 1 "CP1_Small" V 15437 3000 50  0000 C CNN
F 2 "Capacitor_THT:CP_Radial_Tantal_D5.0mm_P5.00mm" H 15300 3000 50  0001 C CNN
F 3 "~" H 15300 3000 50  0001 C CNN
	1    15300 3000
	0    -1   -1   0   
$EndComp
Text Label 6600 2200 2    50   ~ 0
GND
Text Label 6600 2500 2    50   ~ 0
GND
Text Label 7850 1250 2    50   ~ 0
GND
Text Label 7850 1850 2    50   ~ 0
GND
Text Label 9300 1250 2    50   ~ 0
GND
Text Label 9300 1850 2    50   ~ 0
GND
Text Notes 800  9300 0    50   ~ 0
SAE CPH 8100-56\n(must clip outer 4 pins)\n(must saw off screw flaps)
Text Notes 7400 10650 0    50   ~ 0
Register list LSB:\n0 = write lsb data 12-15\n1 = write lsb data 8-11\n2 = write lsb inst 12-15 (optional)\n3 = write lsb inst 8-11 (optional)\n4 = read data lsb 12-15\n5 = read data lsb 8-11\n6 = read status 0-3\n7 = spare\n20 = read addr lsb 12-15\n21 = read addr lsb 8-11\n22= read status 4\n
Text Notes 8850 10600 0    50   ~ 0
Register list MSB:\n8 = write msb data 4-7\n9 = write msb data 0-3\n10 = write control 5-7\n11 = write control 0-3\n12 = read data msb 4-7\n13 = read data msb 0-3\n14 = write msb inst 4-7\n15 = write msb inst 0-3\n28 = read addr msb 4-7\n29 = read addr msb 0-3\n
$Comp
L Device:R_Small R67
U 1 1 5C3B3FEE
P 11700 8350
F 0 "R67" V 11504 8350 50  0000 C CNN
F 1 "1M" V 11595 8350 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" H 11700 8350 50  0001 C CNN
F 3 "~" H 11700 8350 50  0001 C CNN
	1    11700 8350
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R68
U 1 1 5C3BA4A0
P 11700 9400
F 0 "R68" V 11504 9400 50  0000 C CNN
F 1 "1M" V 11595 9400 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" H 11700 9400 50  0001 C CNN
F 3 "~" H 11700 9400 50  0001 C CNN
	1    11700 9400
	0    1    1    0   
$EndComp
$Comp
L Device:C_Small C17
U 1 1 5C3F42D8
P 11800 9500
F 0 "C17" H 11892 9546 50  0000 L CNN
F 1 "C_Small" H 11892 9455 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_P2.50mm" H 11800 9500 50  0001 C CNN
F 3 "~" H 11800 9500 50  0001 C CNN
	1    11800 9500
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C16
U 1 1 5C3F454A
P 11800 8450
F 0 "C16" H 11892 8496 50  0000 L CNN
F 1 "C_Small" H 11892 8405 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_P2.50mm" H 11800 8450 50  0001 C CNN
F 3 "~" H 11800 8450 50  0001 C CNN
	1    11800 8450
	1    0    0    -1  
$EndComp
Text Label 11800 8550 0    50   ~ 0
GND
Text Label 11800 9600 0    50   ~ 0
GND
NoConn ~ 7650 7250
Text Notes 12300 7100 0    50   ~ 0
SPI
Text Notes 12300 7200 0    50   ~ 0
SPI
Text Notes 12300 7300 0    50   ~ 0
SPI
Text Notes 12500 7000 0    50   ~ 0
A0
Text Notes 12500 7100 0    50   ~ 0
A1
Text Notes 12500 7200 0    50   ~ 0
A2
Text Notes 12500 7300 0    50   ~ 0
A0'
Text Notes 12450 6600 0    50   ~ 0
ENLCD1
Text Notes 12450 6700 0    50   ~ 0
ENLCD2
Text Notes 11800 6450 0    50   ~ 0
Proposed to allow SPI
Text Notes 14200 6700 0    50   ~ 0
LCD4
Text Notes 14200 6800 0    50   ~ 0
LCD5
Text Notes 14200 6900 0    50   ~ 0
LCD6
Text Notes 14200 7000 0    50   ~ 0
I2C. LCD7
Text Notes 14200 7100 0    50   ~ 0
I2C
Text Notes 14050 6450 0    50   ~ 0
Proposed to isolate I2C \nfrom LCD
Text Notes 14200 6600 0    50   ~ 0
FREE
Text Notes 14200 7300 0    50   ~ 0
FREE
Text Notes 14100 10250 0    50   ~ 0
ERRORS: PINHEADERS WRONG FOOTPRINT\nAREF NOT CONNECTED TO KEYCHAIN; \nHole size Nova conn
$Comp
L Device:R_Small R69
U 1 1 5C7B8356
P 13350 9550
F 0 "R69" V 13154 9550 50  0000 C CNN
F 1 "Jumper" V 13245 9550 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" H 13350 9550 50  0001 C CNN
F 3 "~" H 13350 9550 50  0001 C CNN
	1    13350 9550
	0    1    1    0   
$EndComp
Wire Wire Line
	14450 8500 14450 9550
Wire Wire Line
	14450 9550 13450 9550
Connection ~ 14450 8500
Wire Wire Line
	14450 8500 14600 8500
Wire Wire Line
	13250 9550 12600 9550
Text Notes 12650 9700 0    50   ~ 0
Place when using Teensy 3.5 processor
Wire Wire Line
	12600 9550 12600 8350
Wire Wire Line
	12600 8350 11800 8350
Connection ~ 11800 8350
$EndSCHEMATC
