void sensor_temps_menu(){
  menu.hide();
  sensor_temps_template();

  send_serial_command("TON", 2, NULL);

  oldTime = millis() / 100;
  int last_msg_time = 0;

  while(true){
    char key = keypad.getKey();
    if (key == '*') break;

    recvWithStartEndMarkers();

    int currentTime = millis() / 100;
    int deltaTime = currentTime - oldTime;
    oldTime = currentTime;

    if(last_msg_time >= 40){
      tcuConnGood = false;
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("TCU CON Failed");
      delay(3000);
      goto exit_st_menu;
    }  
    
    if (strcmp("T", commandFromTCU) == 0 && integerFromTCU == 1){
      lcd.setCursor(2,0);
      lcd.print(floatFromTCU, 2);
      last_msg_time = 0;
      tcuConnGood = true;
    }
    else if (strcmp("T", commandFromTCU) == 0 && integerFromTCU == 2){
      lcd.setCursor(10,0);
      lcd.print(floatFromTCU, 2);
      last_msg_time = 0;
      tcuConnGood = true;
    }
    else if (strcmp("T", commandFromTCU) == 0 && integerFromTCU == 3){
      lcd.setCursor(2,1);
      lcd.print(floatFromTCU, 2);
      last_msg_time = 0;
      tcuConnGood = true;
    }
    else{
      last_msg_time += deltaTime;
    }
  }
  exit_st_menu:
  send_serial_command("TON", 0, NULL);
  menu.show();
}