/*
Serial communications protocol:
<Command, Int, Float>

Commands to control board from LCD:
TON   -> Enables/Disables the temperature datastream (1/0 on int) int = 2 starts 3 part datastream for each sensor independently
EN    -> Enables/Disables temperature control (1/0 on int) and sets setpoint with float value
P     -> Updates Proportional parameter for PID and saves it on temp control arduino if int = 1
I     -> Updates Integral parameter for PID and saves it on temp control arduino if int = 1
D     -> Updates Derivative parameter for PID and saves it on temp control arduino if int = 1
T1CAL -> Updates float offset for Upper temperature sensor 1 if int = 1
T2CAL -> Updates float offset for Upper temperature sensor 2 if int = 1
T3CAL -> Updates float offset for Lower temperature sensor 1 if int = 1
CRCV  -> Requests current calibration values
PRCV  -> Requests current PID values
F     -> Manual fan control, int sets speed value from 0-255 scaled by 0-100 on keypad input
DC    -> Manual control of mosfet through PWM (-255 <-> +255 on int channel)
S     -> Updates (saves) all EEPROM settings if int = 1

Recieved commands from control board to LCD;
T     -> Value of temperature sensors int 1-3 sets temperature sensors 1-3 and int 4 provides average
P     -> Used to update Proportional menu item if int = 0
I     -> Used to update Integral menu item if int = 0
D     -> Used to update Derivative menu item if int = 0
T1CAL -> Used to update Temperature sensor 1 calibration menu item if int = 0
T2CAL -> Used to update Temperature sensor 1 calibration menu item if int = 0
T3CAL -> Used to update Temperature sensor 1 calibration menu item if int = 0
CALF  -> Calibration sending done (int = 0)
PIDF  -> PID sending done (int = 0)
TCUR  -> Float val of current temperature (average of cold plate sensors)
*/

void send_serial_command(char* command, int iVal, float fVal){ //generates commands of <MSG,Int,Float> form to send to the temperature controller based on inputs
  char output[22];
  char char_int_rep[6];
  char char_float_rep[10];
  sprintf(char_int_rep, "%d", iVal);
  dtostrf(fVal, 5, 2, char_float_rep);
  strcpy(output, "<");
  strcat(output, command);
  strcat(output, ",");
  strcat(output, char_int_rep);
  strcat(output, ",");
  strcat(output, char_float_rep);
  strcat(output, ">");
  tempBoard.println(output);
}

bool heartbeat_recieved(){
  bool conn_good = false;
  send_serial_command("H", 1, NULL);
  delay(100);
  recvWithStartEndMarkers();
  if (strcmp(commandFromTCU, "H") == 0 && integerFromTCU == 2){
    conn_good = true;
  }
  else{
    conn_good = false;
  }
  return conn_good;
}