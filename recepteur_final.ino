#include <SPI.h>

#include "nRF24L01.h"

#include "RF24.h"

#include "RTClib.h"

#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// On choisit 2 pins
int RXPin = 2;
int TXPin = 3;

int GPSBaud = 9600;

TinyGPSPlus gps;

// On crée un Software serial pour communiquer
SoftwareSerial gpsSerial(RXPin, TXPin);

RTC_DS1307 rtc;

File lumi;
File pres;
File temp;

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10
//Contacts from the radio to connect NRF24L01 pinamnam -> Arduino

//SCK -> 13
//MISO -> 12
//MOSI -> 11
//CSN -> 10
//CE -> 9
const byte SDCARD_CS_PIN = 2; // TODO remplacer par le pin de la carte sd
RF24 radio(9, 10);

// this is not the channel address, but the transmitter address
const uint64_t pipes1 = 0xF0F0F0F0D2LL, 
const uint64_t pipes2 = 0xF0F0F0F0E1LL
const uint64_t pipes3 = 0xF0F0F0F0E2LL



void setup(void) {
  gpsSerial.begin(GPSBaud);

  radio.begin();

  // the following statements improve transmission range
  radio.setPayloadSize(2); // setting the payload size to the needed value
  radio.setDataRate(RF24_250KBPS); // reducing bandwidth

  radio.openReadingPipe(1,pipes1);
  radio.openReadingPipe(2,pipes2);
  radio.openReadingPipe(3,pipes3);

  radio.startListening(); // begin to listen

  SD.begin(SDCARD_CS_PIN)

  //For Clock
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
//End Clock

  char datafile[11]="Luminosite\0"; //table for the name 
  lumi = SD.open(datafile, FILE_WRITE);
  char datafile[9]="Pression\0"; //table for the name 
  pres = SD.open(datafile, FILE_WRITE);
  char datafile[12]="Temperature\0"; //table for the name 
  temp = SD.open(datafile, FILE_WRITE);

}

void loop(void) {

  //On ecrit dans des fichiers différents chaques informations

  if (radio.available(&pipes1)) {
    write_current_date(lumi);
    temp.print(";");
    write_coordinates(lumi);
    lumi.print(";");
    lumi.print(radio.read(&data, sizeof(data)));
  }
  if (radio.available(&pipes2)) {
    write_current_date(pres);
    temp.print(";");
    write_coordinates(pres);
    pres.print(";");
    pres.print(radio.read(&data, sizeof(data)));
  }
  if (radio.available(&pipes3)) {
    write_current_date(temp);
    temp.print(";");
    write_coordinates(temp);
    temp.print(";");
    temp.print(radio.read(&data, sizeof(data)));
  }

  delay(500);
}


void write_current_date(File file) {
 DateTime now = rtc.now();
 
 file.print(gps.date.year(), DEC);
 file.print('/');
 file.print(gps.date.month(), DEC);
 file.print('/');
 file.print(gps.date.day(), DEC);
 file.print('_');
 file.print(gps.time.hour(), DEC);
 file.print(':');
 file.print(gps.time.minute(), DEC);
 file.print(':');
 file.print(gps.time.second(), DEC);
}



void write_coordinates(File file)
{
  //On récupère les coordonnées
  if (gps.location.isValid())
  {
    file.print(gps.location.lat(), 6);
    file.print(";");
    file.print(gps.location.lng(), 6);
    file.print(";");
    file.print(gps.altitude.meters());
  }
}
