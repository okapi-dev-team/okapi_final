#include <Stepper.h>  //Stepper Bibliothek für Schrittmotor einbinden
#include <DS3231.h> //DS3231 Bibliothek für RTC DS3231 einbinden
#include <IRremote.h> //IRremote Bibliothek für IR Fernbedinung einbinden

int Trigger = 8;  //ArduinoPin 8 für den Trigger-Pin des Ultraschallsensors festgelegt
int Echo = 9; //ArduinoPin 9 für den Echo-Pin des Ultraschallsensors festgelegt
int SPU = 2048;  //Schritte pro Umdrehung des Schrittmotors auf 2048 festgelegt
int ZeitHochfahren[3] = {7, 30, 0};  //Array mit 3 int Variablen definiert und Uhrzeit 7:30:00 Uhr zugewiesen.
int ZeitRunterfahren[3] = {20, 0, 0}; //Array mit 3 int Variablen definiert und Uhrzeit 20:00:00 Uhr zugewiesen.
int Eingang = A0; //ArduinoPin A0 für den Helligkeitssensor festgelegt
int Helligkeit = 0; //Variable „Helligkeit“ erstellt, unter der später der Messwert gespeichert wird (Startwert 0)
int Infrarot = 11;  //ArduinoPin 11 für den Infrarotsensor festgelegt
int AlarmanlagenStatus = 0; //AlarmanlagenStatus (0 = ausgeschaltet, 1 = eingeschaltet)
long Dauer = 0; //Variable "Dauer" erstellt, unter der die Zeit gespeichert wird, die eine Schallwelle bis zur Reflektion und zurück benötigt (Startwert 0)
long Entfernung = 0;  //Variable "Entfernung" erstellt, unter der die berechnete Entfernung gespeichert wird (Startwert 0)

Stepper Motor(SPU, 3,5,4,6);  //Objekt für Stepper Initialisierung mit Bezeichnung "Motor" und Angabe der Anschluss-Pins

DS3231  rtc(SDA, SCL);  //Objekt für RTC Initalisierung mit Bezeichnung "rtc", SDA Pin und SCL Pin

Time  Zeit; //Objekt für Initialiserung einer Zeitstruktur

IRrecv irrecv(Infrarot);  //Objekt für IRremote Initalisierung an Pin 11

decode_results Ergebnis;  //Speicherung des empfangenen Daten durch Infrarot unter "Ergebnis"

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
  
  rtc.begin();  //Starten der Kommunikation mit der Echtzeituhr

  irrecv.enableIRIn();   //Initialisierung des Infrarotempfängers
}

void loop()
{
  if(AlarmanlagenStatus == 1)  //Wenn die Alarmanlage eingeschaltet ist, soll ...
  {
    Helligkeit =analogRead(Eingang);  //... die Spannung am Fotowiderstand ausgelesen werden und "Helligkeit" gespeichert werden

    Serial.print("Helligkeit: "); //... die gemessene Helligkeit am seriellen Monitor ausgegeben werden
    Serial.println(Helligkeit);
    
    Zeit = rtc.getTime(); //... die Zeit von der RTC ausgelesen werden und unter "Zeit" gespeichert werden

    Serial.print(Zeit.hour);  //... die Zeit von der RTC am seriellen Monitor ausgegeben werden
    Serial.print(":");
    Serial.print(Zeit.min);
    Serial.print(":");
    Serial.println(Zeit.sec);

    delay(1000);  //... die Uhrzeit und Helligkeit jede Sekunde überprüft werden

    if(((Zeit.hour >= ZeitHochfahren[0] && Zeit.hour < ZeitRunterfahren[0]) && Zeit.min >= ZeitHochfahren[1] && Zeit.sec >= ZeitHochfahren[2]) || Helligkeit >= 100) //... wenn die Zeit der RTC mit der Zeit fürs Hochfahren übereinstimmt oder die Helligkeit größer gleich 100 ist ...
    {
      Serial.println("Hochfahren"); //... die Nachricht "Hochfahren" am seriellen Monitor ausgegeben werden
      
      Hochfahren(); //... die Funktion "Hochfahren" ausgeführt werden
    }
    
    if((Zeit.hour >= ZeitRunterfahren[0] && Zeit.min >= ZeitRunterfahren[1] && Zeit.sec >= ZeitRunterfahren[2]) || Helligkeit < 100) //... wenn die Zeit der RTC mit der Zeit fürs Runterfahren übereinstimmt oder die Helligkeit unter 100 fällt ...
    {
      Serial.println("Runterfahren"); //... die Nachricht "Runterfahren" am seriellen Monitor ausgegeben werden
      
      Runterfahren(); //... die Funktion "Runterfahren" ausgeführt werden
    }
  }
  else  //Ansonsten ...
  {
    if (irrecv.decode(&Ergebnis)) //... sollen wenn Daten am Infrarotempfänger empfangen wurden ...
    {
      Serial.println(Ergebnis.value, DEC);  //... diese als Dezimalzahl am seriellen Monitor ausgegeben werden
  
      if (Ergebnis.value == 16724175) //... und wenn die Daten mit der Zahl "16724175" übereinstimmen (Taste "1" der Fernbedingung)
      {
        Serial.println("Hochfahren"); //... die Nachricht "Hochfahren" am seriellen Monitor ausgegeben werden
        
        Hochfahren(); //... die Funktion "Hochfahren" ausgeführt werden
      }
  
      if (Ergebnis.value == 16718055) //... und wenn die Daten mit der Zahl "16718055" übereinstimmen (Taste "2" der Fernbedingung)
      {
        Serial.println("Runterfahren"); //... die Nachricht "Runterfahren" am seriellen Monitor ausgegeben werden
        
        Runterfahren(); //... die Funktion "Runterfahren" ausgeführt werden
      }
  
      irrecv.resume();  //... soll der IR-Empfänger den nächsten Wert einlesen
    }
  }
}
