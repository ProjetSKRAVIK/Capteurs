#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_TSL2591.h"
  
#include <SPI.h>

#include "nRF24L01.h"

#include "RF24.h"

int transmitterId;

// connecter SCL au I2C Clock
// connecter SDA au I2C Data
// connecter Vin au 3.3-5V DC
// connecter GROUND au common ground
//SCK -> 13
//MISO -> 12
//MOSI -> 11
//CSN -> 10
//CE -> 9

RF24 radio(9, 10);
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591); //On définit un nombre pour identifier le capteur
const uint64_t pipe = 0xF0F0F0F0D2LL;

void configureSensor(void)
{
  // On peut définir le gain en fonction de la luminosité
  //tsl.setGain(TSL2591_GAIN_LOW);    // 1x gain (très lumineux)
  tsl.setGain(TSL2591_GAIN_MED);      // 25x gain
  //tsl.setGain(TSL2591_GAIN_HIGH);   // 428x gain
  
  // On peut changer le temps que met le capteur pour recevoir la lumière (il faut augmenter ce temps quand la luminosité est faible)
  //tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS);  // court
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_200MS);
  tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_400MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_500MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_600MS); 

  /* On affiche les réglages*/
  Serial.println(F("------------------------------------"));
  Serial.print  (F("Gain:         "));
  tsl2591Gain_t gain = tsl.getGain();
  switch(gain)
  {
    case TSL2591_GAIN_LOW:
      Serial.println(F("1x (Low)"));
      break;
    case TSL2591_GAIN_MED:
      Serial.println(F("25x (Medium)"));
      break;
    case TSL2591_GAIN_HIGH:
      Serial.println(F("428x (High)"));
      break;
    case TSL2591_GAIN_MAX:
      Serial.println(F("9876x (Max)"));
      break;
  }
  Serial.print  (F("Timing:       "));
  Serial.print((tsl.getTiming() + 1) * 100, DEC); 
  Serial.println(F(" ms"));
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));
}

void advancedRead(void)

  uint32_t lum = tsl.getFullLuminosity();
  uint16_t ir, full;
  ir = lum >> 16;
  full = lum & 0xFFFF;
  //Serial.print(F("[ ")); Serial.print(millis()); Serial.print(F(" ms ] "));

//Infrarouge
  //Serial.print(F("IR: ")); Serial.print(ir);  Serial.print(F("  "));
  //Serial.print(F("Spectre complet: ")); Serial.print(full); Serial.print(F("  "));
// Visible
  //Serial.print(F("Visible: ")); Serial.print(full - ir); Serial.print(F("  "));

//Luminosité
  return tsl.calculateLux(full, ir)
}


void setup() {
  // put your setup code here, to run once:
  tsl.begin()
  configureSensor();


  radio.begin();

  radio.setDataRate(RF24_250KBPS); // reducing bandwidth

  radio.openWritingPipe(pipe); //On definit le canal utilisé par notreemetteur

}

void loop() {

  radio.powerUp();
  lumi = advancedRead(); 
  
  radio.write(&lumi, sizeof(lumi));
  delay(1000);
}
