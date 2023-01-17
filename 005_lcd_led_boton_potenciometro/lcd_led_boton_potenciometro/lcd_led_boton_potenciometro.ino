#include <LiquidCrystal.h>
const int photocellPin = A0;
const int ledPin = 13;
LiquidCrystal lcd(52, 50, 48, 46, 44, 42);
int outputValue = 0;
/*************************************************/
void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
}
/*************************************************/
void loop() {
  outputValue = analogRead(photocellPin);
  lcd.setCursor(0, 0);
  lcd.print("Photocell:");
  lcd.setCursor(11, 0);
  lcd.print(outputValue);  //print the temperature on lcd1602
  Serial.println(outputValue);
  delay(1000);
  lcd.setCursor(11, 0);
  lcd.print("");
}
/*************************************************/