#include <OneWire.h> 
#include <DallasTemperature.h>
// On utilise la pin 2
#define ONE_WIRE_BUS 2 
// On crée un port OneWire et on l'utilise avec Dallas temperature
OneWire oneWire(ONE_WIRE_BUS);  
DallasTemperature sensors(&oneWire);


void setup(void) 
{ 

 Serial.begin(9600); 
 sensors.begin(); 
} 
void loop(void) 
{ 
 #Les commandes pour récuperer la température sont très simple
 sensors.requestTemperatures(); 
 Serial.print("Temperature is: "); 
 Serial.print(sensors.getTempCByIndex(0)); 
   delay(1000); 
} 
