#include <EasyBuzzer.h>
#include <AsyncTaskLib.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

#define PIN_BUZZER 6
#define MAX_TRYS 3


const byte ROWS = 4;  // rows
const byte COLS = 4;  // columns
//define the symbols on the buttons of the keypads
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
byte rowPins[ROWS] = { 11, 10, 9, 8 };  //connect to the row pinouts of the keypad
byte colPins[COLS] = { 5, 4, 3, 2 };    //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
int_fast8_t trys = 0;
String readString() {
  lcd.setCursor(0, 1);

  lcd.print("                ");
  lcd.setCursor(0, 1);

  String strNumber = "";
  char readedChar;
  while (true) {
    actualizarTareas();
    char key = keypad.getKey();
    if (key) {
      if (key == '*') {
        break;  // clear input
      } else if (isAlpha(key)) {
        continue;
      } else if (isDigit(key)) {  // only act on numeric keys
        strNumber += key;         // append new character to input string
        lcd.print(key);
      }
    }
  }

  return strNumber;
}

void decisionPassword(bool correctPass) {
  if(correctPass){

  }else if(!correctPass){
    trys++;
  }
  if(trys > MAX_TRYS){

  }
}
void esperarCincoSegundos(){

}
void actualizarTareas() {
}


void setup() {
  // put your setup code here, to run once:
}

void loop() {
  actualizarTareas();
}
