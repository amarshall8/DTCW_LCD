void reset_settings(){
  menu.hide();
  lcd.setCursor(1,0);
  lcd.print("Resetting");
  send_serial_command("TNTCAL", 1, 0.0);
  delay(100);
  send_serial_command("TNBCAL", 1, 0.0);
  delay(100);
  send_serial_command("P", 1, 2.0);
  delay(100);
  send_serial_command("I", 1, 5.0);
  delay(100);
  send_serial_command("D", 1, 1.0);
  delay(100);
  send_serial_command("S", 1, NULL);
  delay(500);
  menu.back();
  menu.show();
}