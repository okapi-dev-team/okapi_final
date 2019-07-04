#include <Stepper.h>  //Stepper Bibliothek für Schrittmotor einbinden

int SPU = 2048;  //Schritte pro Umdrehung des Schrittmotors auf 2048 festgelegt
int Trigger = 8;  //ArduinoPin 8 für den Trigger-Pin des Ultraschallsensors festgelegt
int Echo = 9; //ArduinoPin 9 für den Echo-Pin des Ultraschallsensors festgelegt
long Dauer = 0; //Variable "Dauer" erstellt, unter der die Zeit gespeichert wird, die eine Schallwelle bis zur Reflektion und zurück benötigt (Startwert 0)
long Entfernung = 0;  //Variable "Entfernung" erstellt, unter der die berechnete Entfernung gespeichert wird (Startwert 0)

Stepper Motor(SPU, 3,5,4,6);  //Objekt für Stepper Initialisierung mit Bezeichnung "Motor" und Angabe der Anschluss-Pins

/** \brief  Berechnet die Entfernung zu einem Gegenstand anhand einer Ultraschallwelle
 *  \return Entfernung als long integer
 */
long Abstandsbestimmung()
{
  digitalWrite(Trigger, LOW); //Spannung vom Trigger-Pin für rauschfreies Signal beim späteren Senden genommen
  delay(5); //Dauer: 5 Millisekunden
  digitalWrite(Trigger, HIGH);  //Aussendung einer Ultraschallwelle
  delay(10);  //Ausgabe Ultraschallwelle für 10 Millisekunden
  digitalWrite(Trigger, LOW); //Aussachalter der Ultraschallwelle
  
  Dauer = pulseIn(Echo, HIGH);  //Messung der Zeit in Mikrosekunden bis der Schall zum Ultraschallsensor zurückkehrt und Speicherung des Ergebnis unter "Dauer"

  Entfernung = (Dauer/2) * 0.03432; //Berechnung der Entfernung in Zentimetern (Teilung durch zwei: Berechnung von nur einer Strecke; Multiplizierung mit der Schallgeschwindigkeit in der Einheit Zentimeter/Mikrosekunde)

  Serial.print("Entfernung: "); //Ausgabe der gemessenen Entfernung in cm am seriellen Monitor
  Serial.print(Entfernung);
  Serial.println("cm");

  return Entfernung;  //Rückgabe der Entfernung
}

/** \brief Fährt den Schrittmotor bis ein vorgegebener Abstand erreicht wurde
 *  
 */
void Runterfahren()
{
  if(Abstandsbestimmung() <= 10)  //Wenn die gemessene Entfernung bei 10cm oder weniger liegt, soll...
  {
    Serial.print("Geschlossen, Entfernung: ");  //... die Nachricht "Geschlossen" und die gemessene Entfernung in cm am seriellen Monitor ausgegeben werden
    Serial.print(Entfernung);
    Serial.println("cm");
  }
  else  //Ansonsten...
  {
    do  //... soll der Schrittmotor solange fahren, bis ...
    {
      Motor.step(2048);  //Der Schrittmotor macht 2048 Schritte (eine Umdrehung)
    }
    while(Abstandsbestimmung() > 10); //... die gemessene Entfernung bei mehr als 10cm liegt
    Serial.print("Geschlossen, Entfernung: ");  //... die Nachricht "Geschlossen" und die gemessene Entfernung in cm am seriellen Monitor ausgegeben werden
    Serial.print(Entfernung);
    Serial.println("cm");
  }
}

/** \brief Fährt den Schrittmotor bis ein vorgegebener Abstand erreicht wurde
 *  
 */
void Hochfahren()
{
  if(Abstandsbestimmung() >= 100)  //Wenn die gemessene Entfernung bei 100cm oder mehr liegt, soll...
  {
    Serial.print("Geöffnet, Entfernung: "); //... die Nachricht "Geschlossen" und die gemessene Entfernung in cm am seriellen Monitor ausgegeben werden
    Serial.print(Entfernung);
    Serial.println("cm");
  }
  else  //Ansonsten...
  {
    do  //... soll der Schrittmotor solange fahren, bis ...
    {
      Motor.step(-2048);  //Der Schrittmotor macht 2048 Schritte in die andere Richtung (eine Umdrehung)
    }
    while(Abstandsbestimmung() < 100); //... die gemessene Entfernung bei weniger als 100cm liegt
    Serial.print("Geöffnet, Entfernung: "); //... die Nachricht "Geöffnet" und die gemessene Entfernung in cm am seriellen Monitor ausgegeben werden
    Serial.print(Entfernung);
    Serial.println("cm");
  }
}

void setup()
{
  Serial.begin(9600); //Starten der seriellen Kommunikation mit 9600 bauds per second

  pinMode(Trigger, OUTPUT);  //Definition des Arduino Pins für den Trigger als Ausgang
  pinMode(Echo, INPUT);  //Definition des Arduino Pins für das Echo als Eingang
  
  Motor.setSpeed(10); //Festlegen der Geschwindigkeit des Schrittmotors auf 10 Umdrehungen pro Minute
}

void loop()
{.
  Serial.println("Hochfahren"); //Ausgabe der Nachricht "Hochfahren" am seriellen Monitor
      
  Hochfahren(); //Ausführen der Funktion "Hochfahren"

  delay(1000);  //Es soll eine Sekunde Pause zwischen dem Ausführen der Funktionen liegen
  
  Serial.println("Runterfahren"); //Ausgabe der Nachricht "Runterfahren" am seriellen Monitor
      
  Runterfahren(); //Ausführen der Funktion "Runterfahren"

  delay(1000);  //Es soll eine Sekunde Pause zwischen dem Ausführen der Funktionen liegen
}
