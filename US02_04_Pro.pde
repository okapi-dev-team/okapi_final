import processing.serial.*;  //serial Bibliothek für serielle Kommunikation einbinden

Serial Port;  //Erstellen eines Objekts Port in Klasse Serial
int[] datenEmpfangen=new int[10];  //Definition eines int-Arrays datenEmpfangen
int datenCount=0;  //Initalisierung der int-Variablen datenCount (Zählvariable)

int alarmanlageStatus,alarm,benutzer1,benutzer2,rechteBenutzer1,rechteBenutzer2,sensor1,sensor2,sensor3,sensor4;  //Definition der int-Variablen für empfangene Daten

boolean seriellerKontakt=false;  //Initalisierung der bool-Variable seriellerKontakt

Table tabelleAnmeldung;  //Erstellen eines Objekts tabelleAnmeldung in Klasse Table
Table tabelleAlarm;  //Erstelle eines Objekts tabelleAlarm in Klasse Table

void setup()
{  
  Port=new Serial(this,Serial.list()[6],9600);  //Definition des Objekts Port (Eintrag in Liste verfügbarer serieller Ports, baud rate=9600 bauds per second)
  
  tabelleAnmeldung=new Table();  //erstelle Tabelle tabelleAnmeldung
  tabelleAnmeldung.addColumn("Datum/Uhrzeit");  //erstelle Tabellenspalte Datum/Uhrzeit
  tabelleAnmeldung.addColumn("Ereignis");  //erstelle Tabellenspalte Ereignis
  tabelleAnmeldung.addColumn("Benutzer");  //erstelle Tabellenspalte Benutzer
  
  tabelleAlarm=new Table();  //erstelle Tabelle tabelleAlarm
  tabelleAlarm.addColumn("Datum/Uhrzeit");  //erstelle Tabellenspalte Datum/Uhrzeit
  tabelleAlarm.addColumn("Ereignis");  //erstelle Tabellenspalte Ereignis
}

void draw()
{
}

/** \brief Liest Daten aus seriellem Port aus und schreibt Daten in seriellen Port (serieller Handshake). Wird nur aufgerufen, wenn serielle Daten vorliegen.
*   \param Port Objekt aus Klasse Serial (ist der verwendete serielle Port)
*/
void serialEvent(Serial Port)
{
  if(seriellerKontakt==false)  //Wenn boolean-Variable seriellerKontakt =false ist, dann...
  {
    if(Port.read()=='A')  //Wenn erster Eintrag in seriellem Port ='A' ist, dann...
    {
      Port.clear();  //lösche Daten aus seriellem Puffer
      seriellerKontakt=true;  //setze boolean-Variable seriellerKontakt auf true (Kontakt zwischen Arduino und Processing wurde hergestellt)
      println("Kontakt hergestellt");  //Information für Benutzer in Processing Konsole
      Port.write('A');  //schreibe 'A' in seriellen Port
    }
  }
  
  else  //Sonst (seriellerKontakt==true) = Kontakt bereits hergestellt
  {
    datenEmpfangen[datenCount]=Port.read();  //speicher Byte aus seriellem Port an der Stelle datenCount in int-Array datenEmpfangen
    datenCount++;  //erhöhe int-Variable datenCount um 1 (hochzählen)

    if(datenCount>9)  //Wenn Zählvariable größer 9 (zehn Einträge in Array), dann...
    {
      alarmanlageStatus=datenEmpfangen[0];  //setze die Variablen für einzelne Funktionen gleich den Einträgen des Arrays
      alarm=datenEmpfangen[1];
      benutzer1=datenEmpfangen[2];
      rechteBenutzer1=datenEmpfangen[3];
      benutzer2=datenEmpfangen[4];
      rechteBenutzer2=datenEmpfangen[5];
      sensor1=datenEmpfangen[6];
      sensor2=datenEmpfangen[7];
      sensor3=datenEmpfangen[8];
      sensor4=datenEmpfangen[9];
      
      for(int i=0;i<10;i++)  //Ausgeben der Daten in Processing Konsole
      {
        print(datenEmpfangen[i]);
      }
      println();
      
      Port.write('A');  //schreibe 'A' in seriellen Port (bereit für weitere Daten)
      datenCount=0;  //setze Zählvariable gleich 0
    }
    
    String sekundeStr=nf(second(),2);  //String sekundeStr=aktuelle Sekundenzahl in 2 stelliger Darstellung
    String minuteStr=nf(minute(),2);  //String minuteStr=aktuelle Minutenzahl in 2 stelliger Darstellung
    String stundeStr=nf(hour(),2);  //String stundeStr=aktuelle Stundenzahl in 2 stelliger Darstellung
    String tagStr=nf(day(),2);  //String tagStr=aktuelle Tageszahl in 2 stelliger Darstellung
    String monatStr=nf(month(),2);  //String monatStr=aktuelle Monatszahl in 2 stelliger Darstellung
    String jahrStr=nf(year());  //String jahrStr=aktuelle Jahreszahl
  
    String[] uhrzeitStr=new String[3];  //erstelle String-Array uhrzeitStr mit 3 Einträgen
    uhrzeitStr[0]=stundeStr;  //Stunden = 1. Eintrag
    uhrzeitStr[1]=minuteStr;  //Minuten = 2. Eintrag
    uhrzeitStr[2]=sekundeStr;  //Sekunden = 3. Eintrag
    String uhrzeitKomplettStr=join(uhrzeitStr,":");  //füge alle Arrayeinträge zu einem String zusammen (Trennzeichen :)
  
    String[] datumStr=new String[3];  //erstelle String-Array datumStr mit 3 Einträgen
    datumStr[0]=tagStr;  //Tag = 1. Eintrag
    datumStr[1]=monatStr;  //Monat = 2. Eintrag
    datumStr[2]=jahrStr;  //Jahr = 3. Eintrag
    String datumKomplettStr=join(datumStr,".");  //füge alle Arrayeinträge zu einem String zusammen (Trennzeichen .)
    
    if(alarmanlageStatus==1&&alarm==1)   //Wenn Alarmanlage eingeschaltet ist und eine Bewegung registriert wird, dann...
    {
      TableRow zeile=tabelleAlarm.addRow();  //erstelle Tabellenzeile
      zeile.setString("Datum/Uhrzeit",datumKomplettStr+" "+uhrzeitKomplettStr);  //setze Eintrag in Spalte Datum/Uhrzeit = aktuelles Datum/Uhrzeit
      zeile.setString("Ereignis","Alarm");  //setze Eintrag in Spalte Ereignis = Alarm
       
      saveTable(tabelleAlarm,"Alarm.csv","csv");  //speichere Tabelle als Alarm.csv
    }
    
    if(benutzer1==1||benutzer2==1)   //Wenn bekannter Nutzer Anmeldung durchgeführt hat, dann...
    {
      TableRow zeile=tabelleAnmeldung.addRow();  //erstelle Tabellenzeile
      zeile.setString("Datum/Uhrzeit",datumKomplettStr+" "+uhrzeitKomplettStr);  //setze Eintrag in Spalte Datum/Uhrzeit = aktuelles Datum/Uhrzeit
      zeile.setString("Ereignis","Anmeldung");  //setze Eintrag in Spalte Ereignis = Anmeldung
      if(benutzer1==1)  //Wenn Benutzer = Nutzer1
      {
        zeile.setString("Benutzer","Nutzer1");  //setze Eintrag in Spalte Benutzer = Nutzer1
      }
      if(benutzer2==1)  //Wenn Benutzer = Nutzer2
      {
        zeile.setString("Benutzer","Nutzer2");  //setze Eintrag in Spalte Benutzer = Nutzer2
      }
    
      saveTable(tabelleAnmeldung,"Anmeldung.csv","csv");  //speichere Tabelle als Anmeldung.csv
    }
  }
}
