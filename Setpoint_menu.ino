void setpoint_menu(int default_setpoint, bool enable_on_call){
  menu.hide();
  // Create custom LCD characters
  lcd.createChar(3, hot_symbol1);
  lcd.createChar(4, hot_symbol2);
  lcd.createChar(5, thermometer1);
  lcd.createChar(6, thermometer2);
  lcd.createChar(7, thermometer3);
  
  bool setpoint_menu_exit = true;         // boolean to exit setpoint menu
  bool en_selection = false;              // is the enable option selected?
  bool en_on_off = enable_on_call;        // on off toggle for temperature control (passed to backend over uart) form <E,0,0>
  bool prev_on_off_state = false;         // records state of enable boolean in previous loop to prevent spamming messages
  int setpoint = default_setpoint;        // setpoint variable (passed to backend over UART) form <S,(setpoint),0>
  int prev_setpoint;
  float rt_temp = 0;
  
  setpoint_template();                    // draw template for lcd setpoint menu
  print_setpoint(default_setpoint, 5);    // print default setpoint value (5 is int position on screen)
  on_off_display(en_on_off, 11, 1);              // print default off state for display

  send_serial_command("TON", 1, NULL);

  oldTime = millis() / 100;
  int last_msg_time = 0;

  while (setpoint_menu_exit == true)      // setpoint menu while loop
  {

    recvWithStartEndMarkers();

    int currentTime = millis() / 100;
    int deltaTime = currentTime - oldTime;
    oldTime = currentTime;

    if(last_msg_time >= 100){
      tcuConnGood = false;
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("TCU CON Failed");
      delay(1500);
      goto exit_setpoint_menu;
    }

    if (strcmp("T", commandFromTCU) == 0 && integerFromTCU == 2){
      rt_temp = floatFromTCU;
      last_msg_time = 0;
    }
    else{
      last_msg_time += deltaTime;
    }

    if (en_selection == false){           // print right arrow on upper row for
      lcd.setCursor(0,1);
      lcd.print(" ");
      lcd.setCursor(0,0);
      lcd.print(char(0x7E));
    }
    else{
      lcd.setCursor(0,0);
      lcd.print(" ");
      lcd.setCursor(0,1);
      lcd.print(char(0x7E));
    }
    
    if (rt_temp < -10){
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("Invalid Temp");
      lcd.setCursor(2,1);
      lcd.print("Sensor  Data");
      delay(3000);
      goto exit_setpoint_menu;
    }

    char key = keypad.getKey();
    switch (key)
    {
      // set which key does what action in menu mode
      case '#':
      if (en_selection == true){
        if(en_on_off == false) {
          en_on_off = true;
        }
        else {
          en_on_off = false;
        }
        on_off_display(en_on_off, 11, 1);
        break;
      }
      else {
        setpoint = keypad_entry(default_setpoint, false, 2, 5, 0, 1, true);
        oldTime = millis() / 100;
        if(setpoint > 60) {setpoint = 60;} // drop entered setpoint to 60 degrees if over 60
        print_setpoint(setpoint, 5); 
        break;
      }
      case '2': en_selection = false; break;
      case '8': en_selection = true; break;
      case '*': setpoint_menu_exit = false; break;
      default: break;
    }

    if (prev_on_off_state != en_on_off || prev_setpoint != setpoint){
      if (en_on_off == true) {
      send_serial_command("EN", 1, setpoint);
      }
      if (en_on_off == false) {
        send_serial_command("EN", 0, NULL);
      }
    }
    
    prev_setpoint = setpoint;
    prev_on_off_state = en_on_off;

    rt_display(rt_temp);
  }
  exit_setpoint_menu:
  send_serial_command("TON", 0, NULL);
  send_serial_command("EN", 0, NULL);
  menu.show();
}