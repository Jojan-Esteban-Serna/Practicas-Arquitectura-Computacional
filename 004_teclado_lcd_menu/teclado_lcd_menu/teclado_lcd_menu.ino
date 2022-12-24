#include <LiquidCrystal.h>
#include <LiquidMenu.h>
#include <Keypad.h>
#include <EasyBuzzer.h>
#define DEFAULT_TEMPHIGH 29
#define DEFAULT_TEMPLOW 26
#define PULSADOR 2        // pulsador en pin 2
#define BUZZER_PASIVO 12  // buzzer pasivo en pin 12


#define LED_RED 46
#define LED_GREEN 44
#define LED_BLUE 45

#define TEST_TEMP 15
#define MAX_TEMP 125
#define MAX_LIGTH 1023
// Pin mapping for the display
const int rs = 7, en = 8, d4 = 22, d5 = 24, d6 = 26, d7 = 28;
int umbrTempHigh = DEFAULT_TEMPHIGH, umbrTempLow = DEFAULT_TEMPLOW, umbrLuzHigh = 800, umbrLuzLow = 300;
//Prototipos funciones


int readNumber();
void editar_valor(String titulo, byte *varimp);
void color(unsigned char red, unsigned char green, unsigned char blue);
bool isInTempRange(int number, int *varimp);
bool isInLightRange(int number, int *varimp);



//LCD R/W pin to ground
//10K potentiometer wiper to VO
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
#pragma region Screens
//char *messages[5] = { "1.UmbTempHigh", "2.UmbTempLow", "3.UmbLuzHigh", "4.UmbLuzLow", "5.Reset" };
char messages[5][16] = { {"1.UmbTempHigh"}, {"2.UmbTempLow"}, {"3.UmbLuzHigh"}, {"4.UmbLuzLow"}, {"5.Reset"} };

LiquidScreen *lastScreen = nullptr;

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

LiquidLine screen_5_line_1(0, 0, "");
LiquidLine screen_5_line_2(0, 1, "");
LiquidScreen screen_5(screen_5_line_1, screen_5_line_2);

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
int readNumber() {
  lcd.setCursor(0, 1);

  lcd.print("                ");
  lcd.setCursor(0, 1);

  String strNumber = "";
  char readedChar;
  while (true) {
    char key = keypad.getKey();
    if (key) {
      if (key == '*') {
        break;  // clear input
      } else if (key > 'A' && key < 'Z') {
        continue;
      } else if (key >= '0' && key <= '9') {  // only act on numeric keys
        strNumber += key;                     // append new character to input string
        lcd.print(key);
      }
    }
  }

  return strNumber.toInt();
}
void color(unsigned char red, unsigned char green, unsigned char blue)  // the color generating function
{
  analogWrite(LED_RED, red);
  analogWrite(LED_BLUE, blue);
  analogWrite(LED_GREEN, green);
}

bool isInTempRange(int number, int *varimp) {
  return ((varimp == &umbrTempLow && number < umbrTempHigh || varimp == &umbrTempHigh && number > umbrTempLow) && number <= MAX_TEMP);
}

bool isInLightRange(int number, int *varimp) {
  return ((varimp == &umbrLuzLow && number < umbrLuzHigh || varimp == &umbrLuzHigh && number > umbrLuzLow) && number <= MAX_LIGTH);
}
void editar_valor(String titulo, int *varimp, bool (*isInRangeFunction)(int, int*)) {
  menu.change_screen(&screen_5);
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 0);
  lcd.print(titulo);
  lcd.setCursor(0, 1);
  lcd.print(*varimp);
  lcd.print(" \"*\" to edit");
  char pressedKey;
  while ((pressedKey = keypad.getKey()) != '*' && pressedKey == NO_KEY && pressedKey != '#' || isAlphaNumeric(pressedKey)) {
  }
  if (pressedKey == '#') {
    menu.change_screen(lastScreen);
    return;
  }
  int number = readNumber();
  if (isInRangeFunction(number, varimp)) {
    *varimp = number;
    menu.change_screen(lastScreen);
    return;
  }

  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("Error press \"*\"");
  while ((pressedKey = keypad.getKey()) != '*' && pressedKey == NO_KEY || pressedKey == '#' || isAlphaNumeric(pressedKey)) {
  }

  menu.change_screen(lastScreen);
}
auto umbTempHighFunc = []() {
  editar_valor("UmbTempHigh", &umbrTempHigh, isInTempRange);
};

auto umbTempLowFunc = []() {
  editar_valor("UmbTempLow", &umbrTempLow, isInTempRange);
};

auto umbLuzHighFunc = []() {
  editar_valor("UmbLuzHigh", &umbrLuzHigh, isInLightRange);
};
auto umbLuzLowFunc = []() {
  editar_valor("UmbLuzLow", &umbrLuzLow, isInLightRange);
};
void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  menu.add_screen(screen_5);
  pinMode(PULSADOR, INPUT_PULLUP);  // pin 2 como entrada con resistencia de pull-up
  pinMode(BUZZER_PASIVO, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  EasyBuzzer.setPin(BUZZER_PASIVO);

  //screen_2_line_1.attach_function(1,)
  screen_1_line_1.attach_function(1, umbTempHighFunc);
  screen_1_line_2.attach_function(1, umbTempLowFunc);

  screen_2_line_1.attach_function(1, umbTempLowFunc);
  screen_2_line_2.attach_function(1, umbLuzHighFunc);

  screen_3_line_1.attach_function(1, umbLuzHighFunc);
  screen_3_line_2.attach_function(1, umbLuzLowFunc);

  screen_4_line_1.attach_function(1, umbLuzLowFunc);
  screen_4_line_2.attach_function(1, []() {
    menu.change_screen(&screen_5);

    lcd.setCursor(0, 0);
    lcd.print("                ");

    lcd.setCursor(0, 0);
    lcd.print("\"*\" to confirm ");
    lcd.setCursor(0, 1);
    lcd.print("\"#\" to cancel  ");
    char pressedKey;
    while ((pressedKey = keypad.getKey()) != '*' && pressedKey == NO_KEY && pressedKey != '#' || isAlphaNumeric(pressedKey)) {
    }
    if (pressedKey == '#') {
      menu.change_screen(lastScreen);
      return;
    }
    umbrTempHigh = DEFAULT_TEMPHIGH;
    umbrTempLow = DEFAULT_TEMPLOW;
    menu.change_screen(lastScreen);
  });
  menu.update();
}

void loop() {
  EasyBuzzer.update();

  char key = keypad.getKey();
  if (key == 'A') {

    if (menu.get_currentScreen() != &screen_1) {

      menu.previous_screen();
    }
  } else if (key == 'D') {

    if (menu.get_currentScreen() != &screen_4) {
      menu.next_screen();
    }

  } else if (key == '#') {
    menu.switch_focus();

  } else if (key == '*') {
    lastScreen = menu.get_currentScreen();
    menu.call_function(1);
    //lcd.print(readNumber());
  }
  if (TEST_TEMP > umbrTempHigh) {
    color(255, 0, 0);
    EasyBuzzer.singleBeep(
      300,  // Frequency in hertz(HZ).
      500   // Duration of the beep in milliseconds(ms).
    );
  } else if (TEST_TEMP < umbrTempLow) {
    color(0, 0, 255);
    EasyBuzzer.stopBeep();
  } else {
    color(0, 255, 0);
    EasyBuzzer.stopBeep();
  }
}