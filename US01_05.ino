/** Display Beschaltung über Tastendruck
 *  Display
 *  SDA  ->  SDA
 *  SCL  ->  SCL
 * 
 *  Taster 1 (Ein) -> Pin 10
 *  Taster 2 (Aus) -> Pin 11
 * 
 *  beide mit 1000 Ohm Pulldown
 */
#include <Wire.h> //Wire Bibliothek für Display einbinden
#include <LiquidCrystal_I2C.h>  //LiquidCrystal I2C Bibliothek für I2C-Kommunikation mit Display einbinden

LiquidCrystal_I2C lcd(0x27,20,4); //Objekt für Display Initalisierung mit I2C Adresse, Zeichen pro Zeile und Zeilen

int tasterEin=10;  //Arduino Pin für Alarmanlage EIN
int tasterAus=11;  //Arduino Pin für Alarmanlage AUS
int tasterEinStatus=0;  //Tasterstatus EIN Taster (LOW=nicht gedrückt, HIGH=gedrückt)
int tasterAusStatus=0;  //Tasterstatus AUS Taster (LOW=nicht gedrückt, HIGH=gedrückt)
int alarmanlageStatus=0;  //Status der Alarmanlage (0=ausgeschaltet, 1=eingeschaltet)

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
  lcd.init(); //Initalisierung des LCD-Displays                      
  lcd.backlight();  //Einschalten der Hintergrundbeleuchtung des LCD-Displays
  
  pinMode(tasterEin, INPUT); //Definition des Arduino Pins für den Taster Ein als Eingang
  pinMode(tasterAus, INPUT); //Definition des Arduino Pins für den Taster Aus als Eingang

  Serial.begin(9600); //Starten der seriellen Kommunikation mit 9600 bauds per second
}

void loop()
{
  alarmanlageTaster(tasterEinStatus, tasterAusStatus);  //dauerhaftes Aktualisieren der Tasterstatus
  Serial.println(alarmanlageStatus);
}
