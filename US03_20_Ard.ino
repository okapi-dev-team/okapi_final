#include <SPI.h>  //SPI Bibliothek für RFID-Leser einbinden
#include <MFRC522.h>  //MFRC522 Bibliothek für RFID-Leser einbinden
#include <Wire.h> //Wire Bibliothek für Display einbinden
#include <LiquidCrystal_I2C.h>  //LiquidCrystal I2C Bibliothek für I2C-Kommunikation mit Display einbinden
#include <Servo.h>  //Servo Bibliothek für Servo einbinden
#include "DHT.h"  //DHT Bibliothek für DHT22 Sensor einbinden
#include <Stepper.h>  //Stepper Bibliothek für Schrittmotor einbinden
#include <DS3231.h> //DS3231 Bibliothek für RTC DS3231 einbinden
#include <IRremote.h> //IRremote Bibliothek für IR Fernbedinung einbinden

const int SDA_PIN=53; //RFID SDA Pin
const int RST_PIN=2;  //RFID RST Pin
const int TASTER_EIN=10;  //Arduino Pin für Alarmanlage EIN
const int TASTER_AUS=11;  //Arduino Pin für Alarmanlage AUS
const int BEWEGUNGSMELDER=12; //Arduino Pin für Bewegungsmelder
const int ALARM_LED=13; //Arduino Pin für Signal LED (Alarm)
const int LED_PIN_GRUEN=6;  //Arduino Pin für LED 1 (grün)
const int LED_PIN_BLAU=7; //Arduino Pin für LED 2 (blau)
const int SIRENE=5; //Arduino Pin für Sirene
const int SERVO=8;  //Arduino Pin für Servo
const int DHT_PIN=A0; //Arduino Pin für DHT Sensor
const int BODENFEUCHTIGKEIT_SENSOR=A1;  //Arduino Pin für Bodenfeuchtigkeitssensor
const int MAX_LUFTFEUCHTIGKEIT=60; //Maximale Luftfeuchtigkeit auf 60% festlegt
const int MAX_TEMPERATUR=30; //Maximale Temperatur auf 30°C festlegt
const int MIN_BODENFEUCHTIGKEIT=30;  //Minimale Bodenfeuchtigkeit auf 30% festlegt
const int BODEN_LED=4;  //Arduino Pin für Signal LED (Bodenfeuchtigkeit)
const int TRIGGER=23;  //Arduino Pin für den Trigger-Pin des Ultraschallsensors festgelegt
const int ECHO=25; //Arduino Pin für den Echo-Pin des Ultraschallsensors festgelegt
const int SPU=2048;  //Schritte pro Umdrehung des Schrittmotors auf 2048 festgelegt
const int FOTODIODE=A2; //Arduino Pin für den Helligkeitssensor festgelegt
const int IR_DIODE=29;  //Arduino Pin für den Infrarotsensor festgelegt

int tasterEinStatus=0;  //Tasterstatus EIN Taster (LOW=nicht gedrückt, HIGH=gedrückt)
int tasterAusStatus=0;  //Tasterstatus AUS Taster (LOW=nicht gedrückt, HIGH=gedrückt)
int alarmanlageStatus=0;  //Status der Alarmanlage (0=ausgeschaltet, 1=eingeschaltet)
int bewegungsStatus=0;  //Status Bewegungsmelder (LOW=keine Bewegung registriert, HIGH=Bewegung registriert)
int messBodenfeuchtigkeit=0; //Variable "messBodenfeuchtigkeit" erstellt, unter der später der Messwert gespeichert wird (Startwert 0)
int bodenfeuchtigkeit=0;  //Variable "bodenfeuchtigkeit" erstellt, unter der der auf die neue Skala umgewandeltete Messwert gespeichert wird (Startwert 0)
int temperatur=0; //Variable für gemessene Temperatur
int luftfeuchtigkeit=0; //Variable für gemessene Luftfeuchtigkeit
int helligkeit=0; //Variable „helligkeit“ erstellt, unter der später der Messwert gespeichert wird (Startwert 0)

long dauer=0; //Variable "dauer" erstellt, unter der die Zeit gespeichert wird, die eine Schallwelle bis zur Reflektion und zurück benötigt (Startwert 0)
long entfernung=0;  //Variable "entfernung" erstellt, unter der die berechnete Entfernung gespeichert wird (Startwert 0)

int rfidRead[4]={0};  //int Array für 4 Blöcke der RFID-Karten ID
String rfidFinalStr="";  //String für komplette RFID-Karten ID

int datenSenden[10]={0};  //int Array für zu sendende Daten Arduino->Processing
int datenEmpfangen[1]={0};  //int Array für zu empfangende Daten Processing->Arduino

int zeitHochfahren[3]={7,30,0};  //Array mit 3 int Variablen definiert und Uhrzeit 7:30:00 Uhr zugewiesen
int zeitRunterfahren[3]={20,0,0}; //Array mit 3 int Variablen definiert und Uhrzeit 20:00:00 Uhr zugewiesen

bool fenster=false; //toggle für Fensterposition (false=geschlossen, true=geöffnet)
 
MFRC522 mfrc522(SDA_PIN, RST_PIN);  //Objekt für RFID-Leser Initalisierung mit SDA Pin und RST Pin

LiquidCrystal_I2C lcd(0x27,20,2); //Objekt für Display Initalisierung mit I2C Adresse, Zeichen pro Zeile und Zeilen

Servo servoFenster; //Objekt für Servo Initalisierung

DHT dht; //Objekt für DHT Initalisierung mit Bezeichnung

Stepper motor(SPU,33,37,35,39);  //Objekt für Stepper Initialisierung mit Bezeichnung "motor" und Angabe der Anschluss-Pins

DS3231  rtc(SDA, SCL);  //Objekt für RTC Initalisierung mit Bezeichnung "rtc", SDA Pin und SCL Pin

Time  zeit; //Objekt für Initialiserung einer Zeitstruktur

IRrecv irrecv(IR_DIODE);  //Objekt für IRremote Initalisierung

decode_results ergebnis;  //Speicherung des empfangenen Daten durch Infrarot unter "ergebnis"

class User  //Initalisierung einer Klasse User mit privaten Attributen und öffentlichen Methoden
{
  private:
    String nameStr,rechteStr,codeStr,rfidUserStr;
    String user[4];
   
  public:
    void userAnlegen(String nameStr, String rechteStr, String codeStr, String rfidUserStr);
    void userAusgeben();
    void userZugang();
};

User user1,user2; //Definition der Objekte user1 und user2 in Klasse User

/** \brief Legt ein String Array mit Strings nameStr, rechteStr, codeStr, rfidUserStr an
 *  \param nameStr Name des Benutzers
 *  \param rechteStr Zugriffsrechte des Benutzers (master oder slave)
 *  \param codeStr Pin-Code des Benutzers
 *  \param rfidUserStr RFID-Karten ID der Benutzer RFID-Karte
 */
void User::userAnlegen(String nameStr, String rechteStr, String codeStr, String rfidUserStr)
{
  user[0]=nameStr;
  user[1]=rechteStr;
  user[2]=codeStr;
  user[3]=rfidUserStr;
}

/** \brief Gibt die angelegten Nutzerkonten im seriellen Monitor aus
 *  
 */
void User::userAusgeben()
{
  for(int i=0;i<4;i++)  //Schleife über alle Einträge des user Arrays
  {
    Serial.println(user[i]);
  }
}

/** \brief Erkennt den Nutzer anhand seiner RFID-Karten ID, stellt bei vorhandenen Rechten die Zugriffsrechte auf volle Rechte und begrüßt Benutzer auf Display
 *  
 */
void User::userZugang()
{
  if(user[3]==rfidFinalStr) //Identifizierung des Benutzers über Abgleich der ausgelesenen RFID-Karten ID und hinterlegten RFID-Karten IDs
  {
    if(user[0]=="Nutzer1") //Wenn Benutzername gleich Nutzer1 ist, dann...
    {
      datenSenden[2]=1; //hat sich Nutzer1 angemeldet
      
      if(user[1]=="master") //Wenn Nutzer1 master Zugriffsrechte hat, dann..
      {
        datenSenden[3]=1; //Zugriffsrechte auf master setzen
      }
      if(user[1]=="slave")  //Wenn Nutzer1 slave Zugriffsrechte hat, dann...
      {
        datenSenden[3]=2; //Zugriffsrechte auf slave setzen
      }
    }

    if(user[0]=="Nutzer2")  //Nutzer2 analog zu Nutzer1
    {
      datenSenden[4]=1;

      if(user[1]=="master")
      {
        datenSenden[5]=1;
      }
      if(user[1]=="slave")
      {
        datenSenden[5]=2;
      }
    }

    //Serial.println("Zugang für "+user[0]);

    lcd.clear();  //Löschen des LCD-Display Inhalts
    lcd.setCursor(5,0); //Setzen des Cursors auf die 5. Stelle in der 1. Zeile
    lcd.print("Hallo"); //Schreiben des Texts "Hallo"
    lcd.setCursor(5,1); //Setzen des Cursors auf die 5. Stelle in der 2. Zeile
    lcd.print(user[0]); //Schreiben des Benutzernames

    delay(1000);  //Anzeige des Benutzernames für 1 Sekunde
  }

  lcd.clear();  //Löschen des LCD-Display Inhalts
}

/** \brief Status der Alarmanlage verändern und auf dem Display ausgeben
 *  
 */
void alarmanlageTaster()
{
  tasterEinStatus=digitalRead(TASTER_EIN);  //Auslesen des digitalen Pins (Pin für Taster Ein) und Speichern in tasterEinStatus (HIGH oder LOW)
  tasterAusStatus=digitalRead(TASTER_AUS);  //Auslesen des digitalen Pins (Pin für Taster Aus) und Speichern in tasterAusStatus (HIGH oder LOW)
  
  if(tasterEinStatus==HIGH) //Wenn Taster Ein gedrückt, dann...
  {
    alarmanlageStatus=1;  //Setzen des Alarmanlagenstatus auf 1=eingeschaltet
    lcd.clear();  //Löschen des LCD-Display Inhalts
    lcd.setCursor(2,0); //Setzen des Cursors auf die 2. Stelle in der 1. Zeile
    lcd.print("Alarmanlage"); //Schreiben des Texts "Alarmanlage"
    lcd.setCursor(1,1); //Setzen des Cursors auf die 1. Stelle in der 2. Zeile
    lcd.print("eingeschaltet"); //Schreiben des Texts "eingeschaltet"
    delay(1000);  //Anzeige des Alarmanlagenstatus für 1 Sekunde
  }
  
  if((tasterAusStatus==HIGH)&&(datenSenden[3]==1||datenSenden[5]==1))  //Wenn Taster Aus gedrückt und Nutzerrechte=master, dann...
  {
    alarmanlageStatus=0;  //Setzen des Alarmanlagenstatus auf 0=ausgeschaltet
    digitalWrite(ALARM_LED,LOW);  //Ausschalten der Alarm LED
    digitalWrite(SIRENE,LOW); //Ausschalten der Sirene    
    lcd.clear();  //Löschen des LCD-Display Inhalts
    lcd.setCursor(2,0); //Setzen des Cursors auf die 2. Stelle in der 1. Zeile
    lcd.print("Alarmanlage"); //Schreiben des Texts "Alarmanlage"
    lcd.setCursor(1,1); //Setzen des Cursors auf die 1. Stelle in der 2. Zeile
    lcd.print("ausgeschaltet"); //Schreiben des Texts "ausgeschaltet"
    delay(1000);  //Anzeige des Alarmanlagenstatus für 1 Sekunde
  }

  lcd.clear();  //Löschen des LCD-Display Inhalts

  datenSenden[0]=alarmanlageStatus; //aktuellen Alarmanlagenstatus setzen
}

/** \brief vorhandene RFID-Karte auslesen, blockweise in Array speichern und gesamte Karten ID als String speichern 
 *  
 */
void rfidScan()
{
  if(mfrc522.PICC_IsNewCardPresent()&&mfrc522.PICC_ReadCardSerial())  //Wenn RFID-Karte vom Scanner erfasst ist und ausgelesen wird, dann...
  {
    for(int i=0;i<4;i++) //jeden Karten ID Block in Array Element speichern
    {
      rfidRead[i]=mfrc522.uid.uidByte[i];
    }
  }

  rfidFinalStr=String(rfidRead[0])+String(rfidRead[1])+String(rfidRead[2])+String(rfidRead[3]); //alle Elemente des Arrays zu String zusammenfügen
}

/** \brief Bewegungsmelder auslesen und bei Auslösen in datenSenden Array schreiben. Ist die Alarmanlage angeschaltet, wird bei Auslösen eine Signal LED eingeschaltet
 * 
 */
void bewegungsmelder()
{
  bewegungsStatus=digitalRead(BEWEGUNGSMELDER); //Auslesen des Bewegungsmelders
  if(bewegungsStatus==HIGH) //Wenn Bewegung detektiert, dann...
  {
    datenSenden[1]=1; //Alarm in datenSenden Array setzen
    if(alarmanlageStatus==1)  //Wenn Alarmanlage eingeschaltet ist, dann...
    {
      digitalWrite(ALARM_LED,HIGH); //Signal LED einschalten
      digitalWrite(SIRENE,HIGH);  //Sirene einschalten
    }
  }
}

/** \brief Misst die aktuelle Temperatur und gibt sie zurück
 *  \return Temperatur als int-Variable
 */
int messenTemperatur()
{
  
  temperatur=dht.getTemperature(); //Auslesen der Temperatur aus dem Sensor und Speicherung unter der Variable "temperatur"
  
  if(dht.getStatusString()!="OK") //Kontrolle des Messwerts auf Sinnhaftigkeit. Wenn Status ungleich "OK", dann...
  {
    return 0; //Rückgabe Wert 0
  }
  else  //Sonst...
  {
    return temperatur; //Rückgabe der gemessenen Temperatur
  }
}

/** \brief Misst die aktuelle Luftfeuchtigkeit und gibt sie zurück
 *  \return Luftfeuchtigkeit als int-Variable
 */
int messenLuftfeuchtigkeit()
{
  luftfeuchtigkeit=dht.getHumidity(); //Auslesen der Luftfeuchtigkeit aus dem Sensor und Speicherung unter der Variable "luftfeuchtigkeit"
  
  if(dht.getStatusString()!="OK") //Kontrolle des Messwerts auf Sinnhaftigkeit. Wenn Status ungleich "OK", dann...
  {
    return 0; //Rückgabe Wert 0
  }
  else  //Sonst
  {
    return luftfeuchtigkeit; //Rückgabe der gemessenen Luftfeuchtigkeit
  }
}

/** \brief Misst die aktuelle Bodenfeuchtigkeit und gibt sie zurück
 *  \return Bodenfeuchtigkeit als int-Variable
 */
int messenBodenfeuchtigkeit()
{
  messBodenfeuchtigkeit=analogRead(BODENFEUCHTIGKEIT_SENSOR); //Auslesen der Bodenfeuchtigkeit aus dem Sensor und Speicherung unter der Variable "messBodenfeuchtigkeit"
  delay(10);  //ADC delay
  bodenfeuchtigkeit=map(messBodenfeuchtigkeit,0,150,0,100); //Umwandeln des Messwerts der Bodenfeuchtigkeit auf eine Skala von 0-100% und Speicherung unter der Variable "bodenfeuchtigkeit" (0% = trocken, 100% = feucht)

  if(bodenfeuchtigkeit>100) //Wenn Bodenfeuchtigkeit den Wert 100 überschreitet, dann...
  {
    bodenfeuchtigkeit=100;  //setze sie fest auf 100
  }
  
  return bodenfeuchtigkeit; //Rückgabe der gemessenen Bodenfeuchtigkeit
}

/** \brief  Berechnet die Entfernung zu einem Gegenstand anhand einer Ultraschallwelle
 *  \return entfernung als long integer
 */
long abstandsbestimmung()
{
  digitalWrite(TRIGGER,LOW); //Spannung vom Trigger-Pin für rauschfreies Signal beim späteren Senden genommen
  delay(5); //Dauer: 5 Millisekunden
  digitalWrite(TRIGGER,HIGH);  //Aussendung einer Ultraschallwelle
  delay(10);  //Ausgabe Ultraschallwelle für 10 Millisekunden
  digitalWrite(TRIGGER,LOW); //Aussachalter der Ultraschallwelle
  
  dauer=pulseIn(ECHO,HIGH);  //Messung der Zeit in Mikrosekunden bis der Schall zum Ultraschallsensor zurückkehrt und Speicherung des Ergebnis unter "dauer"
  entfernung=(dauer/2)*0.03432; //Berechnung der Entfernung in Zentimetern (Teilung durch zwei: Berechnung von nur einer Strecke; Multiplizierung mit der Schallgeschwindigkeit in der Einheit Zentimeter/Mikrosekunde)

  return entfernung;  //Rückegabe der Entfernung
}

/** \brief Fährt den Schrittmotor bis ein vorgegebener Abstand erreicht wurde
 *  
 */
void runterfahren()
{
  if(abstandsbestimmung()<=10)  //Wenn die gemessene Entfernung bei 10cm oder weniger liegt, soll...
  {
    ; //nichts geschehen
  }
  else  //Ansonsten...
  {
    do  //... soll der Schrittmotor solange fahren, bis ...
    {
      motor.step(2048);  //Der Schrittmotor macht 2048 Schritte (eine Umdrehung) gegen den Uhrzeigersinn
    }
    while(abstandsbestimmung()>10); //... die gemessene Entfernung bei mehr als 10cm liegt
  }
}

/** \brief Fährt den Schrittmotor bis ein vorgegebener Abstand erreicht wurde
 *  
 */
void hochfahren()
{
  if(abstandsbestimmung()>=30)  //Wenn die gemessene Entfernung bei 30cm oder mehr liegt, soll...
  {
    ; //nichts geschehen
  }
  else  //Ansonsten...
  {
    do  //... soll der Schrittmotor solange fahren, bis ...
    {
      motor.step(-2048);  //Der Schrittmotor macht 2048 Schritte (eine Umdrehung) im Uhrzeigersinn
    }
    while(abstandsbestimmung()<30); //... die gemessene Entfernung bei weniger als 30cm liegt
  }
}

/** \brief Zeitschaltung/Dämmerungsschalter für Jalousiesteuerung und Bedienung per IR-Fernbedienung
 * 
 */
void jalousieSteuerung()
{
  if(alarmanlageStatus==1)  //Wenn die Alarmanlage eingeschaltet ist, soll ...
  {
    if(((zeit.hour>=zeitHochfahren[0]&&zeit.hour<zeitRunterfahren[0])&&zeit.min>=zeitHochfahren[1]&&zeit.sec>=zeitHochfahren[2])||helligkeit>=100)  //... wenn die Zeit der RTC mit der Zeit fürs Hochfahren übereinstimmt oder die Helligkeit größer gleich 100 ist ...
    {
      hochfahren(); //... die Funktion "hochfahren" ausgeführt werden
    }
    
    if((zeit.hour>=zeitRunterfahren[0]&&zeit.min>=zeitRunterfahren[1]&&zeit.sec>=zeitRunterfahren[2])||helligkeit<100) //... wenn die Zeit der RTC mit der Zeit fürs Runterfahren übereinstimmt oder die Helligkeit unter 100 fällt ...
    {
      runterfahren(); //... die Funktion "runterfahren" ausgeführt werden
    }
  }
  else  //Ansonsten ...
  {
    if(irrecv.decode(&ergebnis)) //... sollen wenn Daten am Infrarotempfänger empfangen wurden ...
    {
      if(ergebnis.value==16724175) //... und wenn die Daten mit der Zahl "16724175" übereinstimmen (Taste "1" der Fernbedingung)
      {
        hochfahren(); //... die Funktion "Hochfahren" ausgeführt werden
      }
  
      if(ergebnis.value==16718055) //... und wenn die Daten mit der Zahl "16718055" übereinstimmen (Taste "2" der Fernbedingung)
      {
        runterfahren(); //... die Funktion "Runterfahren" ausgeführt werden
      }
  
      irrecv.resume();  //... soll der IR-Empfänger den nächsten Wert einlesen
    }
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

void setup()
{
  Serial.begin(9600); //Starten der seriellen Kommunikation mit 9600 bauds per second
  while(!Serial)  //Solange keine serielle Kommunikation aufgebaut wurde...
  { //nichts tun
  }
  rtc.begin();  //Starten der Kommunikation mit der Echtzeituhr
  
  SPI.begin();  //Starten der SPI Kommunikation
  mfrc522.PCD_Init(); //Initalisierung des RFID-Karten Lesers

  dht.setup(DHT_PIN);  //Starten der Kommunikation mit dem DHT Sensor

  lcd.init(); //Initalisierung des LCD-Displays                    
  lcd.backlight();  //Einschalten der Hintergrundbeleuchtung des LCD-Displays

  pinMode(TASTER_EIN,INPUT); //Definition des Arduino Pins für den Taster Ein als Eingang
  pinMode(TASTER_AUS,INPUT); //Definition des Arduino Pins für den Taster Aus als Eingang
  pinMode(BEWEGUNGSMELDER,INPUT); //Definition des Arduino Pins für den Bewegungsmelder als Eingang
  pinMode(ALARM_LED,OUTPUT); //Definition des Arduino Pins für die Signal LED (Alarm) als Ausgang
  pinMode(LED_PIN_GRUEN,OUTPUT); //Definition des Arduino Pins für die LED_GRUEN als Ausgang
  pinMode(LED_PIN_BLAU,OUTPUT); //Definition des Arduino Pins für die LED_BLAU als Ausgang
  pinMode(SIRENE,OUTPUT); //Definition des Arduino Pins für die Sirene als Ausgang
  pinMode(BODEN_LED,OUTPUT);  //Definition des Arduino Pins für die Siganl LED (Bodenfeuchtigkeit) als Ausgang
  pinMode(TRIGGER,OUTPUT);  //  //Definition des Arduino Pins für den Trigger als Ausgang
  pinMode(ECHO,INPUT);  //Definition des Arduino Pins für das Echo als Eingang

  servoFenster.attach(SERVO); //Definition des Arduino Pins für den Servo
  servoFenster.write(0);  //Startposition 0°=Fenster geschlossen

  motor.setSpeed(10); //Festlegen der Geschwindigkeit des Schrittmotors auf 10 Umdrehungen pro Minute

  irrecv.enableIRIn();   //Initialisierung des Infrarotempfängers

  user1.userAnlegen("Nutzer1","master","1234","133120249101");  //Anlegen des 1. Nutzers mit Benutzername, Zugriffsrechten, Pin-Code und RFID-Karten ID
  user2.userAnlegen("Nutzer2","slave","0000","1661716019"); //Anlegen des 2. Nutzers mit Benutzername, Zugriffsrechten, Pin-Code und RFID-Karten ID

  //user1.userAusgeben(); //Ausgabe des 1. Nutzerprofils in seriellen Monitor
  //user2.userAusgeben(); //Ausgabe des 2. Nutzerprofils in seriellen Monitor

  seriellerKontakt(); //Durchführung serieller Handshake
}

void loop()
{  
  bewegungsmelder();  //Auslesen des Bewegungsmelders
  
  rfidScan(); //Suchen und ggf. Scannen von RFID-Karte
  
  user1.userZugang(); //Zugangskontrolle
  user2.userZugang(); //Zugangskontrolle

  alarmanlageTaster();  //dauerhaftes Aktualisieren der Tasterstatus und ggf. Ausgabe auf Display

  if(bodenfeuchtigkeit<MIN_BODENFEUCHTIGKEIT)  //Wenn die gemessene Luftfeuchtigkeit den Minimalwert unterschreitet, soll ...
  {
    digitalWrite(BODEN_LED, HIGH); //... die LED angeschaltet werden
  }
  else  //Ansonsten ...
  {
    digitalWrite(BODEN_LED, LOW); //... soll die LED ausgeschaltet bleiben
  }

  helligkeit=analogRead(FOTODIODE); //Auslesen der Fotodiode und speichern des Messwerts in int-Variable "helligkeit"
  
  zeit=rtc.getTime(); //Auslesen der RTC und speichern der aktuellen Uhrzeit in Objekt zeit

  jalousieSteuerung();  //Ausführen der Jalousiesteuerung

  delay(dht.getMinimumSamplingPeriod());  //Delay für Ausleserate des Sensors

  datenSenden[6]=messenTemperatur();  //Übertragen der gemessenen Temperatur
  datenSenden[7]=messenLuftfeuchtigkeit();  //Übertragen der gemessenen Luftfeuchtigkeit
  datenSenden[8]=messenBodenfeuchtigkeit();  //Übertragen der gemessenen Bodenfeuchtigkeit
  datenSenden[9]=helligkeit;  //Übertragen der gemessenen Helligkeit

  if(Serial.available()>0)  //Wenn Daten am seriellen Port vorliegen (Handshake hat stattgefunden und Processing hat 'A' zurückgesendet), dann...
  {
    datenEmpfangen[0]=Serial.read();
    if(datenEmpfangen[0]==1) //Wenn eingangsByte identisch '1' ist, dann...
    {
      fenster=true; //setze toggle auf true->geöffnet
    }
    if(datenEmpfangen[0]==2) //Wenn eingangsByte identisch '2' ist, dann...
    {
      fenster=false;  //setze toggle auf false->geschlossen
    }

    for(int i=0;i<10;i++) //schreibe Einträge des datenSenden Arrays in seriellen Port
    {
    Serial.write(datenSenden[i]);
    }
    //Serial.println();
  }

  if(fenster||(alarmanlageStatus==1&&((messenLuftfeuchtigkeit()>MAX_LUFTFEUCHTIGKEIT)||(messenTemperatur()>MAX_TEMPERATUR)))) //Wenn Fenster geöffnet werden soll (entweder Taster über GUI oder Überschreitung Messwerte Tmeperatur oder Luftfeuchtigkeit), dann...
  {
    digitalWrite(LED_PIN_BLAU,LOW); //Schalten des Arduino Pins (Spannung = HIGH (5V)) für LED 2 (blau) -> LED leuchtet nicht
    digitalWrite(LED_PIN_GRUEN,HIGH); //Schalten des Arduino Pins (Spannung = HIGH (5V)) für LED 1 (grün) -> LED leuchtet
    servoFenster.write(90); //fahre Servo in Fenster offen Position (90°)
  }
  else  //Sonst (Fenster soll geschlossen werden)
  {
    digitalWrite(LED_PIN_GRUEN,LOW); //Schalten des Arduino Pins (Spannung = LOW (0V)) für LED 1 (grün) -> LED leuchtet nicht
    digitalWrite(LED_PIN_BLAU,HIGH); //Schalten des Arduino Pins (Spannung = HIGH (5V)) für LED 2 (blau) -> LED leuchtet
    servoFenster.write(0); //fahre Servo in Fenster geschlossen Position (0°)
  }
  
  
  //delay(1000);  //warte 1 Sekunde (Begrenzung des übermittelten Datenstroms)
  
  for(int i=0;i<10;i++) //setze alle Elemente des datenSenden Arrays =0 (Daten wurden übermittelt, bereit für neue Daten)
  {
    datenSenden[i]=0;
  }

  for(int i=0;i<4;i++)  //setze gescannte RFID ID =0 (bereit für neue RFID-Karte)
  {
    rfidRead[i]=0;
  }
}
