#include <Adafruit_Keypad.h>
#include <LiquidCrystal.h>
#include <string.h>
#include <EasyBuzzer.h>
#include <AsyncTaskLib.h>


#define PULSADOR 2        // pulsador en pin 2
#define BUZZER_PASIVO 12  // buzzer pasivo en pin 12


#define LED_RED 46
#define LED_GREEN 44
#define LED_BLUE 45

#pragma region configuracion_teclado
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
Adafruit_Keypad customKeypad = Adafruit_Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
#pragma endregion
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
#pragma region configuracion_lcd
const int rs = 7, en = 8, d4 = 22, d5 = 24, d6 = 26, d7 = 28;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
#pragma endregion


bool flgEsperar = false;
bool flgQuedanIntentos = true;
bool flgPuedeLeer = true;
bool flgPasswordcorrecto = false;
bool flgPasswordIngresado = false;
bool flgFirstCharacter = true;

char contrasenia_leida[9];
signed char conteoCaracteres = 0;
char contrasenia[] = "12345";
signed char intentos = 0;
long elapsedtime = 0;
void color(unsigned char red, unsigned char green, unsigned char blue)  // the color generating function
{
  analogWrite(LED_RED, red);
  analogWrite(LED_BLUE, blue);
  analogWrite(LED_GREEN, green);
}

AsyncTask tskAwaitFiveSeconds(300, true, []() {
  if (flgEsperar) {
    for (int i = 5; i > 0; i--) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Espere ");
      lcd.print(i);
      lcd.println(" Secs");
      delay(1000);
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sigue intentando");
    EasyBuzzer.stopBeep();
    flgEsperar = false;
    flgPuedeLeer = true;
    flgQuedanIntentos = true;
    flgPasswordcorrecto = false;
    flgPasswordIngresado = false;
    intentos = 0;
  }
});

AsyncTask tskAwaitTenSeconds(10000, false, []() {
  verificarContrasenia();
  flgFirstCharacter = true;
  flgPasswordIngresado = true;
  lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("time out: try again");
});

void verificarContrasenia() {
  if (strcmp(contrasenia, contrasenia_leida) == 0) {
    Serial.println("Contraseña correcta");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Correcto");
    flgPasswordcorrecto = true;
  } else {
    Serial.println("Contraseña incorrecta");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Incorrecto");
    flgPasswordcorrecto = false;
  }
}
AsyncTask tskLeerPassword(100, true, []() {
  if (flgPuedeLeer && flgQuedanIntentos) {

    if (customKeypad.available() && conteoCaracteres <= 8) {
      keypadEvent e = customKeypad.read();
      char caracterLeido = (char)e.bit.KEY;
      Serial.println(caracterLeido);
      if (caracterLeido == '*' && e.bit.EVENT == KEY_JUST_PRESSED || conteoCaracteres == 8) {
        contrasenia_leida[conteoCaracteres] = '\0';
        verificarContrasenia();
        tskAwaitTenSeconds.Stop();

        flgPasswordIngresado = true;
      }

      if (e.bit.EVENT == KEY_JUST_PRESSED && caracterLeido != '*') {
        if (flgFirstCharacter) {
          tskAwaitTenSeconds.Start();
          flgFirstCharacter = false;
        }
        tskAwaitTenSeconds.Reset();
        contrasenia_leida[conteoCaracteres++] = caracterLeido;
        Serial.print("Contraseña leida: ");
        Serial.println(contrasenia_leida);
        char tempBuffer[9] = "";
        memset(tempBuffer, '*', conteoCaracteres);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(tempBuffer);
      }

      //Serial.println(conteoCaracteres);

      if (e.bit.EVENT == KEY_JUST_PRESSED) {
        Serial.println(" pressed");
      } else if (e.bit.EVENT == KEY_JUST_RELEASED) Serial.println(" released");
    }
  }
});

AsyncTask tskDecisionPassword(200, true, []() {
  if (flgPasswordIngresado && !flgEsperar) {
    strcpy(contrasenia_leida, "        \0");
    conteoCaracteres = 0;
    flgPasswordIngresado = false;
    if (flgPasswordcorrecto) {
      color(0, 255, 0);
      Serial.println("LED VERDE");

    } else {
      color(0, 0, 255);
      Serial.println("LED AZUL");
      intentos++;
    }
    if (intentos > 3) {
      flgQuedanIntentos = false;
      flgPuedeLeer = false;
      flgEsperar = true;
      color(255, 0, 0);
      Serial.println("LED ROJO");
      EasyBuzzer.singleBeep(
        300,  // Frequency in hertz(HZ).
        500   // Duration of the beep in milliseconds(ms).
      );
    }
  }
});




void setup() {
  lcd.begin(16, 2);
  customKeypad.begin();
  pinMode(PULSADOR, INPUT_PULLUP);  // pin 2 como entrada con resistencia de pull-up
  pinMode(BUZZER_PASIVO, OUTPUT);   // pin 8 como salida
  Serial.begin(9600);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  EasyBuzzer.setPin(BUZZER_PASIVO);
  tskLeerPassword.Start();

  tskDecisionPassword.Start();
  tskAwaitFiveSeconds.Start();
}
void loop() {
  // put your main code here, to run repeatedly:
  customKeypad.tick();
  tskLeerPassword.Update();
  tskAwaitTenSeconds.Update();
  tskDecisionPassword.Update();
  tskAwaitFiveSeconds.Update();
  EasyBuzzer.update();
}
