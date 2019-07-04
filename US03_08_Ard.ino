const int TASTER1_PIN=2;  //Arduino Pin für Taster 1
const int TASTER2_PIN=3;  //Arduino Pin für Taster 2
const int LED_PIN_GRUEN=5;  //Arduino Pin für LED 1 (grün)
const int LED_PIN_BLAU=6; //Arduino Pin für LED 2 (blau)

int taster1Status=0;  //Tasterstatus Taster 1 (0=nicht gedrückt, 1=gedrückt)
int taster2Status=0;  //Tasterstatus Taster 2 (0=nicht gedrückt, 1=gedrückt)

int sensor1=50; //Platzhalter für auszulesende Sensorwerte
int sensor2=100;
int sensor3=150;
int sensor4=200;

int benutzer=0; //Benutzerrechte (0=slave, 1=master)
                //hier fest auf slave gesetzt zu Testzwecken


void setup()
{
  Serial.begin(9600); //Starten der seriellen Kommunikation mit 9600 bauds per second
  while(!Serial)  //Solange keine serielle Kommunikation aufgebaut wurde...
  {
    ; //nichts tun
  }

  pinMode(TASTER1_PIN,INPUT); //Definition des Arduino Pins für den Taster 1 als Eingang
  pinMode(TASTER2_PIN,INPUT); //Definition des Arduino Pins für den Taster 2 als Eingang
  pinMode(LED_PIN_GRUEN,OUTPUT); //Definition des Arduino Pins für LED 1 (grün) als Ausgang
  pinMode(LED_PIN_BLAU,OUTPUT); //Definition des Arduino Pins für LED 2 (blau) als Ausgang
  
  seriellerKontakt(); //Durchführung serieller Handshake
}

void loop()
{
  if(Serial.available()>0)  //Wenn Daten am seriellen Port vorliegen, dann...
  {
    char eingangsByte=Serial.read();  //Speichern des Inhalts in die char-Variable eingangsByte
    
    if(eingangsByte=='1') //Wenn eingangsByte identisch '1' ist, dann...
    {
      digitalWrite(LED_PIN_GRUEN,HIGH); //Schalten des Arduino Pins (Spannung = HIGH (5V)) für LED 1 (grün) -> LED leuchtet
    }
    else  //Sonst...
    {
      digitalWrite(LED_PIN_GRUEN,LOW); //Schalten des Arduino Pins (Spannung = LOW (0V)) für LED 1 (grün) -> LED leuchtet nicht
    }
    
    if(eingangsByte=='2') //Wenn eingangsByte identisch '2' ist, dann...
    {
      digitalWrite(LED_PIN_BLAU,HIGH); //Schalten des Arduino Pins (Spannung = HIGH (5V)) für LED 2 (blau) -> LED leuchtet
    }
    else  //Sonst...
    {
      digitalWrite(LED_PIN_BLAU,LOW); //Schalten des Arduino Pins (Spannung = HIGH (5V)) für LED 2 (blau) -> LED leuchtet nicht
    }

    if(digitalRead(TASTER1_PIN)==HIGH)  //Wenn Taster 1 gedrückt, dann...
    {
      taster1Status=1;  //setze taster1Status=1
    }
    else  //Sonst...
    {
      taster1Status=0;  //setze taster1Status=0
    }

    if(digitalRead(TASTER2_PIN)==HIGH)  //Wenn Taster 2 gedrückt, dann...
    {
      taster2Status=1;  //setze taster2Status=1
    }
    else  //Sonst...
    {
      taster2Status=0;  //setze taster2Status=0
    }
    
    Serial.write(taster1Status);  //Ausgabe von taster1Status in seriellen Port
    Serial.write(taster2Status);  //Ausgabe von taster2Status in seriellen Port

    Serial.write(sensor1);  //Ausgabe von sensor1 in seriellen Port
    Serial.write(sensor2);  //Ausgabe von sensor2 in seriellen Port
    Serial.write(sensor3);  //Ausgabe von sensor3 in seriellen Port
    Serial.write(sensor4);  //Ausgabe von sensor4 in seriellen Port

    Serial.write(benutzer);  //Ausgabe von benutzer in seriellen Port
  }  
}

/** \brief Durchführung serieller Handshake (Arduino <-> Processing)
 * 
 */
void seriellerKontakt()
{
  while(Serial.available()<=0)  //Solange keine Daten am seriellen Port vorliegen, ...
  {
    Serial.print('A');  //schreibe 'A' in seriellen Port
    delay(300); //warte 300 Millisekunden
  }
}
