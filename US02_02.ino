
int Bewegungsmelder=7;  // ArduinoPin 7 für den "Bewegungsmelder" festgelegt.
int Bewegungsstatus=0;  // Der Variabel „Bewegungsstatus“ wird der Wert 0 zugeordnet. (Später wird hier gespeichert, ob eine Bewegung detektiert wird oder nicht.)
int LED=4;  // ArduinoPin 4 für den "LED" festgelegt.
int Lautsprecher=5; // ArduinoPin 5 für den "Lautsprecher" festgelegt.


void setup()
{
  pinMode(Bewegungsmelder, INPUT);  // ArduinoPin 7 "Bewegungsmelder" ist ein Eingang.
  pinMode(LED, OUTPUT); // ArduinoPin 4 "LED" ist ein Ausgang.
  pinMode(Lautsprecher,OUTPUT); // ArduinoPin 5 "Lautsprecher" ist ein Ausgang.
}


void loop()
{
  Bewegungsstatus=digitalRead(Bewegungsmelder); // ArduinoPin 7 wird ausgelesen und das Ergebnis für den "Bewegungstatus" wird mit dem Wert "HIGH" für 5 Volt oder "LOW" für 0 Volt gespeichert.
  
  if (Bewegungsstatus == HIGH)  // Bei der Detektion einer Bewegung wird das Spannungsignal auf "HIGH" gesetzt und ...
  {
    digitalWrite(LED, HIGH);  // ... die LED angeschaltet.
    digitalWrite(Lautsprecher, HIGH); // ... der Lautsprecher angeschaltet.
  }
  
  digitalWrite(LED, LOW); // ... die LED ausgeschaltet.
  digitalWrite(Lautsprecher, LOW);  // ... der Lautsprecher ausgeschaltet.

}
