#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// On choisit 2 pins
int RXPin = 2;
int TXPin = 3;

int GPSBaud = 9600;

TinyGPSPlus gps;

const byte SDCARD_CS_PIN = 10; // TODO remplacer avec le pin de la carte SD
File file;
char datafile[13]; //tableau pour le nom du fichier
    
// On crée un Software serial pour communiquer
SoftwareSerial gpsSerial(RXPin, TXPin);

void setup()
{
  Serial.begin(9600);
  gpsSerial.begin(GPSBaud);

  //pour la carte sd
 if(!SD.begin(SDCARD_CS_PIN)) {
 Serial.println(F("Echec Initialisation  !"));
 return;
 }
 Serial.println(F("Initialisation OK"));
//fin SD
}

void loop()
{
  // On affiche les données dès qu'elles sont reçues
  while (gpsSerial.available() > 0)
    if (gps.encode(gpsSerial.read()))
      name_file();
      //ouverture fichier
      file = SD.open(datafile, FILE_WRITE);
 
      //Test pour écriture
      if(file) {
      Serial.println(F("Ecriture en cours"));
      //Ecriture
      write_current_date(file);
      file.print(";");
      write_localisation();
      file.print(";");
      file.close();
      }
      //fermeture du fichier
      
      //displayInfo();

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

void name_file(){
  //nom du fichier en fonction de la date
  if (gps.date.isValid())
  {
    int month =gps.date.month() 
    int day =gps.date.day();
    int year = gps.date.year();
    sprintf(datafile,"%02d%02d%04d.csv",day,month,year); // %d pour un int 
    datafile[13]='\0';//à mettre pour fermer convenablement le fichier
  }
  else
  {
    Serial.println("Date non disponible");
    return
  }
}

void write_current_date(File file) {
 file.print(gps.date.year(), DEC);
 file.print('/');
 file.print(gps.date.month(), DEC);
 file.print('/');
 file.print(gps.date.day(), DEC);
 file.print('_');
 if (gps.time.hour() < 10) Serial.print(F("0"));
 file.print(gps.time.hour());
 file.print(":");
 if (gps.time.minute() < 10) Serial.print(F("0"));
 file.print(gps.time.minute());
 file.print(":");
 if (gps.time.second() < 10) Serial.print(F("0"));
 file.print(gps.time.second());
}

void write_localisation(){
  //On récupère les coordonnées
  if (gps.location.isValid())
  {
    file.print(gps.location.lat(), 6);
    file.print(";");
    file.print(gps.location.lng(), 6);
    file.print(";");
    file.print(gps.altitude.meters());
  }
  else
  {
    Serial.println("Coordonnées non disponibles");
  }

   delay(1000);//delai entre les mesures, a adapter pour avoir 1 mesure par seconde si possible
}
