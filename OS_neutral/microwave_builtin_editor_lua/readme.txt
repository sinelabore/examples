Example realizing a microwave oven in lua. 
More info is available here: https://www.sinelabore.de/doku.php/wiki/news/14feb2020

To generate the code calling "generate.bat" you need java installed on your PC
To edit the state machine project calling "edit.bat" you need java and graphviz installed on your PC
Adjust the path to the codegen bin folder according your installation. 

Execute the sample program by calling
lua54.exe main.lua

Output looks as follows:

oven off
---
timer_inc
oven off
---
timer_inc
oven off
---
set pwr =high
oven off
---
timer_preset
timer_start
oven on
---
oven off
timer clear
---




