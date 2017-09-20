Energia (= Arduino for Texas Instruments processors) demo

 The program demonstrates how to use code generated from Sinelabore with Energia.

 When the program starts the LED1 blinks with 1s cycle time
 S2 allows to switch to a faster cycle time and back again
 S1 allows to enter a state where the LED1 is on or off (see state diagram).

 This demo shows that is is very easy to realize complex behaviour 
 which is easy to understand on state diagram level - but not so easy
 anymore on code level. AND: if you changes something - just regenerate
 the state machine code - no hand coding needed!!!

Preconditions:
- Installed Astah UML Tool to change the state diagram. If you want to edit the state diagram check the path to the Astah installation and update it.
- Installed Java VM
- Download sinelabore and copy bin folder into his directory.

To create code from the Astah model call: make -f SCMakefile If you don't have installed 'make' look into the SCMakefile and call the codegenerator manually.

The following files can be reused in further projects to have a basic timer and queue (fifo) service available.
- timer.c / timer.h
- fifo.c / fifo.h

