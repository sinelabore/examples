SET VM=c:\jdk-11\bin\java.exe

%VM% -cp "..\bin\*" codegen.Main -p ssc -o test test.xml
gcc -I. -I.. -I..\lib\ -Wall -Wpedantic main.c test.c ..\lib\timer.c  ..\lib\fifo.c -o test.exe