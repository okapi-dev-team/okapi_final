/** RFID auslesen Basisschaltung
 *  SDA  ->  Pin 53
 *  SCK  ->  Pin 52
 *  MOSI ->  Pin 51
 *  MISO ->  Pin 50
 *  IRQ  ->  -
 *  GND  ->  GND
 *  RST  ->  Pin 5
 *  3,3V  ->  3,3V
 */
#include <SPI.h>  //SPI Bibliothek für RFID-Leser einbinden
#include <MFRC522.h>  //MFRC522 Bibliothek für RFID-Leser einbinden

const int SDA_PIN=53; //RFID SDA Pin
const int RST_PIN=5;  //RFID RST Pin

String rfidReadStr[4];  //String Array für 4 Hexadeximal Blöcke der RFID-Karten ID
String rfidFinalStr;  //String für komplette RFID-Karten ID

MFRC522 mfrc522(SDA_PIN, RST_PIN);  //Objekt für RFID-Leser Initalisierung mit SDA Pin und RST Pin

void setup()
{
  Serial.begin(9600); //Starten der seriellen Kommunikation mit 9600 bauds per second
  SPI.begin();  //Starten der SPI Kommunikation
  mfrc522.PCD_Init(); //Initalisierung des RFID-Karten Lesers
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
}
