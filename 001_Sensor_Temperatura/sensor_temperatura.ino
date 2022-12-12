//Integrantes: 
//Santiago Agredo Vallejo
//Jojan esteban Serna Serna

#include "DHTStable.h"
#include <LiquidCrystal.h>

DHTStable DHT;

#define DHT11_PIN 22
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
void setup()
{
   lcd.begin(16, 2);
  // Print a message to the LCD.
  Serial.begin(115200);
  Serial.println(__FILE__);
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHTSTABLE_LIB_VERSION);
  Serial.println();
  Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C), \t Led");
}


void loop()
{
  // READ DATA
  Serial.print("DHT11, \t");
  int chk = DHT.read11(DHT11_PIN);
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
  // Mostrar datos en la 
  lcd.print("Humedad: ");
  lcd.print(DHT.getHumidity());
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(DHT.getTemperature());
  
  Serial.print(DHT.getHumidity(), 1);
  Serial.print(",\t");
  Serial.print(DHT.getTemperature(), 1);
  Serial.print(",\t");

  colortemp();
  delay(1000);
  lcd.clear();
}

void colortemp(){
  if(DHT.getTemperature() > 29){
    Serial.println("RED");
    
  }else if(DHT.getTemperature() < 26){
    Serial.println("BLUE");

  }else{
    Serial.println("GREEN");    

  }
}
