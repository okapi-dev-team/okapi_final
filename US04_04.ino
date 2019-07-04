#include <DS3231.h> //DS3231 Bibliothek für RTC DS3231 einbinden

DS3231  rtc(SDA, SCL);  //Objekt für RTC Initalisierung mit Bezeichnung "rtc", SDA Pin und SCL Pin

Time  Zeit; //Objekt für Initialiserung einer Zeitstruktur

void setup()
{
  Serial.begin(9600); //Starten der seriellen Kommunikation mit 9600 bauds per second

  rtc.begin();  //Starten der Kommunikation mit der Echtzeituhr
}

void loop()
{
  Zeit = rtc.getTime(); //Auslesen der Zeit von der RTC und Speicherung unter "Zeit"

  Serial.print(Zeit.hour);  //Ausgabe der Zeit von der RTC am seriellen Monitor
  Serial.print(":");
  Serial.print(Zeit.min);
  Serial.print(":");
  Serial.println(Zeit.sec);

  delay(1000);  //Jede Sekunde soll die Uhrzeit ausgelesen werden
}
