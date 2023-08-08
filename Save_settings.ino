void save_settings(){
  send_serial_command("S", 1, NULL);
  menu.hide();
  lcd.setCursor(1,0);
  lcd.print("Saved Settings");
  delay(2000);
  menu.back();
  menu.up();
  menu.show();
}