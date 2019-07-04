int Trigger = 8;  //ArduinoPin 8 für den Trigger-Pin des Ultraschallsensors festgelegt
int Echo = 9; //ArduinoPin 9 für den Echo-Pin des Ultraschallsensors festgelegt
long Dauer = 0; //Variable "Dauer" erstellt, unter der die Zeit gespeichert wird, die eine Schallwelle bis zur Reflektion und zurück benötigt (Startwert 0)
long Entfernung = 0;  //Variable "Entfernung" erstellt, unter der die berechnete Entfernung gespeichert wird (Startwert 0)

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

void setup()
{
  Serial.begin(9600); //Starten der seriellen Kommunikation mit 9600 bauds per second

  pinMode(Trigger, OUTPUT);  //Definition des Arduino Pins für den Trigger als Ausgang
  pinMode(Echo, INPUT);  //Definition des Arduino Pins für das Echo als Eingang
}

void loop()
{
  Abstandsbestimmung(); //Ausführen der Funktion "Abstandsbestimmung"
  delay(1000);  //Messung der Entfernung einmal pro Sekunden
}
