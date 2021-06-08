# VacHoseSwitchingBox
A Shop Vacuum Hub Box capable of switching the central shop vac hose to 3 possible machine ports. 
Arduino based control with remote control.

This repo gathers the design files and arduino software for the project, aiming at making a Box to be used in the workshop in order to commute a central Vacuum main hose to up to three different ports for connecting three different machines.
This project is a complement to the Shop Cyclone project that can be seen here https://github.com/ChristopheMineau/CycloneTurbine

The Cyclone Turbine project uses a remote control, and I decided to use the same remote control, which had plenty of leftover buttons available to control this Hose hub.

The Hub is on one side connected to the Cyclone and to the other side connected to three possible machines or shop stands. 
One port is permanently connected to the table saw, an other to the big band saw, and the third one to a 5m flying hose for any use around the shop.
I just have to press buttons P1 / P2 / P3 on the remote control and wait for a few seconds that the desired port is connected to the vacuum.
As an alternative, I can press a faceplate button as well to switch between the ports.
Three leds indicate the current position, and they alternately blink when travelling.
An error condition would be indicated by the three leds blinking at the same time.

## Contents

### Box design
The design is modeled using Rhino 3D.
See https://github.com/ChristopheMineau/VacHoseSwitchingBox/blob/master/Commutateur_tuyaux.3dm

The principle 

### Labels
The control board is enclosed in an electric box enclosure. The buttons and leds are labelled using stickers designed with this Scribus file.
https://github.com/ChristopheMineau/VacHoseSwitchingBox/blob/master/Scibus_labels.sla

### Spec
See file

