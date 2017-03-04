EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:switches
LIBS:m68kcomp1
LIBS:m68kcomp1-cache
EELAYER 25 0
EELAYER END
$Descr A3 16535 11693
encoding utf-8
Sheet 2 2
Title "72pin SIMM DRAM controller"
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel -150 50   0    60   Input ~ 0
A0
Text HLabel -150 150  0    60   Input ~ 0
A1
Text HLabel -150 250  0    60   Input ~ 0
A2
Text HLabel -150 350  0    60   Input ~ 0
A3
Text HLabel -150 450  0    60   Input ~ 0
A4
Text HLabel -150 550  0    60   Input ~ 0
A5
Text HLabel -150 650  0    60   Input ~ 0
A6
Text HLabel -150 750  0    60   Input ~ 0
A7
Text HLabel -150 850  0    60   Input ~ 0
A8
Text HLabel -150 950  0    60   Input ~ 0
A9
Text HLabel -150 1050 0    60   Input ~ 0
A10
Text HLabel -150 1150 0    60   Input ~ 0
A11
Text HLabel -150 1250 0    60   Input ~ 0
A12
Text HLabel -150 1350 0    60   Input ~ 0
A13
Text HLabel -150 1450 0    60   Input ~ 0
A14
Text HLabel -150 1550 0    60   Input ~ 0
A15
Text HLabel -150 1650 0    60   Input ~ 0
A16
Text HLabel -150 1750 0    60   Input ~ 0
A17
Text HLabel -150 1850 0    60   Input ~ 0
A18
Text HLabel -150 1950 0    60   Input ~ 0
A19
Text HLabel -150 2050 0    60   Input ~ 0
A20
Text HLabel -150 2150 0    60   Input ~ 0
A21
Text HLabel 7700 850  2    60   BiDi ~ 0
D0
Text HLabel 7700 950  2    60   BiDi ~ 0
D1
Text HLabel 7700 1050 2    60   BiDi ~ 0
D2
Text HLabel 7700 1150 2    60   BiDi ~ 0
D3
Text HLabel 7700 1250 2    60   BiDi ~ 0
D4
Text HLabel 7700 1350 2    60   BiDi ~ 0
D5
Text HLabel 7700 1450 2    60   BiDi ~ 0
D6
Text HLabel 7700 1550 2    60   BiDi ~ 0
D7
Text HLabel 7700 1750 2    60   BiDi ~ 0
D8
Text HLabel 7700 1850 2    60   BiDi ~ 0
D9
Text HLabel 7700 1950 2    60   BiDi ~ 0
D10
Text HLabel 7700 2050 2    60   BiDi ~ 0
D11
Text HLabel 7700 2150 2    60   BiDi ~ 0
D12
Text HLabel 7700 2250 2    60   BiDi ~ 0
D13
Text HLabel 7700 2350 2    60   BiDi ~ 0
D14
Text HLabel 7700 2450 2    60   BiDi ~ 0
D15
Text HLabel 3650 3300 0    60   Input ~ 0
~UDS
Text HLabel 3650 2650 0    60   Input ~ 0
~LDS
Text HLabel 4600 4550 0    60   Input ~ 0
R/~W
Text HLabel 3150 4800 0    60   Input ~ 0
~CS
Text HLabel 1400 4900 0    60   Input ~ 0
~OE
Text HLabel 2750 6350 0    60   Input ~ 0
CLK20
$Comp
L SIM4X32 U?
U 1 1 58B9C949
P 5850 2650
F 0 "U?" H 5850 3200 50  0000 C CNN
F 1 "SIM4X32" H 6300 650 50  0000 C CNN
F 2 "" H 5850 2650 50  0000 C CNN
F 3 "" H 5850 2650 50  0000 C CNN
	1    5850 2650
	1    0    0    -1  
$EndComp
NoConn ~ 5050 3550
NoConn ~ 5050 3650
NoConn ~ 5050 3750
NoConn ~ 5050 3850
Text HLabel -150 2400 0    60   Input ~ 0
A22
$Comp
L 74HC04 U?
U 1 1 58B9CFBE
P -450 2700
F 0 "U?" H -300 2800 50  0000 C CNN
F 1 "74HC04" H -250 2600 50  0000 C CNN
F 2 "" H -450 2700 50  0000 C CNN
F 3 "" H -450 2700 50  0000 C CNN
	1    -450 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6650 850  7700 850 
Wire Wire Line
	6650 2650 6800 2650
Wire Wire Line
	6800 2650 6800 850 
Connection ~ 6800 850 
Wire Wire Line
	6650 950  7700 950 
Wire Wire Line
	6650 1050 7700 1050
Wire Wire Line
	6650 1150 7700 1150
Wire Wire Line
	6650 1250 7700 1250
Wire Wire Line
	6650 1350 7700 1350
Wire Wire Line
	6650 1450 7700 1450
Wire Wire Line
	6650 1550 7700 1550
Wire Wire Line
	6650 2750 6850 2750
Wire Wire Line
	6850 2750 6850 950 
Connection ~ 6850 950 
Wire Wire Line
	6650 2850 6900 2850
Wire Wire Line
	6900 2850 6900 1050
Connection ~ 6900 1050
Wire Wire Line
	6650 2950 6950 2950
Wire Wire Line
	6950 2950 6950 1150
Connection ~ 6950 1150
Wire Wire Line
	6650 3050 7000 3050
Wire Wire Line
	7000 3050 7000 1250
Connection ~ 7000 1250
Wire Wire Line
	6650 3150 7050 3150
Wire Wire Line
	7050 3150 7050 1350
Connection ~ 7050 1350
Wire Wire Line
	6650 3250 7100 3250
Wire Wire Line
	7100 3250 7100 1450
Connection ~ 7100 1450
Wire Wire Line
	6650 3350 7150 3350
Wire Wire Line
	7150 3350 7150 1550
Connection ~ 7150 1550
Wire Wire Line
	6650 1750 7700 1750
Wire Wire Line
	6650 1850 7700 1850
Wire Wire Line
	6650 1950 7700 1950
Wire Wire Line
	6650 2050 7700 2050
Wire Wire Line
	6650 2150 7700 2150
Wire Wire Line
	6650 2250 7700 2250
Wire Wire Line
	6650 2350 7700 2350
Wire Wire Line
	6650 2450 7700 2450
Wire Wire Line
	6650 3550 7250 3550
Wire Wire Line
	7250 3550 7250 1750
Connection ~ 7250 1750
Wire Wire Line
	6650 3650 7300 3650
Wire Wire Line
	7300 3650 7300 1850
Connection ~ 7300 1850
Wire Wire Line
	6650 3750 7350 3750
Wire Wire Line
	7350 3750 7350 1950
Connection ~ 7350 1950
Wire Wire Line
	6650 3850 7400 3850
Wire Wire Line
	7400 3850 7400 2050
Connection ~ 7400 2050
Wire Wire Line
	6650 3950 7450 3950
Wire Wire Line
	7450 3950 7450 2150
Connection ~ 7450 2150
Wire Wire Line
	6650 4050 7500 4050
Wire Wire Line
	7500 4050 7500 2250
Connection ~ 7500 2250
Wire Wire Line
	6650 4150 7550 4150
Wire Wire Line
	7550 4150 7550 2350
Connection ~ 7550 2350
Wire Wire Line
	6650 4250 7600 4250
Wire Wire Line
	7600 4250 7600 2450
Connection ~ 7600 2450
Text HLabel 3150 4600 0    60   Input ~ 0
~AS
$Comp
L 74LS32 U?
U 2 1 58B9D319
P 4800 3950
F 0 "U?" H 4800 4000 50  0000 C CNN
F 1 "74LS32" H 4800 3900 50  0000 C CNN
F 2 "" H 4800 3950 50  0000 C CNN
F 3 "" H 4800 3950 50  0000 C CNN
	2    4800 3950
	0    1    -1   0   
$EndComp
Wire Wire Line
	4800 3350 5050 3350
Wire Wire Line
	4600 4550 4700 4550
Wire Wire Line
	4900 4550 4900 4700
Wire Wire Line
	4900 4700 4350 4700
Text Label 4350 4700 0    60   ~ 0
~EN
Text Label 4850 3350 0    60   ~ 0
~WE
Wire Wire Line
	4850 2850 5050 2850
Wire Wire Line
	4850 2950 5050 2950
Wire Wire Line
	5050 3050 5000 3050
Wire Wire Line
	5000 3050 5000 2850
Connection ~ 5000 2850
Wire Wire Line
	5050 3150 4950 3150
Wire Wire Line
	4950 3150 4950 2950
Connection ~ 4950 2950
$Comp
L 74LS32 U?
U 3 1 58B9DEA5
P 4250 2750
F 0 "U?" H 4250 2800 50  0000 C CNN
F 1 "74LS32" H 4250 2700 50  0000 C CNN
F 2 "" H 4250 2750 50  0000 C CNN
F 3 "" H 4250 2750 50  0000 C CNN
	3    4250 2750
	1    0    0    -1  
$EndComp
$Comp
L 74LS32 U?
U 4 1 58B9DF6F
P 4250 3200
F 0 "U?" H 4250 3250 50  0000 C CNN
F 1 "74LS32" H 4250 3150 50  0000 C CNN
F 2 "" H 4250 3200 50  0000 C CNN
F 3 "" H 4250 3200 50  0000 C CNN
	4    4250 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4850 2750 4850 2850
Wire Wire Line
	3650 2850 3650 3100
Wire Wire Line
	3650 2950 3400 2950
Connection ~ 3650 2950
Text Label 3400 2950 0    60   ~ 0
~CAS
Wire Wire Line
	4850 2950 4850 3200
$Comp
L 74LS32 U?
U 1 1 58B9F11D
P 3750 4700
F 0 "U?" H 3750 4750 50  0000 C CNN
F 1 "74LS32" H 3750 4650 50  0000 C CNN
F 2 "" H 3750 4700 50  0000 C CNN
F 3 "" H 3750 4700 50  0000 C CNN
	1    3750 4700
	1    0    0    -1  
$EndComp
$Comp
L 74HC74 U?
U 1 1 58B9FDC4
P 3700 6350
F 0 "U?" H 3850 6650 50  0000 C CNN
F 1 "74HC74" H 4000 6055 50  0000 C CNN
F 2 "" H 3700 6350 50  0000 C CNN
F 3 "" H 3700 6350 50  0000 C CNN
	1    3700 6350
	1    0    0    -1  
$EndComp
$Comp
L 74HC74 U?
U 2 1 58B9FE2E
P 3700 7800
F 0 "U?" H 3850 8100 50  0000 C CNN
F 1 "74HC74" H 4000 7505 50  0000 C CNN
F 2 "" H 3700 7800 50  0000 C CNN
F 3 "" H 3700 7800 50  0000 C CNN
	2    3700 7800
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR?
U 1 1 58BA006E
P 3700 5800
F 0 "#PWR?" H 3700 5650 50  0001 C CNN
F 1 "VCC" H 3700 5950 50  0000 C CNN
F 2 "" H 3700 5800 50  0000 C CNN
F 3 "" H 3700 5800 50  0000 C CNN
	1    3700 5800
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR?
U 1 1 58BA0098
P 3700 7250
F 0 "#PWR?" H 3700 7100 50  0001 C CNN
F 1 "VCC" H 3700 7400 50  0000 C CNN
F 2 "" H 3700 7250 50  0000 C CNN
F 3 "" H 3700 7250 50  0000 C CNN
	1    3700 7250
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 6150 4500 6150
Wire Wire Line
	4500 6150 4500 7000
Wire Wire Line
	4500 7000 3100 7000
Wire Wire Line
	3100 7000 3100 7600
Wire Wire Line
	4300 8000 5200 8000
Wire Wire Line
	4650 8000 4650 5550
Wire Wire Line
	4650 5550 3100 5550
Wire Wire Line
	3100 5550 3100 6150
Wire Wire Line
	2750 6350 3100 6350
Wire Wire Line
	3000 6350 3000 7800
Wire Wire Line
	3000 7800 3100 7800
Connection ~ 3000 6350
Wire Wire Line
	2600 6900 3700 6900
Wire Wire Line
	2850 6900 2850 8350
Wire Wire Line
	2850 8350 3700 8350
Connection ~ 2850 6900
Text Label 2650 6900 0    60   ~ 0
~EN
Wire Wire Line
	4300 6550 5200 6550
NoConn ~ 4300 7600
Text Label 4750 6550 0    60   ~ 0
~CAS_REFR
Connection ~ 4650 8000
Text Label 4750 8000 0    60   ~ 0
~RAS_REFR
Wire Notes Line
	2300 5400 5400 5400
Wire Notes Line
	5400 5400 5400 8800
Wire Notes Line
	5400 8800 2300 8800
Wire Notes Line
	2300 8800 2300 5400
Text Notes 2900 8700 0    60   ~ 0
CAS-before-RAS Refresh when not enabled
$EndSCHEMATC
