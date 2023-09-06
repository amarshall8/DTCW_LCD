/*//////////////////////////////
      Display Templates
//////////////////////////////*/

void setpoint_template(){
  lcd.setCursor(1,0);
  lcd.print("Set:");
  lcd.setCursor(10,0);
  lcd.print(":");
  lcd.setCursor(1,1);
  lcd.print("Temp Ctrl:");
}

void calibration_template(){
  lcd.setCursor(1,0);
  lcd.print("NT:");
  lcd.setCursor(1,1);
  lcd.print("NB:");
}

void PID_template(){
  lcd.setCursor(1,0);
  lcd.print("P:");
  lcd.setCursor(9,0);
  lcd.print("I:");
  lcd.setCursor(1,1);
  lcd.print("D:");
}

void sensor_temps_template(){
  lcd.setCursor(0,0);
  lcd.print("NT:");
  lcd.setCursor(0,1);
  lcd.print("NB:");
}

void fans_template(){
  lcd.setCursor(1,0);
  lcd.print("Fans:");
  lcd.setCursor(1,1);
  lcd.print("Speed:");
}

void direct_control_template(){
  lcd.setCursor(1,0);
  lcd.print("H-Bridge:");
  lcd.setCursor(1,1);
  lcd.print("PWM Value:");
}

/*///////////////////////////
    LCD Print Functions
///////////////////////////*/

void print_setpoint(int setpoint_val, int lcd_pos){
  lcd.setCursor(lcd_pos,0); //set cursor and clear typed entry (fixes bad inputs)
  lcd.print("   ");
  lcd.setCursor(lcd_pos,0); //set cursor and print int entry
  lcd.print(setpoint_val);
  lcd.print(char(223)); //print degree symbol
}

void rt_display(float temp_rt){
  //lcd.setCursor(10,0);
  //lcd.print("     ");
  lcd.setCursor(9,0);
  if (temp_rt > 37) {
      lcd.write(byte(7));
      int currentTime = millis() / 500;
      int deltaTime = currentTime - oldTime2;
      oldTime2 = currentTime;
      if (deltaTime >= 1){
        //Serial.println(hot_flash);
        lcd.setCursor(8,0);
        if (hot_flash == false) {hot_flash = true; lcd.write(byte(4));}
        else {hot_flash = false; lcd.write(byte(3));}
        deltaTime = 0;

      }
  }
  else if (temp_rt >= 15) {lcd.write(byte(6)); lcd.setCursor(8,0); lcd.write(" ");}
  else if (temp_rt >= 0) {lcd.write(byte(5)); lcd.setCursor(8,0); lcd.write(" ");}
  lcd.setCursor(11,0);
  lcd.print(temp_rt, 1);
  lcd.print(char(223));
  lcd.print(" ");
}

void on_off_display(bool on_off, int lcd_col, int lcd_row){
  lcd.setCursor(lcd_col, lcd_row);
  lcd.print("   ");
  lcd.setCursor(lcd_col, lcd_row);
  if(on_off == true){
    lcd.print("ON");
  }
  else{
    lcd.print("OFF");
  }
}
