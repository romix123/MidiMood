EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "MidiMood v3"
Date ""
Rev "1"
Comp ""
Comment1 "MidiMooD pcb version 3 based on earlier work by Gustavo Silveira (musicalnerd.com/youtube"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Mechanical:MountingHole_Pad H4
U 1 1 60694900
P 800 1000
F 0 "H4" H 900 1003 50  0000 L CNN
F 1 "MountingHole_Pad" H 900 958 50  0001 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad_TopBottom" H 800 1000 50  0001 C CNN
F 3 "~" H 800 1000 50  0001 C CNN
	1    800  1000
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H1
U 1 1 606B552D
P 800 700
F 0 "H1" H 900 703 50  0000 L CNN
F 1 "MountingHole_Pad" H 900 658 50  0001 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad_TopBottom" H 800 700 50  0001 C CNN
F 3 "~" H 800 700 50  0001 C CNN
	1    800  700 
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H2
U 1 1 606B6290
P 1200 700
F 0 "H2" H 1300 703 50  0000 L CNN
F 1 "MountingHole_Pad" H 1300 658 50  0001 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad_TopBottom" H 1200 700 50  0001 C CNN
F 3 "~" H 1200 700 50  0001 C CNN
	1    1200 700 
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H3
U 1 1 606B6F12
P 1550 700
F 0 "H3" H 1650 703 50  0000 L CNN
F 1 "MH" H 1650 658 50  0001 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad_TopBottom" H 1550 700 50  0001 C CNN
F 3 "~" H 1550 700 50  0001 C CNN
	1    1550 700 
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H5
U 1 1 606B7BA2
P 1550 1000
F 0 "H5" H 1650 1003 50  0000 L CNN
F 1 "MountingHole_Pad" H 1650 958 50  0001 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad_TopBottom" H 1550 1000 50  0001 C CNN
F 3 "~" H 1550 1000 50  0001 C CNN
	1    1550 1000
	1    0    0    -1  
$EndComp
Wire Wire Line
	800  800  1200 800 
Wire Wire Line
	1550 800  1200 800 
Connection ~ 1200 800 
Wire Wire Line
	1200 800  1200 1100
Wire Wire Line
	800  1100 1200 1100
Connection ~ 1200 1100
Wire Wire Line
	1200 1100 1550 1100
$Comp
L power:GND #PWR05
U 1 1 607A0BC3
P 1200 1100
F 0 "#PWR05" H 1200 850 50  0001 C CNN
F 1 "GND" H 1205 927 50  0000 C CNN
F 2 "" H 1200 1100 50  0001 C CNN
F 3 "" H 1200 1100 50  0001 C CNN
	1    1200 1100
	1    0    0    -1  
$EndComp
$EndSCHEMATC
