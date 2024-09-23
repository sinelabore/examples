@echo off
REM Check if the environment variable CLASSPATH is set
REM Example: set CLASSPATH=c:\sinelaboreRT6.3.4\bin\*
if "%CLASSPATH%"=="" (
    echo CLASSPATH environment variable is not set.
    exit /b 1
)

REM Run the Java command using the CLASSPATH environment variable
java -cp "%CLASSPATH%" codegen.Main -s -p CADIFRA -o learning learning.cdd