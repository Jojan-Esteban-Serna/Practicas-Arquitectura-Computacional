/*
Integrantes:
- Santiago Agredo vallejo
- Fredy Junior Ortega Garcia
- Jojan Esteban Serna Serna
*/
#include <LiquidCrystal.h>
#include "DHTStable.h"
#include "AsyncTaskLib.h"

#define NOTE_B0  31	// notas y frecuencias
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978


#define LED_GREEN 40
#define LED_BLUE 38
#define LED_RED 42

#define DHT11_PIN 22 
#define BUZZER_PASIVO 51

int melodia[] = {		// array con las notas de la melodia
  NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_F4, NOTE_E4, NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_G4, NOTE_F4, NOTE_C4, NOTE_C4, NOTE_C5, NOTE_A4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_AS4, NOTE_AS4, NOTE_A4, NOTE_F4, NOTE_G4, NOTE_F4
};

int duraciones[] = {		// array con la duracion de cada nota
  8, 8, 4, 4, 4, 2, 8, 8, 4, 4, 4, 2, 8, 8, 4, 4, 4, 4, 4, 8, 8, 4, 4, 4, 2 
};

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
DHTStable DHT;

void errors(int chk){
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
float humy, temp;
AsyncTask asyncTask1(2000, true, []() { 
  int chk = DHT.read11(DHT11_PIN);
  errors(chk);
  temp = DHT.getTemperature(); 
  Serial.println("Temp: ");
  Serial.println(temp);   
});
AsyncTask asyncTask2(1000, true, []() {  
  int chk = DHT.read11(DHT11_PIN);
  errors(chk);
  humy = DHT.getHumidity();                                        
  Serial.println("Humd: ");
  Serial.println(humy);  
});

AsyncTask asyncTask3(4000, true, []() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("HUMEDAD: ");
    lcd.print(humy);
    lcd.setCursor(0,1);
    lcd.print("TEMP: ");
    lcd.print(temp);
});

void setup()
{
    lcd.begin(16, 2);
    Serial.begin(115200);
    pinMode(BUZZER_PASIVO, OUTPUT);	// pin 51 como salida
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
    pinMode(LED_RED, OUTPUT);
    asyncTask1.Start();
    asyncTask2.Start();
    asyncTask3.Start();
}
void activarMelodia(){
     Serial.println("*Suena musica de fondo*");
     delay(1000);
  
  /*
    for (int i = 0; i < 25; i++) 
    {			// bucle repite 25 veces
        int duracion = 1000 / duraciones[i];		// duracion de la nota en milisegundos
        tone(BUZZER_PASIVO, melodia[i], duracion);	// ejecuta el tono con la duracion
        int pausa = duracion * 1.30;			// calcula pausa
        delay(pausa);					// demora con valor de pausa
        noTone(BUZZER_PASIVO);				// detiene reproduccion de tono
    }  */
}
void loop()
{
    asyncTask1.Update();
    asyncTask2.Update();
    asyncTask3.Update();
    if(temp > 29){
        digitalWrite(LED_RED, HIGH);  
        activarMelodia();
    }else if(temp < 26){
        digitalWrite(LED_BLUE, HIGH); 
        activarMelodia();
    }else if(temp > 26 && temp <29){
        digitalWrite(LED_GREEN, HIGH);  
    } 
    digitalWrite(LED_RED, LOW); 
    digitalWrite(LED_BLUE, LOW); 
    digitalWrite(LED_GREEN, LOW);
}