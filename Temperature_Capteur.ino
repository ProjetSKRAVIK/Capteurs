#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"
#include <OneWire.h> 
#include <DallasTemperature.h>
// On utilise la pin 2
#define ONE_WIRE_BUS 2 
// On crée un port OneWire et on l'utilise avec Dallas temperature
OneWire oneWire(ONE_WIRE_BUS);  
DallasTemperature sensors(&oneWire);

RTC_DS1307 rtc;

File file;
int sensorValue;
const byte SDCARD_CS_PIN = 10; // TODO remplacer par le pin de la carte SD


void setup() {
  Serial.begin(9600);
//carte SD
 if(!SD.begin(SDCARD_CS_PIN)) {
 Serial.println(F("Initialisation failed !"));
 return;
 }
 Serial.println(F("Initialisation OK"));
//fin SD

//horloge
 #ifdef AVR
 Wire.begin();
 #else
 Wire1.begin(); // Shield I2C pins connect to alt I2C bus on Arduino Due
 #endif
 rtc.begin();

 if (! rtc.isrunning()) {
 Serial.println("RTC failed!");
 rtc.adjust(DateTime(__DATE__, __TIME__));
 }
//fin horloge

}

void loop() {
  DateTime now = rtc.now();

//nom du fichier en fonction de la date
 char datafile[13];
 int day=now.day();
 int month = now.month();
 int year= now.year(); 
 sprintf(datafile,"%02d%02d%04d.csv",day,month,year); // %d pour un int 
 datafile[13]='\0';//à mettre pour fermer convenablement le fichier
//fin nommage

//ouverture du fichier
 file = SD.open(datafile, FILE_WRITE);
 
 //Test pour écriture
 if(file) {
 Serial.println(F("Ecriture en cours"));
 //Ecriture
 write_current_date(file);
 file.print(";");
 write_data
 file.close();
 }
 //fermeture du fichier
 
}

void write_current_date(File file) {
 DateTime now = rtc.now();
 
 file.print(now.year(), DEC);
 file.print('/');
 file.print(now.month(), DEC);
 file.print('/');
 file.print(now.day(), DEC);
 file.print('_');
 file.print(now.hour(), DEC);
 file.print(':');
 file.print(now.minute(), DEC);
 file.print(':');
 file.print(now.second(), DEC);
}

void write_data(){
   #Les commandes pour récuperer la température sont très simple
 sensors.requestTemperatures(); 
 file.print(sensors.getTempCByIndex(0)); 
 delay(1000); 
}
