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

#include <Wire.h>
#include "MS5837.h"
#include <SPI.h>

#include "nRF24L01.h"

#include "RF24.h"


//SCK -> 13
//MISO -> 12
//MOSI -> 11
//CSN -> 10
//CE -> 9
RF24 radio(9, 10);
MS5837 sensor;


const uint64_t pipe = 0xE8E8F0F0E1LL;


void setup() {
  
  Wire.begin();

  // Inititalisation du capteur de pression et verification des connections
  sensor.init()

  sensor.setModel(MS5837::MS5837_30BA);
 //On donne la densité du fluide dans lequel on plonge la sonde
  sensor.setFluidDensity(1029); // kg/m^3 (997 eau douce, 1029 eau de mer)

  radio.begin();

  radio.setDataRate(RF24_250KBPS); // reducing bandwidth

  radio.openWritingPipe(pipe); //On definit le canal utilisé par notreemetteur

}

void loop() {
  radio.powerUp();
  // On lit la pression, la temperature et la profondeur
  sensor.read();

  pres = 200000 + sensor.pressure(); //On code l'id de notre arduino (2) dans la luminosité car pression<30000mPa

  radio.write(&pres, sizeof(pres));

  delay(1000);
}
