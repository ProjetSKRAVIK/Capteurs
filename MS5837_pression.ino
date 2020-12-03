/* Blue Robotics MS5837 Library Example
-----------------------------------------------------
 
Title: Blue Robotics MS5837 Library Example

Description: This example demonstrates the MS5837 Library with a connected
sensor. The example reads the sensor and prints the resulting values
to the serial terminal.

The code is designed for the Arduino Uno board and can be compiled and 
uploaded via the Arduino 1.0+ software.

-------------------------------
The MIT License (MIT)

Copyright (c) 2015 Blue Robotics Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-------------------------------*/
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"
#include "MS5837.h"


RTC_DS1307 rtc;
MS5837 sensor;
File file;
int sensorValue;
const byte SDCARD_CS_PIN = 10; // TODO remplacer avec le pin de la carte sd


void setup() {
  Serial.begin(9600);
  //carte SD
 if(!SD.begin(SDCARD_CS_PIN)) {
 Serial.println(F("Initialisation failed !"));
 return;
 }
 Serial.println(F("Initialisation OK"));
 //fin carte SD

 //Horloge
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
 Wire.begin();

 // Inititalisation du capteur de pression et verification des connections
 while (!sensor.init()) {
    Serial.println("Init failed!");
    Serial.println("Are SDA/SCL connected correctly?");
    Serial.println("Blue Robotics Bar30: White=SDA, Green=SCL");
    Serial.println("\n\n\n");
    delay(5000);
 }
  
 sensor.setModel(MS5837::MS5837_30BA);
 //On donne la densité du fluide dans lequel on plonge la sonde
 sensor.setFluidDensity(997); // kg/m^3 (eau douce, 1029 eau de mer)

}

void loop() {
  DateTime now = rtc.now();

//Nnom du fichier en fonction de la date (
 char datafile[13]; 
 int day=now.day();
 int month = now.month();
 int year= now.year(); 
 sprintf(datafile,"%02d%02d%04d.csv",day,month,year); // %d pour un int 
 datafile[13]='\0';//à mettre pour fermer convenablement le fichier
//end naming phase

//ouverture du fichier
 file = SD.open(datafile, FILE_WRITE);
 
 //Test pour écriture
 if(file) {
 Serial.println(F("Ecriture en cours"));
 //Ecriture
 write_current_date(file);
 file.print(";");
 write_data();
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
  // On lit la pression, la temperature et la profondeur
  sensor.read();
 
  file.print(sensor.pressure()); 
  file.print(" mbar;");
  
  file.print(sensor.temperature()); 
  file.print(" deg C;");
  
  file.print(sensor.depth()); 
  file.print(" m;");

  file.print(sensor.altitude()); 
  file.println(" m;");

  delay(1000);
}
