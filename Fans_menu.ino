void fans_menu(){
  menu.hide();
  fans_template();

  bool fans_menu_exit = true;         // boolean to exit fans menu
  bool fan_speed_selection = false;  
  bool prev_on_off_state = false;         // records state of enable boolean in previous loop to prevent spamming messages
  int fan_speed = 0;
  int prev_fanspeed;
  int bit_val = 0;
  bool fan_on_off = false; 
  on_off_display(fan_on_off, 6, 0);
  lcd.setCursor(7,1);
  lcd.print(fan_speed);
  lcd.print("%"); 

  while (fans_menu_exit == true)      // setpoint menu while loop
  {
    switch (fan_speed_selection)
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
      if (fan_speed_selection == false){
        if(fan_on_off == false) {
          fan_on_off = true;
        }
        else {
          fan_on_off = false;
        }
        on_off_display(fan_on_off, 6, 0);
        break;
      }
      else {
        fan_speed = keypad_entry(0, false, 3, 7, 1, 1, false);
        //Serial.println(fan_speed);
        if(fan_speed > 100) {fan_speed = 100;} // drop entered setpoint to 60 degrees if over 60
        bit_val = 255*((float)fan_speed/100.0);
        Serial.println(bit_val);
        lcd.setCursor(7,1);
        lcd.print(fan_speed);
        lcd.print("%"); 
        break;
      }
      case '2': fan_speed_selection = false; break;
      case '8': fan_speed_selection = true; break;
      case '6': if (fan_speed_selection == false) {fan_on_off = true; on_off_display(fan_on_off, 6, 0); break;}
      case '4': if (fan_speed_selection == false) {fan_on_off = false; on_off_display(fan_on_off, 6, 0); break;}
      case '*': fans_menu_exit = false; break;
      default: break;
    }

    if (prev_on_off_state != fan_on_off || prev_fanspeed != fan_speed){
      if (fan_on_off == true) {
        //Serial.println(fan_speed);
      send_serial_command("F", bit_val, NULL);
      }
      if (fan_on_off == false) {
        send_serial_command("F", 0, NULL);
      }
    }

    prev_fanspeed = fan_speed;
    prev_on_off_state = fan_on_off;
  }
  send_serial_command("F", 0, NULL);
  menu.show();
}