import processing.serial.*;  //serial Bibliothek für serielle Kommunikation einbinden

Serial Port;  //Erstellen eines Objekts Port in Klasse Serial
int[] datenEmpfangen=new int[10];  //Definition eines int-Arrays datenEmpfangen
int[] datenSenden=new int[1];  //Definition eines int-Arrays datenSenden
int datenCount=0;  //Initalisierung der int-Variablen datenCount (Zählvariable)

int alarmanlageStatus,alarm,benutzer1,benutzer2,rechteBenutzer1,rechteBenutzer2,sensor1,sensor2,sensor3,sensor4;  //Definition der int-Variablen für empfangene Daten

boolean seriellerKontakt=false;  //Initalisierung der bool-Variable seriellerKontakt
boolean benutzer1Hold=false;  //Initalisierung von bool-Variablen als toggle (true=Benutzer ist angemeldet/hat master Rechte/GUI soll angezeigt werden; false=Benutzer ist nicht angemeldet/hat slave Rechte/GUI soll nicht angezeigt werden)
boolean benutzer2Hold=false;
boolean masterRechteHold=false;
boolean GUI=false;

Table tabelleAnmeldung;  //Erstellen eines Objekts tabelleAnmeldung in Klasse Table
Table tabelleAlarm;  //Erstelle eines Objekts tabelleAlarm in Klasse Table

void setup()
{
  size(1000,800);  //Erstellen Canvas mit Größe 1000x800 Pixel
  background(200);  //Füllen des Hintergrunds mit Greyscale Wert 200
  
  Port=new Serial(this,Serial.list()[7],9600);  //Definition des Objekts Port (Eintrag in Liste verfügbarer serieller Ports, baud rate=9600 bauds per second)
  
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
  background(200);  //Füllen des Hintergrunds mit Greyscale Wert 200
  
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
  
  if(GUI)  //Wenn GUI angezeigt werden soll (Benutzer ist angemeldet)
  {
    stroke(0);  //Strichfarbe Greyscale Wert 0
    strokeWeight(4);  //Strichstärke 4 Pixel
    strokeCap(SQUARE);  //Strichkontur quadratisch
  
    line(500,0,500,400);  //zeichne Linien für Bildschirmaufteilung
    line(0,400,1000,400);
    line(500,50,1000,50);
    line(750,0,750,50);
    line(500,100,1000,100);
  
    textSize(20);  //setze die Schriftgröße auf 20pt
    fill(0);  //setze die Schriftfarbe auf Greyscale Wert 0
    textAlign(LEFT,TOP);  //Textausrichtung
    text(uhrzeitKomplettStr,760,10);  //schreibe die aktuelle Uhrzeit auf das Canvas
    textAlign(RIGHT,TOP);  //Textausrichtung
    text(datumKomplettStr,990,10);  //schreibe das aktuelle Datum auf das Canvas
    textAlign(CENTER,TOP);  //Textausrichtung
    text("Benutzer abmelden",750,64);  //schreibe "Benutzer abmelden" auf das Canvas
  
    textSize(20);  //setze die Schriftgröße auf 20pt
    fill(0);  //setze die Schriftfarbe auf Greyscale Wert 0
    textAlign(CENTER,TOP);  //Textausrichtung
    if(benutzer1Hold)  //Wenn Nutzer1 angemeldet ist, dann...
    {
      text("Nutzer1",625,10);  //schreibe Benutzername Nutzer1 auf das Canvas
    }
    if(benutzer2Hold)  //Wenn Nutzer2 angemeldet ist, dann...
    {
      text("Nutzer2",625,10);  //schreibe Benutzername Nutzer2 auf das Canvas
    }
  
    stroke(0);  //Strichfarbe Greyscale Wert 0
    strokeWeight(3);  //Strichstärke 3 Pixel
    strokeJoin(ROUND);  //Strichverbindung rund
    noFill();  //keine Füllung der folgenden Formen
  
    beginShape();  //zeichne Form (aufrechtes Dach)
    vertex(755,240);
    vertex(785,210);
    vertex(815,240);
    endShape();
    rect(750,205,70,40);  //zeichne Rechteck
    
    beginShape();  //zeichne Form (umgedrehtes Dach)
    vertex(755,260);
    vertex(785,290);
    vertex(815,260);
    endShape();
    rect(750,255,70,40);  //zeichne Rechteck
    
    textSize(20);  //setze die Schriftgröße auf 20pt
    fill(0);  //setze die Schriftfarbe auf Greyscale Wert 0
    textAlign(CENTER,CENTER);  //Textausrichtung
    text("Fenster auf/zu",625,250);  //schreibe "Fenster auf/zu" auf das Canvas
  
    textSize(40);  //setze die Schriftgröße auf 40pt
    fill(0);  //setze die Schriftfarbe auf Greyscale Wert 0
    textAlign(LEFT,TOP);  //Textausrichtung
    text("Sensor 1: "+sensor1,50,25);  //schreibe die Sensorbezeichnungen und Sensorwerte auf das Canvas
    text("Sensor 2: "+sensor2,50,125);
    text("Sensor 3: "+sensor3,50,225);
    text("Sensor 4: "+sensor4,50,325);
    
    if(masterRechteHold)  //Wenn benutzer=master, dann... (Anzeige der Protokollierung nur für Benutzer mit master Rechten)
    {
      int zeilenAnmeldung=tabelleAnmeldung.getRowCount();  //setze int-Variable zeilenAnmeldung gleich der Zeilenanzahl der Tabelle
      if(zeilenAnmeldung>0&&zeilenAnmeldung<19)  //Wenn Zeilenanzahl größer 0 (Einträge in Tabelle) und kleiner 19, dann...
      {
        for(int i=0;i<zeilenAnmeldung;i++)  //gehe jede Zeile von Beginn an durch
        {
          textSize(20);  //setze die Schriftgröße auf 20pt
          fill(0);  //setze die Schriftfarbe auf Greyscale Wert 0
          textAlign(LEFT,TOP);  //Textausrichtung
          text(tabelleAnmeldung.getString(i,"Datum/Uhrzeit"),10,20*i+410);  //schreibe den Eintrag der Spalte Datum/Uhrzeit auf das Canvas
          text(tabelleAnmeldung.getString(i,"Ereignis"),250,20*i+410);  //schreibe den Eintrag der Spalte Ereignis auf das Canvas
          text(tabelleAnmeldung.getString(i,"Benutzer"),400,20*i+410);  //schreibe den Eintrag der Spalte Benutzer auf das Canvas     
        }
      }
      if(zeilenAnmeldung>19)  //Wenn Zeilenanzahl größer 19, dann...
      {
        for(int i=0;i<19;i++)  //gehe die ersten 19 Zeilen von Beginn an durch
        {
          textSize(20);  //setze die Schriftgröße auf 20pt
          fill(0);  //setze die Schriftfarbe auf Greyscale Wert 0
          textAlign(LEFT,TOP);  //Textausrichtung
          text(tabelleAnmeldung.getString(i,"Datum/Uhrzeit"),10,20*i+410);  //schreibe den Eintrag der Spalte Datum/Uhrzeit auf das Canvas
          text(tabelleAnmeldung.getString(i,"Ereignis"),250,20*i+410);  //schreibe den Eintrag der Spalte Ereignis auf das Canvas
          text(tabelleAnmeldung.getString(i,"Benutzer"),400,20*i+410);  //schreibe den Eintrag der Spalte Benutzer auf das Canvas     
        }
      }
      
      int zeilenAlarm=tabelleAlarm.getRowCount();  //setze int-Variable zeilenAlarm gleich der Zeilenanzahl der Tabelle
      if(zeilenAlarm>0&&zeilenAlarm<19)  //Wenn Zeilenanzahl größer 0 (Einträge in Tabelle) und kleiner 19, dann...
      {
        for(int i=0;i<zeilenAlarm;i++)  //gehe jede Zeile von Beginn an durch
        {
          textSize(20);  //setze die Schriftgröße auf 20pt
          fill(0);  //setze die Schriftfarbe auf Greyscale Wert 0
          textAlign(LEFT,TOP);  //Textausrichtung
          text(tabelleAlarm.getString(i,"Datum/Uhrzeit"),510,20*i+410);  //schreibe den Eintrag der Spalte Datum/Uhrzeit auf das Canvas
          text(tabelleAlarm.getString(i,"Ereignis"),750,20*i+410);  //schreibe den Eintrag der Spalte Ereignis auf das Canvas
        }
      }
      if(zeilenAlarm>19)  //Wenn Zeilenanzahl größer 19, dann...
      {
        for(int i=0;i<19;i++)  //gehe die ersten 19 Zeilen von Beginn an durch
        {
          textSize(20);  //setze die Schriftgröße auf 20pt
          fill(0);  //setze die Schriftfarbe auf Greyscale Wert 0
          textAlign(LEFT,TOP);  //Textausrichtung
          text(tabelleAlarm.getString(i,"Datum/Uhrzeit"),510,20*i+410);  //schreibe den Eintrag der Spalte Datum/Uhrzeit auf das Canvas
          text(tabelleAlarm.getString(i,"Ereignis"),750,20*i+410);  //schreibe den Eintrag der Spalte Ereignis auf das Canvas
        }
      }
    }
    
    if(mousePressed==true&&mouseButton==LEFT)  //Wenn linke Maustaste gedrückt, dann...
    {
      if(mouseX>=500&&mouseX<=1000&&mouseY>=52&&mouseY<=98)  //Wenn Mousecursor über Schaltfläche für Benutzer abmelden, dann...
      {
        benutzer1Hold=false;  //setze alle toggle auf false
        benutzer2Hold=false;
        masterRechteHold=false;
        GUI=false;
      }
    }
  }
  
  else  //keine Anzeige der GUI -> Screensaver
  {
    textSize(40);  //setze die Schriftgröße auf 20pt
    fill(0);  //setze die Schriftfarbe auf Greyscale Wert 0
    textAlign(CENTER,CENTER);  //Textausrichtung
    text(uhrzeitKomplettStr,400,400);  //schreibe die aktuelle Uhrzeit auf das Canvas
    textAlign(CENTER,CENTER);  //Textausrichtung
    text(datumKomplettStr,630,400);  //schreibe das aktuelle Datum auf das Canvas
  }

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
    if(GUI&&mousePressed==true&&mouseButton==LEFT)  //Wenn GUI angezeigt wird und linke Maustaste gedrückt, dann...
    {
      if(mouseX>=750&&mouseX<=820&&mouseY>=205&&mouseY<=245)  //Wenn Mauscursor über Schaltfläche für Fenster auf, dann...
      {
        datenSenden[0]=1;
      }
      if(mouseX>=750&&mouseX<=820&&mouseY>=255&&mouseY<=295)  //Wenn Mauscursor über Schaltfläche für Fenster zu, dann...
      {
        datenSenden[0]=2;
      }
      
      Port.write(datenSenden[0]);
      println(datenSenden[0]);
    }
      
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
      tabelleAlarm.sortReverse("Datum/Uhrzeit");  //sortiere die Tabelle nach absteigendem Datum/Uhrzeit
    }
    
    if(benutzer1==1||benutzer2==1)   //Wenn bekannter Nutzer Anmeldung durchgeführt hat, dann...
    {
      GUI=true;  //Benutzer ist angemeldet -> GUI aktivieren
      TableRow zeile=tabelleAnmeldung.addRow();  //erstelle Tabellenzeile
      zeile.setString("Datum/Uhrzeit",datumKomplettStr+" "+uhrzeitKomplettStr);  //setze Eintrag in Spalte Datum/Uhrzeit = aktuelles Datum/Uhrzeit
      zeile.setString("Ereignis","Anmeldung");  //setze Eintrag in Spalte Ereignis = Anmeldung
      if(benutzer1==1)  //Wenn Benutzer = Nutzer1
      {
        zeile.setString("Benutzer","Nutzer1");  //setze Eintrag in Spalte Benutzer = Nutzer1
        benutzer2Hold=false;  //Wenn Nutzer1 angemeldet ist, kann nicht Nutzer2 angemeldet sein
        benutzer1Hold=true;  //Nutzer1 ist angemeldet
        masterRechteHold=false;  //Nutzerrechte erst auf slave setzen, da später geprüft wird
      }
      if(benutzer2==1)  //Wenn Benutzer = Nutzer2
      {
        zeile.setString("Benutzer","Nutzer2");  //setze Eintrag in Spalte Benutzer = Nutzer2
        benutzer1Hold=false;  //Wenn Nutzer2 angemeldet ist, kann nicht Nutzer1 angemeldet sein
        benutzer2Hold=true;  //Nutzer2 ist angemeldet
        masterRechteHold=false;  //Nutzerrechte erst auf slave setzen, da später geprüft wird
      }
    
      saveTable(tabelleAnmeldung,"Anmeldung.csv","csv");  //speichere Tabelle als Anmeldung.csv
      tabelleAnmeldung.sortReverse("Datum/Uhrzeit");  //sortiere die Tabelle nach absteigendem Datum/Uhrzeit
    }
    
    if(rechteBenutzer1==1||rechteBenutzer2==1)  //Wenn benutzer=master, dann... (Anzeige der Protokollierung nur für Benutzer mit master Rechten)
    {
      masterRechteHold=true;  //setze Nutzerrechte auf master
    }
  }
}
