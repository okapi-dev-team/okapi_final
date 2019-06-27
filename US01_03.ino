#include <SPI.h>  //SPI Bibliothek für RFID-Leser einbinden
#include <MFRC522.h>  //MFRC522 Bibliothek für RFID-Leser einbinden

const int SDA_PIN=53; //RFID SDA Pin
const int RST_PIN=5;  //RFID RST Pin

MFRC522 mfrc522(SDA_PIN, RST_PIN);  //Objekt für RFID-Leser Initalisierung mit SDA Pin und RST Pin

String rfidReadStr[4];  //String Array für 4 Hexadeximal Blöcke der RFID-Karten ID
String rfidFinalStr;  //String für komplette RFID-Karten ID

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

void setup()
{
  Serial.begin(9600); //Starten der seriellen Kommunikation mit 9600 bauds per second
  SPI.begin();  //Starten der SPI Kommunikation
  mfrc522.PCD_Init(); //Initalisierung des RFID-Karten Lesers

  user1.userAnlegen("Nutzer1","master","1234","133120249101");  //Anlegen des 1. Nutzers mit Benutzername, Zugriffsrechten, Pin-Code und RFID-Karten ID
  user2.userAnlegen("Nutzer2","slave","0000","1661716019"); //Anlegen des 2. Nutzers mit Benutzername, Zugriffsrechten, Pin-Code und RFID-Karten ID

  user1.userAusgeben(); //Ausgabe des 1. Nutzerprofils in seriellen Monitor
  user2.userAusgeben(); //Ausgabe des 2. Nutzerprofils in seriellen Monitor
}

void loop()
{
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

  delay(1000);

  user1.userZugang(); //Ausgabe des erkannten Benutzers im seriellen Monitor
  user2.userZugang(); //Ausgabe des erkannten Benutzers im seriellen Monitor
  
}
