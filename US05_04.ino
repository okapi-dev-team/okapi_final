int messBodenfeuchtigkeit = 0; //Variable "messBodenfeuchtigkeit" erstellt, unter der später der Messwert gespeichert wird (Startwert 0)
int minBodenfeuchtigkeit = 30;  //Minimale Bodenfeuchtigkeit auf 30% festlegt
int Bodenfeuchtigkeit = 0;  //Variable "Bodenfeuchtigkeit" erstellt, unter der der auf die neue Skala umgewandeltete Messwert gespeichert wird (Startwert 0)

void setup() 
{
  Serial.begin(9600); //Starten der seriellen Kommunikation mit 9600 bauds per second
}

void loop()
{
  messBodenfeuchtigkeit=analogRead(A1); //Auslesen der Bodenfeuchtigkeit aus dem Sensor und Speicherung unter der Variable "messBodenfeuchtigkeit"

  Bodenfeuchtigkeit=map(messBodenfeuchtigkeit,0,700,0,100); //Umwandeln des Messwerts der Bodenfeuchtigkeit auf eine Skala von 0-100% und Speicherung unter der Variable "Bodenfeuchtigkeit" (0% = trocken, 100% = feucht)

  if(Bodenfeuchtigkeit > 100) //Wenn der umgewandelte Messwert der Bodenfeuchtigkeit die 100% überschreitet, soll ...
  {
    Bodenfeuchtigkeit = 100;  //... die Bodenfeuchtigkeit auf 100% gesetzt werden
  }

  Serial.print("Bodenfeuchtigkeit: "); //Das Wort "Bodenfeuchtigkeit" mit Messwert und Einheit wird im seriellen Monitor ausgegeben
  Serial.print(Bodenfeuchtigkeit);
  Serial.println(" %\t");
}
