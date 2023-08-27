//Set LCD library to 4-bit input mode
#define USE_STANDARD_LCD
// include the necessary external libraries:
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <Keypad.h>

//LCD specific libraries
#include <ItemInput.h>
#include <ItemCommand.h>
#include <ItemToggle.h>
#include <ItemSubMenu.h>
#include <LcdMenu.h>

#define LCD_ROWS 2
#define LCD_COLS 16

// initialize the liquidrystal library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

/*/////////////////////////////
        LCD Menu Setup
*//////////////////////////////

void setpoint_4C();
void setpoint_37C();
void setpoint_25C();
void save_settings();
void PID_menu();
void fans_menu();
void direct_control_menu();
void sensor_temps_menu();
void calibration_menu();
void(* resetFunc) (void) = 0; //declare reset function at address 0

extern MenuItem* settingsMenu[];
extern MenuItem* setpointMenu[];
extern MenuItem* diagMenu[];


char low_temp[] = {'4',223,'C','\0'};       //Create 4C character array with degree symbol
char high_temp[] = {'3','7',223,'C','\0'};  //Create 37C character array with degree symbol

// Define the main menu
MAIN_MENU(
    ITEM_SUBMENU("Setpoint Mode", setpointMenu),
    ITEM_SUBMENU("Settings", settingsMenu),
    ITEM_SUBMENU("Diagnostics", diagMenu)
);


// Setpoint submenu
SUB_MENU(setpointMenu, mainMenu,
    ITEM_COMMAND(low_temp, setpoint_4C),
    ITEM_COMMAND(high_temp, setpoint_37C),
    ITEM_COMMAND("Manual Control", setpoint_25C)
);


// Settings submenu
SUB_MENU(settingsMenu, mainMenu,
    ITEM_COMMAND("Temp Cal", calibration_menu),
    ITEM_COMMAND("PID Tuning", PID_menu),
    ITEM_COMMAND("Save Settings", save_settings)
);

// Diagnosic submenu
SUB_MENU(diagMenu, mainMenu,
    ITEM_COMMAND("Sensor Temps", sensor_temps_menu),
    ITEM_COMMAND("Fans", fans_menu),
    ITEM_COMMAND("Direct Control", direct_control_menu)
);

LcdMenu menu(LCD_ROWS, LCD_COLS); //Create LCDMenu object that can be called

/*////////////////////////////////
        Custom Characters
*/////////////////////////////////

//Low temperature thermometer symbol
byte thermometer1[8] = {
  0b00100,  
  0b01010,
  0b01010,
  0b01010,
  0b01010,
  0b11011,
  0b01110,
  0b00000
};

//Medium temperature thermometer symbol
byte thermometer2[8] = {
  0b00100,  
  0b01010,
  0b01010,
  0b01010,
  0b01110,
  0b11111,
  0b01110,
  0b00000
};

//High temperature thermometer symbol
byte thermometer3[8] = {
  0b00100,  
  0b01010,
  0b01110,
  0b01110,
  0b01110,
  0b11111,
  0b01110,
  0b00000
};

//Hot plate symbol first animation frame
byte hot_symbol1[8] = {
  0B00000,
  0B10010,
  0B01001,
  0B10010,
  0B01001,
  0B00000,
  0B11111,
  0B00000
};

//Hot plate symbol second animation frame
byte hot_symbol2[8] = {
  0B00000,
  0B01001,
  0B10010,
  0B01001,
  0B10010,
  0B00000,
  0B11111,
  0B00000
};

/*////////////////////////////////
          Keypad Setup
*/////////////////////////////////

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {7, 6, 5, 4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {16, 15, 14}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//global variables for delta time and LCD flashing
unsigned long oldTime = 0;
bool hot_flash = false;
bool tcuConnGood = false;

SoftwareSerial tempBoard(2, 3); // RX, TX, Creates tempBoard serial object for communication to 2nd arduino

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing

      // variables to hold the parsed data
char commandFromTCU[numChars] = {0};
int integerFromTCU = 0;
float floatFromTCU = 0.0;


void setup() {
  lcd.begin(16, 2);             // set up the LCD's number of columns and rows:
  lcd.setCursor(3,0);           // Print out startup screen
  lcd.print("Flugen Inc.");
  lcd.setCursor(6,1);
  lcd.print("DTCW");
  delay(2000);
  Serial.begin(57600);          // Instantiates serial communication over USB serial connector
  tempBoard.begin(4800);        // Instantiates serial communication to the second arduino board which controls temp and fans
  Serial.println("Online");     // Print on USB Serial that the board is turned on
  tcuConnGood = heartbeat_recieved();
  if (tcuConnGood == true){
    menu.setupLcdWithMenu(13, 12, 11, 10, 9, 8, mainMenu);  // Creates menu, erasing content on the screen while showing the interactive menu system
  }
}

void loop() {
  if(tcuConnGood == false) {
    lcd.clear();
    delay(500);
    lcd.setCursor(2,0);
    lcd.print("TCU  OFFLINE");
    lcd.setCursor(0,1);
    lcd.print("SERVICE REQUIRED");
    delay(500);
    tcuConnGood = heartbeat_recieved();
    if(tcuConnGood == true){
      resetFunc(); //call reset 
    }
  }
  else {
    char key = keypad.getKey();
    switch (key)
    {
      // set which key does what action in menu mode to control cursor movement
      case '#': menu.enter(); break;
      case '2': menu.up();    break;
      case '8': menu.down();  break;
      case '4': menu.left();  break;
      case '6': menu.right(); break;
      case '*': menu.back();  break;
      default: break;
    }
  }
}

// Silly functions that pass default values and automatic enable signals to the setpoint_menu function
void setpoint_4C(){
  setpoint_menu(4,true);
}

void setpoint_37C(){
  setpoint_menu(37,true);
}

void setpoint_25C(){
  setpoint_menu(25,false);
}
