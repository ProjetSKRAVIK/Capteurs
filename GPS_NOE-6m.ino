#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// On choisit 2 pins
int RXPin = 2;
int TXPin = 3;

int GPSBaud = 9600;

TinyGPSPlus gps;

// On crée un Software serial pour communiquer
SoftwareSerial gpsSerial(RXPin, TXPin);

void setup()
{
  Serial.begin(9600);
  gpsSerial.begin(GPSBaud);
}

void loop()
{
  // On affiche les données dès qu'elles sont reçues
  while (gpsSerial.available() > 0)
    if (gps.encode(gpsSerial.read()))
      displayInfo();

  // Si on ne reçoit rien pendant 5 secondes, le GPS ne fonctionne pas
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println("Erreur, pas de GPS detecté");
    while(true);
  }
}

void displayInfo()
{
  
  //On récupère les coordonnées
  if (gps.location.isValid())
  {
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Altitude: ");
    Serial.println(gps.altitude.meters());
  }
  else
  {
    Serial.println("Coordonnées non disponibles");
  }
  
  //On récupère la date
  Serial.print("Date: ");
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.println(gps.date.year());
  }
  else
  {
    Serial.println("Date non disponible");
  }

  //On récupère l'heure
  Serial.print("Heure: ");
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(":");
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(":");
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(".");
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.println(gps.time.centisecond());
  }
  else
  {
    Serial.println("Heure non disponible");
  }

  Serial.println();
  Serial.println();
  delay(1000);
}
