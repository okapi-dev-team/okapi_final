#include "DHT.h"  //DHT Bibliothek für DHT22 Sensor einbinden
#include <Servo.h>  //Servo Bibliothek für Servomotor einbinden

#define DHTPIN A0 //Sensoranschluss and PIN A0
#define DHTTYPE DHT22 //Festlegung das es sich um den Sensor DHT 22 (AM2302) handelt

int maxLuftfeuchtigkeit = 60; //Maximale Luftfeuchtigkeit auf 80% festlegt
int maxTemperatur = 25; //Maximale Temperatur auf 25°C festlegt
int messBodenfeuchtigkeit = 0; //Variable "messBodenfeuchtigkeit" erstellt, unter der später der Messwert gespeichert wird (Startwert 0)
int minBodenfeuchtigkeit = 30;  //Minimale Bodenfeuchtigkeit auf 30% festlegt
int Bodenfeuchtigkeit = 0;  //Variable "Bodenfeuchtigkeit" erstellt, unter der der auf die neue Skala umgewandeltete Messwert gespeichert wird (Startwert 0)
int LED = 4; //ArduinoPin 4 für die LED festgelegt
int AlarmanlagenStatus = 0; //AlarmanlagenStatus (0 = ausgeschaltet, 1 = eingeschaltet)

DHT dht(DHTPIN, DHTTYPE); //Objekt für DHT Initalisierung mit Bezeichnung "dht" und DHT Pin und DHT Type
Servo ServoFenster; //Objekt für Servo Initalisierung mit Bezeichnung "ServoFenster"

void setup() 
{
  Serial.begin(9600); //Starten der seriellen Kommunikation mit 9600 bauds per second
  
  pinMode(LED, OUTPUT); //Definition des Arduino Pins für die LED als Ausgang

  ServoFenster.attach(8); //Festlegung das der Servo an Pin 8 angeschlossen wird
  
  dht.begin();  //Starten der Kommunikation mit dem DHT Sensor

  ServoFenster.write(0);  //Standartmößig soll das das Fenster geschlossen sein (Schließposition 0°)
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

  if(AlarmanlagenStatus == 1 && (Luftfeuchtigkeit > maxLuftfeuchtigkeit || Temperatur > maxTemperatur))  //Wenn bei eingeschalteter Alarmanlage die gemessene Luftfeuchtigkeit oder Temperatur den Maximalwert überschreitet, soll ...
  {
    ServoFenster.write(45); //... der Servomotor das Fenster öffnen (Er steurt die Öffnungsposition 45° an)
  }
  else  //Ansonsten ...
  {
    ServoFenster.write(0);  //... soll das Fenster geschlossen bleiben (Schließposition 0°)
  }

  messBodenfeuchtigkeit=analogRead(A1); //Auslesen der Bodenfeuchtigkeit aus dem Sensor und Speicherung unter der Variable "messBodenfeuchtigkeit"

  Bodenfeuchtigkeit=map(messBodenfeuchtigkeit,0,700,0,100); //Umwandeln des Messwerts der Bodenfeuchtigkeit auf eine Skala von 0-100% und Speicherung unter der Variable "Bodenfeuchtigkeit" (0% = trocken, 100% = feucht)

  if(Bodenfeuchtigkeit > 100) //Wenn der umgewandelte Messwert der Bodenfeuchtigkeit die 100% überschreitet, soll ...
  {
    Bodenfeuchtigkeit = 100;  //... die Bodenfeuchtigkeit auf 100% gesetzt werden
  }

  Serial.print("Bodenfeuchtigkeit: "); //Das Wort "Bodenfeuchtigkeit" mit Messwert und Einheit wird im seriellen Monitor ausgegeben
  Serial.print(Bodenfeuchtigkeit);
  Serial.println(" %\t");

  if(Bodenfeuchtigkeit < minBodenfeuchtigkeit)  //Wenn die gemessene Luftfeuchtigkeit den Minimalwert unterschreitet, soll ...
  {
    digitalWrite(LED, HIGH); //... die LED angeschaltet werden
  }
  else  //Ansonsten ...
  {
    digitalWrite(LED, LOW); //... soll die LED ausgeschaltet bleiben
  }
}
