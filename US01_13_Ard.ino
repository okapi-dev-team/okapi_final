#include <SPI.h>  //SPI Bibliothek für RFID-Leser einbinden
#include <MFRC522.h>  //MFRC522 Bibliothek für RFID-Leser einbinden
#include <Wire.h> //Wire Bibliothek für Display einbinden
#include <LiquidCrystal_I2C.h>  //LiquidCrystal I2C Bibliothek für I2C-Kommunikation mit Display einbinden

const int SDA_PIN=53; //RFID SDA Pin
const int RST_PIN=5;  //RFID RST Pin
const int TASTER_EIN=10;  //Arduino Pin für Alarmanlage EIN
const int TASTER_AUS=11;  //Arduino Pin für Alarmanlage AUS

int tasterEinStatus=0;  //Tasterstatus EIN Taster (LOW=nicht gedrückt, HIGH=gedrückt)
int tasterAusStatus=0;  //Tasterstatus AUS Taster (LOW=nicht gedrückt, HIGH=gedrückt)
int alarmanlageStatus=0;  //Status der Alarmanlage (0=ausgeschaltet, 1=eingeschaltet)

int rfidRead[4]={0};  //int Array für 4 Blöcke der RFID-Karten ID
String rfidFinalStr="";  //String für komplette RFID-Karten ID

int datenSenden[10]={0};  //int Array für zu sendende Daten Arduino->Processing
 
MFRC522 mfrc522(SDA_PIN, RST_PIN);  //Objekt für RFID-Leser Initalisierung mit SDA Pin und RST Pin

LiquidCrystal_I2C lcd(0x27,20,2); //Objekt für Display Initalisierung mit I2C Adresse, Zeichen pro Zeile und Zeilen

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
  
  if((tasterAusStatus==HIGH))  //Wenn Taster Aus gedrückt, dann...
  {
    alarmanlageStatus=0;  //Setzen des Alarmanlagenstatus auf 0=ausgeschaltet
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
  SPI.begin();  //Starten der SPI Kommunikation
  mfrc522.PCD_Init(); //Initalisierung des RFID-Karten Lesers

  lcd.init(); //Initalisierung des LCD-Displays                    
  lcd.backlight();  //Einschalten der Hintergrundbeleuchtung des LCD-Displays

  pinMode(TASTER_EIN, INPUT); //Definition des Arduino Pins für den Taster Ein als Eingang
  pinMode(TASTER_AUS, INPUT); //Definition des Arduino Pins für den Taster Aus als Eingang

  user1.userAnlegen("Nutzer1","master","1234","133120249101");  //Anlegen des 1. Nutzers mit Benutzername, Zugriffsrechten, Pin-Code und RFID-Karten ID
  user2.userAnlegen("Nutzer2","slave","0000","1661716019"); //Anlegen des 2. Nutzers mit Benutzername, Zugriffsrechten, Pin-Code und RFID-Karten ID

  //user1.userAusgeben(); //Ausgabe des 1. Nutzerprofils in seriellen Monitor
  //user2.userAusgeben(); //Ausgabe des 2. Nutzerprofils in seriellen Monitor

  seriellerKontakt(); //Durchführung serieller Handshake
}

void loop()
{  
  alarmanlageTaster();  //dauerhaftes Aktualisieren der Tasterstatus und ggf. Ausgabe auf Display

  datenSenden[1]=0;  //Setzen von Alarmereigniss (0=kein Alarm, 1=Alarm) HIER NUR PLATZHALTER
  
  rfidScan(); //Suchen und ggf. Scannen von RFID-Karte
  
  user1.userZugang(); //Zugangskontrolle
  user2.userZugang(); //Zugangskontrolle

  datenSenden[6]=7;  //Setzen von Sensorwert 1 HIER NUR PLATZHALTER
  datenSenden[7]=8;  //Setzen von Sensorwert 2 HIER NUR PLATZHALTER
  datenSenden[8]=9;  //Setzen von Sensorwert 3 HIER NUR PLATZHALTER
  datenSenden[9]=10;  //Setzen von Sensorwert 4 HIER NUR PLATZHALTER

  if(Serial.available()>0)  //Wenn Daten am seriellen Port vorliegen (Handshake hat stattgefunden und Processing hat 'A' zurückgesendet), dann...
  {
    for(int i=0;i<10;i++) //schreibe Einträge des datenSenden Arrays in seriellen Port
    {
    Serial.write(datenSenden[i]);
    }
  }
  
  
  delay(1000);  //warte 1 Sekunde (Begrenzung des übermittelten Datenstroms)
  
  for(int i=0;i<10;i++) //setze alle Elemente des datenSenden Arrays =0 (Daten wurden übermittelt, bereit für neue Daten)
  {
    datenSenden[i]=0;
  }

  for(int i=0;i<4;i++)  //setze gescannte RFID ID =0 (bereit für neue RFID-Karte)
  {
    rfidRead[i]=0;
  }
}
