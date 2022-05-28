# PewPew6
## ReMap and Rapid Fire
### Introduction

Based on buffi's original openpewpew, PewPew6 is an Arduino Nano based device for rapid fire and button remap in a JAMMA arcade cabinet. PewPew6 supports up to 6 input buttons and can map those to any of 6 outputs. It simply connects in-line between the control panel buttons and the existing wiring harness. In an unconfigured state PewPew6 will provide simple passthrough, making it as though PewPew6 was not even installed. To load custom rapid fire and button remapping, PewPew6 leverages NFC cards. An NFC card card be configured with a desired auto fire and button remapping configuration. When the card is in proximity of the NFC reader, PewPew6 will change configuration on the fly and apply the rapid fire and remap settings stored on the card. This allows you to preconfigure you favorite buttons mappings and/or auto fire rates and store those on any number of NFC cards. 

### PewPew6 Features

- 6 button input and 6 outputs
- Button Remap
- Separate frames_on and frames_off rapid fire settings
- NFC card reader for loading settings
- 6 LED indicators show per button configuration


### NFC Card Format

Settings are stored in a 'Text Record' on the NFC card. 

Each line consists of the following comma separated values:
(Button)(rapid enable)(frames_on)(frames_off)(Output)

-Only one button definition per line
-Last button definition ends with ';'

Line Example
-------
A,0,1,1,1 = Button A, No auto fire enabled, 1 frame_on, 1 frame_off, Output 1
**Note - When auto fire is disabled, the frame_on/frame_off vales are meaningless**


Example Text Record 1:
--------------------
A,0,1,1,1
B,0,1,1,2
C,0,1,1,3
D,0,1,1,4
E,0,1,1,5
F,0,1,1,6;


The example text record 1 shown above displays the correct format. The settings in this example are the default settings stored in the Arduino. No rapid fire, no remapping - passthrough only. 


Example Text Record 2:
--------------------
A,0,1,1,1
B,1,2,2,1
C,1,1,4,1
D,1,4,4,1
E,1,10,1,1
F,1,5,5,1;

The example text record 2 shown above maps all 6 buttons to JAMMA-1. 
Button A has no rapid fire, 1 frame_on, 1 frame_off, JAMMA-1
Button B has rapid fire enabled, 1 frame_on, 1 frame_off, JAMMA-1
Button C has rapid fire enabled, 1 frame_on, 4 frame_off, JAMMA-1
Button D has rapid fire enabled, 4 frame_on, 4 frame_off, JAMMA-1
Button E has rapid fire enabled, 10 frame_on, 1 frame_off, JAMMA-1
Button F has rapid fire enabled, 5 frame_on, 5 frame_off, JAMMA-1


Example Text Record 3:
--------------------
A,0,1,1,1
B,0,2,2,2
D,0,4,4,3
E,0,10,10,4;

The example text record 3 shown above is a fighting game example. 
Only 4 buttons are mapped, all buttons have rapid fire disabled - this is used only for button remapping and no rapid fire.