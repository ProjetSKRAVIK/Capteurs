#include <OneWire.h> 
#include <DallasTemperature.h>
// On utilise la pin 2
#define ONE_WIRE_BUS 2 
// On crée un port OneWire et on l'utilise avec Dallas temperature
OneWire oneWire(ONE_WIRE_BUS);  
DallasTemperature sensors(&oneWire);


#include "nRF24L01.h"

#include "RF24.h"
const uint64_t pipe = 0xF0F0F0F0E2LL;

//SCK -> 13
//MISO -> 12
//MOSI -> 11
//CSN -> 10
//CE -> 9
RF24 radio(9, 10);

void setup(void) 
{ 
  radio.begin();

  radio.setDataRate(RF24_250KBPS); // reducing bandwidth

  radio.openWritingPipe(pipe); //On definit le canal utilisé par notreemetteur


 sensors.begin(); 
} 
void loop(void) 
{ 
 radio.powerUp();
 #Les commandes pour récuperer la température sont très simple
 sensors.requestTemperatures(); 

 temp = 300000+sensors.getTempCByIndex(0); 
 radio.write(&temp, sizeof(temp));

 delay(1000); 
} 
