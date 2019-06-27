#include "DHT.h"  //DHT Bibliothek für DHT22 Sensor einbinden

#define DHTPIN A0 //Sensoranschluss and PIN A0
#define DHTTYPE DHT22 //Festlegung das es sich um den Sensor DHT 22 (AM2302) handelt

int maxLuftfeuchtigkeit = 60; //Maximale Luftfeuchtigkeit auf 80% festlegt
int maxTemperatur = 25; //Maximale Temperatur auf 25°C festlegt

DHT dht(DHTPIN, DHTTYPE); //Objekt für DHT Initalisierung mit Bezeichnung "dht" und DHT Pin und DHT Type

void setup() 
{
  Serial.begin(9600); //Starten der seriellen Kommunikation mit 9600 bauds per second
  
  dht.begin();  //Starten der Kommunikation mit dem DHT Sensor
}

void loop()
{
  delay(2000);  //Zwei Sekunden Pause bis zur Messung (langsamer Sensor)
  
  float Luftfeuchtigkeit = dht.readHumidity();  //Auslesen der Luftfeuchtigkeit aus dem Sensor und Speicherung unter der Variable "Luftfeutchtigkeit"
  float Temperatur = dht.readTemperature(); //Auslesen der Temperatur aus dem Sensor und Speicherung unter der Variable "Temperatur"

  if (isnan(Temperatur) || isnan(Luftfeuchtigkeit)) //Kontrolle der Messwert auf Sinnhaftigkeit (NaN = not a number). Wenn keine Zahl als Messwert vorliegt, ...
  {
    Serial.println("Fehler beim Auslesen des DHT22 Sensors.");  //... soll im seriellen Monitor die Meldung "Fehler beim Auslesen des DHT22 Sensors." ausgegeben werden
  }
  else  //Ansonsten ...
  {
    Serial.print("Luftfeuchtigkeit: "); //... sollen im seriellen Monitor die Wörter "Luftfeuchtigkeit" und "Temperatur" mit ihren Messwerten und Einheiten ausgegeben werden
    Serial.print(Luftfeuchtigkeit);
    Serial.print(" %\t");
    Serial.print("Temperatur: "); 
    Serial.print(Temperatur);
    Serial.println(" *C");
  }
}
