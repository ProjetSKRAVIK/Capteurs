# Capteurs
Tout sur les capteurs que l'on va utiliser

## 1. GPS NEO-6m

![Connections](https://lastminuteengineers.com/wp-content/uploads/arduino/Wiring-Connections-NEO-6M-GPS-Module-to-Arduino-UNO.png)

Bibliothèques utilisées :
- SoftwareSerial pour créer un nouveau port série pour communiquer avec le module GPS
- TinyGPS++ pour faciliter la récuperation des données envoyées par le module GPS
                          
Pins utilisées :
- 2 et 3 pour créer le port Série

## 2. TSL2591

![](https://cdn-learn.adafruit.com/assets/assets/000/095/253/medium640/adafruit_products_TSL2591_arduino_original.jpg?1601480003)

Biblothèques utilisées : 
- Adafruit_Sensor.h et Adafruit_TSL2591.h qui contiennent les fonctions pour communiquer avec le capteur
- Wire pour communiquer en I2C

Pins utilisées : 
- A4 et A5  pour communiquer en I2C

## 3. MS5837

![](https://bluerobotics.com/wp-content/uploads/2019/01/level-converter-jumper-pins-1024x708.jpg)
![](https://bluerobotics.com/wp-content/uploads/2019/01/arduino-jumper-pins-1024x635.jpg)

Bibliothèques utilisées : 
- Wire.h pour communiquer en I2C
- MS5837.h qui a été créé pour ce capteur

Pins utilisées : 
-A4 et A5 pour communiquer en I2C

## 4. DS18B20

![](https://iotbytes.files.wordpress.com/2017/11/temperature_arduino.png?w=567&h=356)

Bibliothèques utilisées :
- OneWire, qui permet de communiquer de manière similaire au protocole I2C avec un seul fil (on perd un peu de vitesse de transmission)
- DallasTemperature, la bibliothèque dédiée au capteur

Pins utilisées :
- 2 uniquement grâce au bus one wire
