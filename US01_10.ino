/** Basisbeschaltung I2C Dsiplay mit RTC
 *  Display und RTC beide an SCA und SCL angeschlossen
 *  I2C Adresse über I2CScanner 
 *  
 *  Uhrzeit über SetTime Sketch aus der DS3231 Bibliothek
 */
#include <Wire.h> //Wire Bibliothek für Display einbinden
#include <LiquidCrystal_I2C.h>  //LiquidCrystal I2C Bibliothek für I2C-Kommunikation mit Display einbinden
#include <DS3231.h> //DS3231 Bibliothek für RTC DS3231 einbinden

DS3231  rtc(SDA, SCL);  //Objekt für RTC Initalisierung mit SDA Pin und SCL Pin

LiquidCrystal_I2C lcd(0x27,20,4); //Objekt für Display Initalisierung mit I2C Adresse, Zeichen pro Zeile und Zeilen

String wochentagStr;  //String für Wochentag
String wochentagKurzStr;  //String für Wochentagabkürzung

/** \brief Auslesen der aktuellen Uhrzeit und Datum aus RTC und Ausgabe auf Display
 *  
 */
void uhrzeitDatumDisplay()
{
  wochentagStr=rtc.getDOWStr(); //Auslesen des Wochentags aus RTC, Speichern in String
  
  if(wochentagStr.equals("Monday")) //Umwandlung des Wochentags in Abkürzung (Mo, Di, Mi, Do, Fr, Sa, So)
  {
    wochentagKurzStr="Mo";
  }
  if(wochentagStr.equals("Tuesday"))
  {
    wochentagKurzStr="Di";
  }
  if(wochentagStr.equals("Wednesday"))
  {
    wochentagKurzStr="Mi";
  }
  if(wochentagStr.equals("Thursday"))
  {
    wochentagKurzStr="Do";
  }
  if(wochentagStr.equals("Friday"))
  {
    wochentagKurzStr="Fr";
  }
  if(wochentagStr.equals("Saturday"))
  {
    wochentagKurzStr="Sa";
  }
  if(wochentagStr.equals("Sunday"))
  {
    wochentagKurzStr="So";
  }

  lcd.setCursor(4,0); //Setzen des Cursors auf die 4. Stelle in der 1. Zeile
  lcd.print(rtc.getTimeStr());  //Schreiben der aktuellen Uhrzeit (ausgelesen von RTC)
  lcd.setCursor(1,1); //Setzen des Cursors auf die 1. Stelle in der 2. Zeile
  lcd.print(wochentagKurzStr);  //Schreiben der aktuellen Wochentagsabkürzung
  lcd.setCursor(5,1); //Setzen des Cursors auf die 5. Stelle in der 2. Zeile
  lcd.print(rtc.getDateStr());  //Schreiben des aktuellen Datums (ausgelesen von RTC)
 
  delay(1000);
}

void setup()
{
  lcd.init(); //Initalisierung des LCD-Displays                    
  lcd.backlight();  //Einschalten der Hintergrundbeleuchtung des LCD-Displays
  
  rtc.begin();  //Starten der Kommunikation mit der Echtzeituhr
}

void loop()
{ 
  uhrzeitDatumDisplay(); //Auslesen der aktuellen Uhrzeit und Datum aus RTC und Ausgabe auf Display
}
