void calibration_menu(){
  menu.hide();
  calibration_template();

  bool calibration_menu_exit = true;         // boolean to exit setpoint menu
  bool cal_recieved = false;
  int sensor_selection = 1;                  // which sensor is selected
  int sNT_entered_val = 0;
  int sNB_entered_val = 0;
  float sNT_trans_val = 0;
  float sNB_trans_val = 0;

  send_serial_command("CRCV", 1, NULL);



  while (calibration_menu_exit == true)      // setpoint menu while loop
  {
    oldTime = millis();
    while(cal_recieved == false){
      unsigned long currentTime = millis();
      unsigned long deltaTime = currentTime - oldTime;
      oldTime = currentTime;
      if(deltaTime < 750){
      recvWithStartEndMarkers();
        if (strcmp("TNTCAL", commandFromTCU) == 0 && integerFromTCU == 0){
          lcd.setCursor(4,0);
          lcd.print(floatFromTCU, 2);
        }
        else if (strcmp("TNBCAL", commandFromTCU) == 0 && integerFromTCU == 0){
          lcd.setCursor(4,1);
          lcd.print(floatFromTCU, 2);
        }
        else if (strcmp("CALF", commandFromTCU) == 0 && integerFromTCU == 0){
          cal_recieved = true;
          Serial.println("recieved");
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
      lcd.setCursor(0,1);
      lcd.print(" ");
      break;
      case 2:
      lcd.setCursor(0,0);
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
            sNT_entered_val = keypad_entry(sNT_entered_val, true, 3, 4, 0, 2, false); 
            sNT_trans_val = sNT_entered_val/100.0;
            send_serial_command("TNTCAL", 1, sNT_trans_val);
            lcd.setCursor(4,0);
            lcd.print(sNT_trans_val,2);
            break;
          case 2: 
            sNB_entered_val = keypad_entry(sNB_entered_val, true, 3, 4, 1, 2, false); 
            sNB_trans_val = sNB_entered_val/100.0;
            send_serial_command("TNBCAL", 1, sNB_trans_val);
            lcd.setCursor(4,1);
            lcd.print(sNB_trans_val,2);
            break;
          default: break;
        }
        break;
      case '2': 
        if (sensor_selection == 2){
          sensor_selection = 1; 
          }
        break;
      case '8': 
        if (sensor_selection == 1){
          sensor_selection = 2;
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