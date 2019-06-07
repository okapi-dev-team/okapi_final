 
int Bewegungsmelder=7;  // ArduinoPin 7 für den "Bewegungsmelder" festgelegt.
int Bewegungsstatus=0;  // Der Variable „Bewegungsstatus“ wird der Wert 0 zugeordnet. (Später wird hier gespeichert, ob eine Bewegung detektiert wird oder nicht.)


void setup()
{
  pinMode(Bewegungsmelder, INPUT);  // ArduinoPin 7 "Bewegungsmelder" ist ein Eingang.
}


void loop()
{
  Bewegungsstatus=digitalRead(Bewegungsmelder); // ArduinoPin 7 wird ausgelesen und das Ergebnis für den "Bewegungstatus" wird mit dem Wert "HIGH" für 5 Volt oder "LOW" für 0 Volt gespeichert.

  if (Bewegungsstatus == HIGH)  // Bei der Detektion einer Bewegung wird das Spannungsignal auf "HIGH" gesetzt und ...
  {
  
  }

}
