int Eingang = A0; //ArduinoPin A0 für den Helligkeitssensor festgelegt
int Helligkeit = 0; //Variable „Helligkeit“ erstellt, unter der später der Messwert gespeichert wird (Startwert 0)

void setup()
{
  Serial.begin(9600); //Starten der seriellen Kommunikation mit 9600 bauds per second
}

void loop()
{
  Helligkeit =analogRead(Eingang);  //Auslesen der Spannung am Fotowiderstand und Speicherung unter "Helligkeit"

  Serial.print("Helligkeit: "); //Ausgabe der gemessenen Helligkeit am seriellen Monitor
  Serial.println(Helligkeit);

  delay(1000);  //Jede Sekunde soll die Helligkeit ausgelesen werden
}
