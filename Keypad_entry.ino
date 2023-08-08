///// keypad_entry function /////
// default_entry sets a default value to leave behind if there's no characters entered
// negative_en allows the 0 key to act as a button to input negative values for the first key press
// character_limit sets the max number of characters (excluding '-') that can be typed (not zero indexed)
// lcd_pos sets the column that typing will begin at (zero indexed)
// lcd_row sets the row that typing will behin at (zero indexed)
// extra_clrspace adds extra spots after the end of where typing would normally end to clear in case there are extra characters there from external print statements

int keypad_entry(int default_entry, bool negative_en, int character_limit, int lcd_pos, int lcd_row, int extra_clrspace, bool setpoint_menu){
  character_limit = character_limit - 1;                                                          // subtract 1 from the character limit to make it non zero indexed
  for(int i = 0; i <= character_limit + extra_clrspace; i++){                                     // blank out any filled in space where characters were before, + extra spaces in case int conversion to float or special characters were added
    lcd.setCursor(lcd_pos + i, lcd_row);                                                          // set the cursor each loop in a spot 1 higher than before
    lcd.print(" ");                                                                               // print a blank space to clear the region of the LCD
  }
  lcd.setCursor(lcd_pos, lcd_row);                                                                // set LCD cursor for typing at the appropriate spot
  lcd.blink();                                                                                    // enable blinking of LCD cursor on the correct typing spot
  int typed_value = 0;                                                                            // entered integer value (returned from char conversion after loop)
  int typ_pos = 0;                                                                                // integer position of current typed character
  char str_set_int[character_limit + 1] = {0};                                                    // zero char array of typed characters - sized based on character limit + 1 space for null terminator
  while(true){                                                                                    // keypad typing loop
  char key = keypad.getKey();                                                                     // recieve keypad input from physical device
  if (key && (typ_pos <= character_limit)){                                                       // if key is pressed and the char array is not full go on to evaluate options for adding extra char to array
    if (key == '*' || key == '#') {str_set_int[typ_pos] = '\0'; break;} // exit loop on press of any non-integer key excluding '#' in negative mode
    if (negative_en == true && key == '0' && typ_pos == 0) {key = '-'; character_limit += 1;}     // if negative typing is enabled and the key is '#' convert it to '-'
    if (key != '#'){                                                                              // if key is not an invalid option for typing (* is handled by earlier break statement)
      str_set_int[typ_pos] = key;                                                                 // set current char array value to the pressed key 
      lcd.setCursor(lcd_pos + typ_pos, lcd_row);                                                  // set LCD cursor at appropriate place
      lcd.print(key);                                                                             // print the current typed value
      typ_pos += 1;                                                                               // increment typing position counter
    }
  }
  if (typ_pos == character_limit + 1 && (key == '*' || key == '#')){    // ends typing if character limit is reached and the end key is pressed (* or # for - mode, * only for - mode)
    str_set_int[typ_pos] = '\0';                                                                  // add null terminator to the char array for atoi conversion
    break;                                                                                        // end while loop to enter char array processing and function return
  }
  if (setpoint_menu == true){
     recvWithStartEndMarkers();

    if (strcmp("T", commandFromTCU) == 0 && integerFromTCU == 4){
    float rt_temp = floatFromTCU;

    rt_display(rt_temp);
    lcd.setCursor(lcd_pos + typ_pos, lcd_row);
    }
  }
  }
  lcd.noBlink();                                                                                  // disables blinking of the cursor in case it already is
  typed_value = atoi(str_set_int);                                                                // convert char string to integer value
  if(typ_pos == 0 || (typ_pos == 1 && str_set_int[0] == '-')) {typed_value = default_entry;}      // if no setpoint entered or just negative entered, output default setpoint on enter press
  return typed_value;                                                                             // return the typed integer value
}