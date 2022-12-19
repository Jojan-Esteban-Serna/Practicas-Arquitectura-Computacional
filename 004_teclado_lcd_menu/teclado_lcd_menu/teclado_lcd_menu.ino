#include <LiquidCrystal.h>
#include <LiquidMenu.h>
#include <Keypad.h>

// Pin mapping for the display
const int rs = 7, en = 8, d4 = 22, d5 = 24, d6 = 26, d7 = 28;

//LCD R/W pin to ground
//10K potentiometer wiper to VO
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
char* messages[16] = { "1.UmbTempHigh", "2.UmbTempLow", "3.UmbLuzHigh", "4.UmbLuzLow", "5.Reset" };
#pragma region configuracion_lineas
LiquidLine screen_1_line_1(0, 0, messages[0]);
LiquidLine screen_1_line_2(0, 1, messages[1]);
LiquidScreen screen_1(screen_1_line_1, screen_1_line_2);

LiquidLine screen_2_line_1(0, 0, messages[1]);
LiquidLine screen_2_line_2(0, 1, messages[2]);
LiquidScreen screen_2(screen_2_line_1, screen_2_line_2);

LiquidLine screen_3_line_1(0, 0, messages[2]);
LiquidLine screen_3_line_2(0, 1, messages[3]);
LiquidScreen screen_3(screen_3_line_1, screen_3_line_2);

LiquidLine screen_4_line_1(0, 0, messages[3]);
LiquidLine screen_4_line_2(0, 1, messages[4]);
LiquidScreen screen_4(screen_4_line_1, screen_4_line_2);


LiquidMenu menu(lcd, screen_1, screen_2, screen_3, screen_4);

#pragma endregion

#pragma region teclado
const byte ROWS = 4;  // rows
const byte COLS = 4;  // columns
//define the symbols on the buttons of the keypads
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
byte rowPins[ROWS] = { 5, 4, 3, 2 };     //connect to the row pinouts of the keypad
byte colPins[COLS] = { 11, 10, 9, 13 };  //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
#pragma endregion
void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  menu.update();
}

void loop() {
    char key = keypad.getKey();

    if (key == 'D') {
      menu.next_screen();
    }else if(key == 'A'){
      menu.previous_screen();
    }else if(key == 'B'){
      menu.set_focusedLine(0);
    }else if(key == 'C'){
            menu.set_focusedLine(1);

    }
  
}
