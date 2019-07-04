#include <Servo.h>  //Servo Bibliothek für Servomotor einbinden

Servo ServoFenster; //Objekt für Servo Initalisierung mit Bezeichnung "ServoFenster"

void setup() 
{
  Serial.begin(9600); //Starten der seriellen Kommunikation mit 9600 bauds per second

  ServoFenster.attach(8); //Festlegung das der Servo an Pin 8 angeschlossen wird
  ServoFenster.write(0);  //Standartmößig soll der Servo bei Position 0° stehen
}

void loop()
{
  ServoFenster.write(45); //Der Servo fährt auf die Position 45°
  delay(500); //Zeit für Servo zum Erreichen der Position (500ms)
  ServoFenster.write(0);  //Der Servo fährt auf die Ausgangsposition 0°
  delay(500); //Zeit für Servo zum Erreichen der Position (500ms)
}
