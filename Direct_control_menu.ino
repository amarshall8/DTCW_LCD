void direct_control_menu(){
  menu.hide();
  direct_control_template();

  bool dc_menu_exit = true;         // boolean to exit fans menu
  bool dc_pwm_selection = false;  
  bool prev_on_off_state = false;         // records state of enable boolean in previous loop to prevent spamming messages
  int dc_pwm = 0;
  int prev_pwm;
  bool dc_on_off = false; 
  on_off_display(dc_on_off, 10, 0);
  lcd.setCursor(11,1);
  lcd.print(dc_pwm);

  while (dc_menu_exit == true)      // setpoint menu while loop
  {
    switch (dc_pwm_selection)
    {
      case false: 
      lcd.setCursor(0,0);
      lcd.print(char(0x7E));
      lcd.setCursor(0,1);
      lcd.print(" ");
      break;
      case true:
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
      if (dc_pwm_selection == false){
        if(dc_on_off == false) {
          dc_on_off = true;
        }
        else {
          dc_on_off = false;
        }
        on_off_display(dc_on_off, 10, 0);
        break;
      }
      else {
        dc_pwm = keypad_entry(0, true, 3, 11, 1, 1, false);
        //Serial.println(dc_pwm);
        if(dc_pwm > 255) {dc_pwm = 255;} // drop entered setpoint to 60 degrees if over 60
        if(dc_pwm < -255) {dc_pwm = -255;}
        lcd.setCursor(11,1);
        lcd.print(dc_pwm);
        break;
      }
      case '2': dc_pwm_selection = false; break;
      case '8': dc_pwm_selection = true; break;
      case '6': if (dc_pwm_selection == false) {dc_on_off = true; on_off_display(dc_on_off, 10, 0); break;}
      case '4': if (dc_pwm_selection == false) {dc_on_off = false; on_off_display(dc_on_off, 10, 0); break;}
      case '*': dc_menu_exit = false; break;
      default: break;
    }

    if (prev_on_off_state != dc_on_off || prev_pwm != dc_pwm){
      if (dc_on_off == true) {
        //Serial.println(dc_pwm);
      send_serial_command("DC", dc_pwm, NULL);
      }
      if (dc_on_off == false) {
        send_serial_command("DC", 0, NULL);
      }
    }

    prev_pwm = dc_pwm;
    prev_on_off_state = dc_on_off;
  }
  send_serial_command("DC", 0, NULL);
  send_serial_command("F", 0, NULL);
  menu.show();
}