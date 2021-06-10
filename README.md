# VacHoseSwitchingBox
A Shop Vacuum Hub Box capable of switching the central shop vac hose to 3 possible machine ports. 
Arduino based control with remote control.

This repo gathers the design files and arduino software for the project.
This project is a complement to the Shop Cyclone project that can be seen here https://github.com/ChristopheMineau/CycloneTurbine

The project is made of a wooden box which is the actual Hub on one side, and a remotely wall mounted plastic control box, which contains a PCB, an arduino board and a few additional components (micro-switches, leds, resistors and a transistor). The two boxes are connected one to the other using a telphone 8 conductors cable.

The command box is powered using a reclaimed 12V DC power supply plug. 

The Cyclone Turbine project uses a remote control, and I decided to use the same remote control, which had plenty of leftover buttons available to control this Hose hub.

The Hub is on one side connected to the Cyclone and to the other side connected to three possible machines or shop stands. 
For instance, in my case :
* One port is permanently connected to the table saw,
* An other to the big band saw,
* And the third one to a 5m flying hose for any use around the shop.

I just have to press buttons P1 / P2 / P3 on the remote control and wait for a few seconds that the desired port is connected to the vacuum.
As an alternative, I can press a faceplate button as well to switch between the ports.
Three leds indicate the current position, and they alternately blink when travelling.
An error condition would be indicated by the three leds blinking at the same time.

![Gear cutting 2](https://github.com/ChristopheMineau/VacHoseSwitchingBox/blob/master/Pictures/HoseSwitchingBox_15.jpg)
![Gear cutting 2](https://github.com/ChristopheMineau/VacHoseSwitchingBox/blob/master/Pictures/HoseSwitchingBox_18.jpg)

## Contents

### Box design folder
The mechanism is based on a large wooden geared crown which acts as an obturator, with a hole allowing to open only one of the three ports.

![Model output ports view](https://github.com/ChristopheMineau/VacHoseSwitchingBox/blob/master/Pictures/Model_3ports_view.png)
![Model gears view](https://github.com/ChristopheMineau/VacHoseSwitchingBox/blob/master/Pictures/Model_Gears_view.png)

There is a small gear which is motored using a small high tork 12V DC motor running at 30 RPM. (Model JGY-371).
This particular model has got a digital encoder in order to locate the exact position of the obturator.
![Motor](https://github.com/ChristopheMineau/VacHoseSwitchingBox/blob/master/Pictures/DC12V_30RPM_JGY-371_.jpg)

A reference position is provided by a Hall effect sensor visible on the first picture above. 
A 5mm cylindrical neodimium magnet is inlaid in a small wooden block screwed to the big crown in order to trigger the Hall effect sensor once per revolution.

#### 3dm file
The design is modeled using Rhino 3D.
See https://github.com/ChristopheMineau/VacHoseSwitchingBox/blob/master/Commutateur_tuyaux.3dm

#### Labels
The control board is fitted in an electric box enclosure. The buttons and leds are labelled using stickers designed with this Scribus file.
https://github.com/ChristopheMineau/VacHoseSwitchingBox/blob/master/Scibus_labels.sla
![Gear cutting 2](https://github.com/ChristopheMineau/VacHoseSwitchingBox/blob/master/Pictures/HoseSwitchingBox_17.jpg)

#### Gcode folder
This folder contains the gcode to cut the wooden gears with a CNC. As my Cnc bed is too small for cutting the full size crown, I have divided this task into 4 segments and rotated the cut part three times.
![Gear cutting 1](https://github.com/ChristopheMineau/VacHoseSwitchingBox/blob/master/Pictures/HoseSwitchingBox_02.jpg)
![Gear cutting 2](https://github.com/ChristopheMineau/VacHoseSwitchingBox/blob/master/Pictures/HoseSwitchingBox_03.jpg)

### Firmware folder
This is the Arduino code, I used an Arduino Nano like board.
Beware this project is not built using the standard Arduino IDE, but using Visual Studio Code and the PlatformIO extension. (see the numerous tutorials on the Web about that)

### PCB folder
This folder contains the fritzing file with the board schematics and PCB design.
The folder contains also the gerber generated files, the Flatcam project files and as a result the gcode in order to mill this PCB on the cnc.
![Gear cutting 2](https://github.com/ChristopheMineau/VacHoseSwitchingBox/blob/master/Pictures/HoseSwitchingBox_13.jpg)
![Gear cutting 2](https://github.com/ChristopheMineau/VacHoseSwitchingBox/blob/master/Pictures/HoseSwitchingBox_14.jpg)

### Spec file
See file https://github.com/ChristopheMineau/VacHoseSwitchingBox/blob/master/Spec_connections.TXT

It contains the Arduino pins used for this project and the cabling informations.

