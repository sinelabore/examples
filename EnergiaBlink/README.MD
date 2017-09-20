Preconditions:
- installed Astah UML Tool to change the state diagram
- installed Java VM

To create code from the Astah model call: make -f SCMakefile
If you don't have installed 'make' look into the SCMakefile and
call the codegenerator yourself.

The following files can be reused in further projects
to have a basic timer and queue (fifo) service available.
timer.c / timer.h
fifo.c / fifo.h

