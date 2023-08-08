void PID_menu(){
  menu.hide();
  PID_template();

  bool PID_menu_exit = true;         // boolean to exit setpoint menu
  bool PID_val_received = false;
  int PID_selection = 1;                  // which sensor is selected
  int P_entered_val = 0;
  int I_entered_val = 0;
  int D_entered_val = 0;
  float P_trans_val = 0;
  float I_trans_val = 0;
  float D_trans_val = 0;

  send_serial_command("PRCV", 1, NULL);

  while (PID_menu_exit == true)      // setpoint menu while loop
  {
    int currentTime = millis() / 750;
    oldTime = currentTime;
    while(PID_val_received == false){
      int currentTime = millis() / 750;
      int deltaTime = currentTime - oldTime;
      oldTime = currentTime;
      if(deltaTime < 1){
        recvWithStartEndMarkers();
        if (strcmp("P", commandFromTCU) == 0 && integerFromTCU == 0){
          lcd.setCursor(3,0);
          lcd.print(floatFromTCU, 1);
        }
        else if (strcmp("I", commandFromTCU) == 0 && integerFromTCU == 0){
          lcd.setCursor(11,0);
          lcd.print(floatFromTCU, 1);
        }
        else if (strcmp("D", commandFromTCU) == 0 && integerFromTCU == 0){
          lcd.setCursor(3,1);
          lcd.print(floatFromTCU, 1);
        }
        else if (strcmp("PIDF", commandFromTCU) == 0 && integerFromTCU == 0){
          PID_val_received = true;
        }
      }
      else {
        tcuConnGood = false;
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("TCU CON Failed");
        delay(1500);
        goto exit_pid_menu;
      }
    }
    switch (PID_selection)
    {
      case 1: 
      lcd.setCursor(0,0);
      lcd.print(char(0x7E));
      lcd.setCursor(8,0);
      lcd.print(" ");
      lcd.setCursor(0,1);
      lcd.print(" ");
      break;
      case 2:
      lcd.setCursor(0,0);
      lcd.print(" ");
      lcd.setCursor(8,0);
      lcd.print(char(0x7E));
      lcd.setCursor(0,1);
      lcd.print(" ");
      break;
      case 3:
      lcd.setCursor(0,0);
      lcd.print(" ");
      lcd.setCursor(8,0);
      lcd.print(" ");
      lcd.setCursor(0,1);
      lcd.print(char(0x7E));
      break;
      default: break;
    }
    char key = keypad.getKey();
    switch (key)
    {
      // set which key does what action in menu mode
      case '#':
        switch (PID_selection)
        {
          case 1: 
            P_entered_val = keypad_entry(P_entered_val, false, 4, 3, 0, 2, false); 
            P_trans_val = P_entered_val/10.0;
            send_serial_command("P", 1, P_trans_val);
            lcd.setCursor(3,0);
            lcd.print(P_trans_val,2);
            break;
          case 2: 
            I_entered_val = keypad_entry(I_entered_val, false, 4, 11, 0, 2, false); 
            I_trans_val = I_entered_val/10.0;
            send_serial_command("I", 1, I_trans_val);
            lcd.setCursor(11,0);
            lcd.print(I_trans_val,2);
            break;
          case 3: 
            D_entered_val = keypad_entry(D_entered_val, false, 4, 3, 1, 2, false); 
            D_trans_val = D_entered_val/10.0;
            send_serial_command("D", 1, D_trans_val);
            lcd.setCursor(3,1);
            lcd.print(D_trans_val,2);
            break;
          default: break;
        }
        break;
      case '2': 
        if (PID_selection == 3){
          PID_selection = 1; 
          }
        break;
      case '8': 
        if (PID_selection == 1 || PID_selection == 2){
          PID_selection = 3;
        } 
        break;
      case '6':
        if (PID_selection == 1 || PID_selection == 3){
          PID_selection = 2;
        } 
        break;
      case '4':
        if (PID_selection == 2){
          PID_selection = 1;
        } 
        break;
      case '*': 
        PID_menu_exit = false;
        break;
      default: break;
    }
  }
  exit_pid_menu:
  menu.show();
}