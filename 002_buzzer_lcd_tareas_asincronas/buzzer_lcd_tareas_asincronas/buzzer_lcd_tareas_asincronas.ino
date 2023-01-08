/*
  Integrantes:
  - Santiago Agredo vallejo
  - Fredy Junior Ortega Garcia
  - Jojan Esteban Serna Serna
*/
#include <LiquidCrystal.h>
#include "DHTStable.h"
#include "AsyncTaskLib.h"
#include <EasyBuzzer.h>

#define LED_BLUE 8
#define LED_GREEN 9
#define LED_RED 10


#define DHT11_PIN 22
#define BUZZER_PASIVO 7


#define HOT_TEMP 29
#define MILD_TEMP 26

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
DHTStable DHT;

void verificarErrores(int chk)
{
  switch (chk)
  {
    case DHTLIB_OK:
      Serial.print("OK,\t");
      break;
    case DHTLIB_ERROR_CHECKSUM:
      Serial.print("Checksum error,\t");
      break;
    case DHTLIB_ERROR_TIMEOUT:
      Serial.print("Time out error,\t");
      break;
    default:
      Serial.print("Unknown error,\t");
      break;
  }
}
float humedadLeida, temperaturaLeida;
AsyncTask tskLeerTemperatura(2000, true, []()
{
  int chk = DHT.read11(DHT11_PIN);
  verificarErrores(chk);
  temperaturaLeida = DHT.getTemperature();
  Serial.print("Temp: ");
  Serial.println(temperaturaLeida);
});

AsyncTask tskLeerHumedad(1000, true, []()
{
  int chk = DHT.read11(DHT11_PIN);
  verificarErrores(chk);
  humedadLeida = DHT.getHumidity();
  Serial.print("Humd: ");
  Serial.println(humedadLeida);
});

AsyncTask tskActualizarDisplay(4000, true, []()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Humedad: ");
  lcd.print(humedadLeida);
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(temperaturaLeida);
});

/*AsyncTask tskReproducirMelodia(4000,false, []()
  {
       Serial.println("*Suena musica de fondo*");


    for (int i = 0; i < 25; i++)
    {           // bucle repite 25 veces
        int duracion = 1000 / duraciones[i];        // duracion de la nota en milisegundos
        tone(BUZZER_PASIVO, melodia[i], duracion);  // ejecuta el tono con la duracion
        int pausa = duracion * 1.30;            // calcula pausa
        delay(pausa);                   // demora con valor de pausa
        noTone(BUZZER_PASIVO);              // detiene reproduccion de tono
    }
  }
  );
*/
void setup()
{
  lcd.begin(16, 2);
  Serial.begin(115200);
  pinMode(BUZZER_PASIVO, OUTPUT);   // pin 51 como salida
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  EasyBuzzer.setPin(BUZZER_PASIVO);

  tskLeerTemperatura.Start();
  tskLeerHumedad.Start();
  tskActualizarDisplay.Start();
}


void color (unsigned char red, unsigned char green, unsigned char blue) // the color generating function
{
  analogWrite(LED_RED, red);
  analogWrite(LED_BLUE, blue);
  analogWrite(LED_GREEN, green);
}
void loop()
{
  tskLeerTemperatura.Update();
  tskLeerHumedad.Update();
  tskActualizarDisplay.Update();
  //tskReproducirMelodia.Update();
  EasyBuzzer.update();
  /*
    EasyBuzzer.singleBeep(
    100,    // Frequency in hertz(HZ).
    500 // Duration of the beep in milliseconds(ms).
    );*/
  //hot 15
  //mild 10
  if (temperaturaLeida > HOT_TEMP)
  {
    color(255, 0, 0);
    EasyBuzzer.singleBeep(
      300,  // Frequency in hertz(HZ).
      500   // Duration of the beep in milliseconds(ms).
    );
  }
  else if (temperaturaLeida < MILD_TEMP)
  {

    color(0, 0, 255);

    EasyBuzzer.singleBeep(
      200,  // Frequency in hertz(HZ).
      500   // Duration of the beep in milliseconds(ms).
    );
  }
  else if (temperaturaLeida > MILD_TEMP && temperaturaLeida < HOT_TEMP)
  {
    color(0, 255, 0);
  }

}