#include <SPI.h>  //SPI Bibliothek für RFID-Leser einbinden
#include <MFRC522.h>  //MFRC522 Bibliothek für RFID-Leser einbinden
#include <Wire.h> //Wire Bibliothek für Display einbinden
#include <LiquidCrystal_I2C.h>  //LiquidCrystal I2C Bibliothek für I2C-Kommunikation mit Display einbinden

const int SDA_PIN=53; //RFID SDA Pin
const int RST_PIN=5;  //RFID RST Pin
const int tasterEin=10;  //Arduino Pin für Alarmanlage EIN
const int tasterAus=11;  //Arduino Pin für Alarmanlage AUS

int tasterEinStatus=0;  //Tasterstatus EIN Taster (LOW=nicht gedrückt, HIGH=gedrückt)
int tasterAusStatus=0;  //Tasterstatus AUS Taster (LOW=nicht gedrückt, HIGH=gedrückt)
int alarmanlageStatus=0;  //Status der Alarmanlage (0=ausgeschaltet, 1=eingeschaltet)

String rfidReadStr[4];  //String Array für 4 Hexadeximal Blöcke der RFID-Karten ID
String rfidFinalStr;  //String für komplette RFID-Karten ID

MFRC522 mfrc522(SDA_PIN, RST_PIN);  //Objekt für RFID-Leser Initalisierung mit SDA Pin und RST Pin

LiquidCrystal_I2C lcd(0x27,20,4); //Objekt für Display Initalisierung mit I2C Adresse, Zeichen pro Zeile und Zeilen

class user  //Initalisierung einer Klasse user mit privaten Attributen und öffentlichen Methoden
{
  private:
    String nameStr,rechteStr,codeStr,rfidUserStr;
    String user[4];
   
  public:
    void userAnlegen(String nameStr, String rechteStr, String codeStr, String rfidUserStr);
    void userAusgeben();
    void userZugang();
};

user user1,user2; //Definition der Objekte user1 und user2 in Klasse user

/** \brief Legt ein String Array mit Strings nameStr, rechteStr, codeStr, rfidUserStr an
 *  \param nameStr Name des Benutzers
 *  \param rechteStr Zugriffsrechte des Benutzers (master oder slave)
 *  \param codeStr Pin-Code des Benutzers
 *  \param rfidUserStr RFID-Karten ID der Benutzer RFID-Karte
 */
void user::userAnlegen(String nameStr, String rechteStr, String codeStr, String rfidUserStr)
{
  user[0]=nameStr;
  user[1]=rechteStr;
  user[2]=codeStr;
  user[3]=rfidUserStr;
}

/** \brief Gibt die angelegten Nutzerkonten im seriellen Monitor aus
 *  
 */
void user::userAusgeben()
{
  for(int i=0;i<4;i++)  //Schleife über alle Einträge des user Arrays
  {
    Serial.println(user[i]);
  }
}

/** \brief Erkennt den Nutzer anhand seiner RFID-Karten ID und gibt ihn im seriellen Monitor aus 
 *  
 */
void user::userZugang()
{
  if(user[3]==rfidFinalStr) //Identifizierung des Benutzers über Abgleich der ausgelesenen RFID-Karten ID und hinterlegten RFID-Karten IDs
  {
    Serial.println("Zugang für "+user[0]);  
  }
}

/** \brief Status der Alarmanlage verändern und auf dem Display ausgeben
 *  \param tasterEinStatus Status des Ein Tasters
 *  \param tasterAusStatus Status des Aus Tasters
 *  \return alarmanlagenStatus als integer (0=aus, 1=eingeschaltet)
 *  
 */
int alarmanlageTaster(int tasterEinStatus, int tasterAusStatus)
{
  tasterEinStatus=digitalRead(tasterEin);  //Auslesen des digitalen Pins (Pin für Taster Ein) und Speichern in tasterEinStatus (HIGH oder LOW)
  tasterAusStatus=digitalRead(tasterAus);  //Auslesen des digitalen Pins (Pin für Taster Aus) und Speichern in tasterAusStatus (HIGH oder LOW)
  
  if(tasterEinStatus==HIGH) //Wenn Taster Ein gedrückt, dann...
  {
    lcd.setCursor(2,0); //Setzen des Cursors auf die 2. Stelle in der 1. Zeile
    lcd.print("Alarmanlage"); //Schreiben des Texts "Alarmanlage"
    lcd.setCursor(1,1); //Setzen des Cursors auf die 1. Stelle in der 2. Zeile
    lcd.print("eingeschaltet"); //Schreiben des Texts "eingeschaltet"
    delay(5000);  //Anzeige des Alarmanlagenstatus für 5 Sekunden
    alarmanlageStatus=1;  //Setzen des Alarmanlagenstatus auf 1=eingeschaltet
    return alarmanlageStatus; //Rückgabe des Alarmanlagenstatus
  }
  
  if(tasterAusStatus==HIGH)  //Wenn Taster Aus gedrückt, dann...
  {
    lcd.setCursor(2,0); //Setzen des Cursors auf die 2. Stelle in der 1. Zeile
    lcd.print("Alarmanlage"); //Schreiben des Texts "Alarmanlage"
    lcd.setCursor(1,1); //Setzen des Cursors auf die 1. Stelle in der 2. Zeile
    lcd.print("ausgeschaltet"); //Schreiben des Texts "ausgeschaltet"
    delay(5000);  //Anzeige des Alarmanlagenstatus für 5 Sekunden
    alarmanlageStatus=0;  //Setzen des Alarmanlagenstatus auf 0=ausgeschaltet
    return alarmanlageStatus; //Rückgabe des Alarmanlagenstatus
  }

  lcd.clear();  //Löschen des LCD-Display Inhalts
}

void setup()
{
  Serial.begin(9600); //Starten der seriellen Kommunikation mit 9600 bauds per second
  while(!Serial)  //Solange keine serielle Kommunikation aufgebaut wurde...
  { //nichts tun
  }
  SPI.begin();  //Starten der SPI Kommunikation
  mfrc522.PCD_Init(); //Initalisierung des RFID-Karten Lesers

  lcd.init(); //Initalisierung des LCD-Displays                    
  lcd.backlight();  //Einschalten der Hintergrundbeleuchtung des LCD-Displays

  pinMode(tasterEin, INPUT); //Definition des Arduino Pins für den Taster Ein als Eingang
  pinMode(tasterAus, INPUT); //Definition des Arduino Pins für den Taster Aus als Eingang

  user1.userAnlegen("Nutzer1","master","1234","133120249101");  //Anlegen des 1. Nutzers mit Benutzername, Zugriffsrechten, Pin-Code und RFID-Karten ID
  user2.userAnlegen("Nutzer2","slave","0000","1661716019"); //Anlegen des 2. Nutzers mit Benutzername, Zugriffsrechten, Pin-Code und RFID-Karten ID

  user1.userAusgeben(); //Ausgabe des 1. Nutzerprofils in seriellen Monitor
  user2.userAusgeben(); //Ausgabe des 2. Nutzerprofils in seriellen Monitor
}

void loop()
{
  alarmanlageTaster(tasterEinStatus, tasterAusStatus);  //dauerhaftes Aktualisieren der Tasterstatus
  Serial.println(alarmanlageStatus);
  
  if(!mfrc522.PICC_IsNewCardPresent())  //Wenn keine neue RFID-Karte vom Lesegerät erkannt wird, dann...
  {
    return; //spring zurück an den Anfang der loop()
  }

  if(!mfrc522.PICC_ReadCardSerial())  //Wenn keine RFID-Karte ausgelesen wird, dann...
  {
    return; //spring zurück an den Anfang der loop()
  }

  for(byte i=0;i<mfrc522.uid.size;i++)  //für Ganzzahl i kleiner der Länge der RFID-Karten ID, speichere den i-ten Block der RFID-Karten ID in Element i von String Array, dann zähle i um 1 hoch
  {
    rfidReadStr[i]=mfrc522.uid.uidByte[i];
  }
  
  rfidFinalStr=rfidReadStr[0]+rfidReadStr[1]+rfidReadStr[2]+rfidReadStr[3]; //Füge Elemente des String Arrays zu einem String zusammen
  Serial.println(rfidFinalStr);

  user1.userZugang(); //Ausgabe des erkannten Benutzers im seriellen Monitor
  user2.userZugang(); //Ausgabe des erkannten Benutzers im seriellen Monito  
}
