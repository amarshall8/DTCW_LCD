void calibration_menu(){
  menu.hide();
  calibration_template();

  bool calibration_menu_exit = true;         // boolean to exit setpoint menu
  bool cal_recieved = false;
  int sensor_selection = 1;                  // which sensor is selected
  int s1_entered_val = 0;
  int s2_entered_val = 0;
  int s3_entered_val = 0;
  float s1_trans_val = 0;
  float s2_trans_val = 0;
  float s3_trans_val = 0;

  send_serial_command("CRCV", 1, NULL);



  while (calibration_menu_exit == true)      // setpoint menu while loop
  {
    oldTime = millis() / 750;
    while(cal_recieved == false){
      int currentTime = millis() / 750;
      int deltaTime = currentTime - oldTime;
      oldTime = currentTime;
      if(deltaTime < 1){
      recvWithStartEndMarkers();
        if (strcmp("T1CAL", commandFromTCU) == 0 && integerFromTCU == 0){
          lcd.setCursor(3,0);
          lcd.print(floatFromTCU, 2);
        }
        else if (strcmp("T2CAL", commandFromTCU) == 0 && integerFromTCU == 0){
          lcd.setCursor(11,0);
          lcd.print(floatFromTCU, 2);
        }
        else if (strcmp("T3CAL", commandFromTCU) == 0 && integerFromTCU == 0){
          lcd.setCursor(3,1);
          lcd.print(floatFromTCU, 2);
        }
        else if (strcmp("CALF", commandFromTCU) == 0 && integerFromTCU == 0){
          cal_recieved = true;
        }
      }
      else {
        tcuConnGood = false;
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("TCU CON Failed");
        delay(3000);
        goto exit_cal_menu;
      }
    }
    switch (sensor_selection)
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
        switch (sensor_selection)
        {
          case 1: 
            s1_entered_val = keypad_entry(s1_entered_val, true, 3, 3, 0, 2, false); 
            s1_trans_val = s1_entered_val/100.0;
            send_serial_command("T1CAL", 1, s1_trans_val);
            lcd.setCursor(3,0);
            lcd.print(s1_trans_val,2);
            break;
          case 2: 
            s2_entered_val = keypad_entry(s2_entered_val, true, 3, 11, 0, 2, false); 
            s2_trans_val = s2_entered_val/100.0;
            send_serial_command("T2CAL", 1, s2_trans_val);
            lcd.setCursor(11,0);
            lcd.print(s2_trans_val,2);
            break;
          case 3: 
            s3_entered_val = keypad_entry(s3_entered_val, true, 3, 3, 1, 2, false); 
            s3_trans_val = s3_entered_val/100.0;
            send_serial_command("T3CAL", 1, s3_trans_val);
            lcd.setCursor(3,1);
            lcd.print(s3_trans_val,2);
            break;
          default: break;
        }
        break;
      case '2': 
        if (sensor_selection == 3){
          sensor_selection = 1; 
          }
        break;
      case '8': 
        if (sensor_selection == 1 || sensor_selection == 2){
          sensor_selection = 3;
        } 
        break;
      case '6':
        if (sensor_selection == 1 || sensor_selection == 3){
          sensor_selection = 2;
        } 
        break;
      case '4':
        if (sensor_selection == 2){
          sensor_selection = 1;
        } 
        break;
      case '*': 
        calibration_menu_exit = false;
        break;
      default: break;
    }
  }
  exit_cal_menu:
  menu.show();
}