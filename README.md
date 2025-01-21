# DTCW LCD
LCD code for the Digital Temperature Controlled Worksurface\
Sends commands to TCU (Temperature control unit) from the user front panel over UART to start, stop, update settings, and control the temperature setpoint of the DTCW\
This split architecture is primarily to take computational load off of the TCU and avoid issues of the control loop being broken by menu code.
