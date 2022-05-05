# MidiMood
 Minimoog inspired MIDI Control Surface

![MidiMoodFrontSmall](https://user-images.githubusercontent.com/1766442/166905280-b6f1127a-f126-4773-8b13-208afb08fc6d.jpg)

MidiMood is a MIDI Control Surface inspired by the synth classic Minimoog. It runs on an atmega32U4 (in this case a cheap Pro Mirco Arduino Leonardo clone) and has 26 Potmeters, 11 switches and an LED. Connected to an iPad and running the Minimoog app, one has a cheap version of a Minimoog.
The design is inspired by Gustavo Silveira's 2019 midimood, see
 * http://www.musiconerd.com
 * http://www.youtube.com/musiconerd
 * http://facebook.com/musiconerdmusiconerd
 
The code included in the rep runs on tttapa (Pieter P)'s wonderfully powerful Control Surface, https://github.com/tttapa/Control-Surface
This library (platform) is incredibly powerful, but at the same time very resource consuming. The MidiMood hardware contains a lot of stuff that cannnot be accomodated for in the memory space available on a Pro Micro.

## Features
- bus powered USB midi (just plug the board in an iPad, Mac or PC and go)
- 26 potentiometers that can each be mapped on a CC message in the firmware
- 11 switches that can each be mapped on a CC message in the firmware 
- optional front panel 
- pcb can be used without front panel (silk screen contains the labels for the various pots)
<img width="2263" alt="midimoog v3 pcb front" src="https://user-images.githubusercontent.com/1766442/166908473-340496c2-a4e7-4393-9d9e-26f4875d0e18.png">

## options
- the board contains serial MIDI in (3.5 mm TRS) with opto-coupler (untested)
- MIDI out (3.5 mm TRS)
- 4 analog inputs 
- 2 digital inputs
- a USB-host adapter allowing hooking up a USB midi keyboard

## components
- My pcb's were made by JLCPCB https://jlcpcb.com. They provide excellent service with low prices and low shipping costs. The pcb's come at around 20EUR for 5. 
- All components can be found on AliExpress
- Rocker Switches 10X15Mm Spst 2Pin 3A 250V KCD11 Snap-In 
https://star.aliexpress.com/share/share.htm?image=Ue034913267074a1898102128b939fe87f.jpg&businessType=ProductDetail&title=%E2%82%AC%200%2C54%20%202%EF%BC%85%20Off%20%7C%205%2F10Pcs%20Drukknop%2010X15Mm%20Spst%202Pin%203A%20250V%20KCD11%20Snap-In%20on%2FOff%20Rocker%20Switch%2010Mm%20%2A%2015Mm%20Zwart%20Rood%20En%20Wit&platform=AE&redirectUrl=https%3A%2F%2Fwww.aliexpress.com%2Fitem%2F1005001513148147.html%3F%26srcSns%3Dsns_Copy%26tid%3Dwhite_backgroup_101%26mb%3DY2QGeZQOErOWB7e%26businessType%3DProductDetail%26spreadType%3DsocialShare
- R0931 Type Single Unit B10K Hald Shaft (D-shaft) Potentiometer Length 12mm
https://star.aliexpress.com/share/share.htm?image=U1c3688bd777744858c1ae8a51962c310y.jpg&businessType=ProductDetail&title=%E2%82%AC%201%2C13%20%7C%205Pcs%20R0931%20Type%20Single%20Unit%20B10K%20Verstelbare%20Potentiometer%20Handvat%20Lengte%2012Mm%20Half%20As&platform=AE&redirectUrl=https%3A%2F%2Fwww.aliexpress.com%2Fitem%2F4000040710218.html%3F%26srcSns%3Dsns_Copy%26tid%3Dwhite_backgroup_101%26mb%3DY2QGeZQOErOWB7e%26businessType%3DProductDetail%26spreadType%3DsocialShare
- 15*20Mm Half-As Pioneer Mixer DJM750 800 850 900 2000 Mixer Pioneer Eq Gain rubber Knop
https://star.aliexpress.com/share/share.htm?image=Udb866b4dc8c74061a6c4adefa7beaa71f.jpg&businessType=ProductDetail&title=%E2%82%AC%202%2C37%20%7C%205%20Stuk%2015%2A20Mm%20Half-As%20Pioneer%20Mixer%20DJM750%20800%20850%20900%202000%20Mixer%20Pioneer%20Eq%20Gain%20rubber%20Knop&platform=AE&redirectUrl=https%3A%2F%2Fwww.aliexpress.com%2Fitem%2F1005001688612127.html%3F%26srcSns%3Dsns_Copy%26tid%3Dwhite_backgroup_101%26mb%3DY2QGeZQOErOWB7e%26businessType%3DProductDetail%26spreadType%3DsocialShare
- Pro Micro ATMEGA32U4 5V/16Mhz Module Met De Bootloader Voor Arduino Mini Usb/Micro Usb Met 2 Rij Pin Header Voor Arduino
https://star.aliexpress.com/share/share.htm?image=U8b53bd56fdd848b98b97a7b488ecd0e5a.jpg&businessType=ProductDetail&title=%E2%82%AC%207%2C30%20%2019%EF%BC%85%20Off%20%7C%20Pro%20Micro%20ATMEGA32U4%205V%2F16Mhz%20Module%20Met%20De%20Bootloader%20Voor%20Arduino%20Mini%20Usb%2FMicro%20Usb%20Met%202%20Rij%20Pin%20Header%20Voor%20Arduino&platform=AE&redirectUrl=https%3A%2F%2Fwww.aliexpress.com%2Fitem%2F1005001335338291.html%3F%26srcSns%3Dsns_Copy%26tid%3Dwhite_backgroup_101%26mb%3DY2QGeZQOErOWB7e%26businessType%3DProductDetail%26spreadType%3DsocialShare
