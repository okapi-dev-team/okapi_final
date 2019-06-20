import processing.serial.*;  //serial Bibliothek für serielle Kommunikation einbinden

Serial seriellerPort;  //Erstellen eines Objekts seriellerPort
int[] seriellerEingang=new int[7];  //Definition eines int-Arrays seriellerEingang
int empfangeneSerielleBytes=0;  //Initalisierung der int-Variablen empfangeneSerielleBytes (Zählvariable)
int taster1,taster2;  //Definition int-Variablen taster1 und taster2 (0=nicht gedrückt, 1=gedrückt)
int sensor1,sensor2,sensor3,sensor4;  //Definition int-Variablen für ausgelesene Sensorwerte
int benutzer;  //Definition int-Variable benutzer (0=slave, 1=master)

boolean seriellerKontakt=false;  //Initalisierung der bool-Variable seriellerKontakt

Table tabelleAnmeldung;  //erstelle ein Objekt tabelleAnmeldung in Klasse Table
Table tabelleAlarm;  //erstelle ein Objekt tabelleAlarm in Klasse Table


void setup()
{
  size(1000,800);  //Erstellen Canvas mit Größe 1000x800 Pixel
  background(200);  //Füllen des Hintergrunds mit Greyscale Wert 200
  
  seriellerPort=new Serial(this,Serial.list()[7],9600);  //Definition des Objekts seriellerPort (baud rate=9600 bauds per second)
  
  tabelleAnmeldung=new Table();  //erstelle Tabelle tabelleAnmeldung
  tabelleAlarm=new Table();  //erstelle Tabelle tabelleAlarm
  
  tabelleAnmeldung.addColumn("Datum/Uhrzeit");  //erstelle Tabellenspalte Datum/Uhrzeit
  tabelleAnmeldung.addColumn("Ereignis");  //erstelle Tabellenspalte Ereignis
  tabelleAnmeldung.addColumn("Benutzer");  //erstelle Tabellenspalte Benutzer
  
  tabelleAlarm.addColumn("Datum/Uhrzeit");  //erstelle Tabellenspalte Datum/Uhrzeit
  tabelleAlarm.addColumn("Ereignis");  //erstelle Tabellenspalte Ereignis
  tabelleAlarm.addColumn("Benutzer");  //erstelle Tabellenspalte Benutzer
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
  
  if(taster1==1)   //Wenn taster1 gedrückt, dann...
  {
    TableRow zeile=tabelleAnmeldung.addRow();  //erstelle Tabellenzeile
    zeile.setString("Datum/Uhrzeit",datumKomplettStr+" "+uhrzeitKomplettStr);  //setze Eintrag in Spalte Datum/Uhrzeit = aktuelles Datum/Uhrzeit
    zeile.setString("Ereignis","Anmeldung");  //setze Eintrag in Spalte Ereignis = Anmeldung
    if(benutzer==0)  //Wenn benutzer=slave, dann...
    {
      zeile.setString("Benutzer","slave");  //setze Eintrag in Spalte Benutzer = slave
    }
    if(benutzer==1)  //Wenn benutzer=master, dann...
    {
      zeile.setString("Benutzer","master");  //setze Eintrag in Spalte Benutzer = master
    }
    
    saveTable(tabelleAnmeldung,"Anmeldung.csv","csv");  //speichere Tabelle als Anmeldung.csv
  }
  if(taster2==1)  //Wenn taster2 gedrückt, dann...
  {
    TableRow zeile=tabelleAlarm.addRow();  //erstelle Tabellenzeile
    zeile.setString("Datum/Uhrzeit",datumKomplettStr+" "+uhrzeitKomplettStr);  //setze Eintrag in Spalte Datum/Uhrzeit = aktuelles Datum/Uhrzeit
    zeile.setString("Ereignis","Alarm");  //setze Eintrag in Spalte Ereignis = Alarm
    if(benutzer==0)  //Wenn benutzer=slave, dann...
    {
      zeile.setString("Benutzer","slave");  //setze Eintrag in Spalte Benutzer = slave
    }
    if(benutzer==1)  //Wenn benutzer=master, dann...
    {
      zeile.setString("Benutzer","master");  //setze Eintrag in Spalte Benutzer = master
    }
  
    saveTable(tabelleAlarm,"Alarm.csv","csv");  //speichere Tabelle als Alarm.csv
  }
  
  stroke(0);  //Strichfarbe Greyscale Wert 0
  strokeWeight(4);  //Strichstärke 4 Pixel
  strokeCap(SQUARE);  //Strichkontur quadratisch
  
  line(500,0,500,400);  //zeichne Linien für Bildschirmaufteilung
  line(0,400,1000,400);
  line(500,50,1000,50);
  line(750,0,750,50);
  
  textSize(20);  //setze die Schriftgröße auf 20pt
  fill(0);  //setze die Schriftfarbe auf Greyscale Wert 0
  textAlign(LEFT,TOP);  //Textausrichtung
  text(uhrzeitKomplettStr,760,10);  //schreibe die aktuelle Uhrzeit auf das Canvas
  textAlign(RIGHT,TOP);  //Textausrichtung
  text(datumKomplettStr,990,10);  //schreibe das aktuelle Datum auf das Canvas
  
  textSize(20);  //setze die Schriftgröße auf 20pt
  fill(0);  //setze die Schriftfarbe auf Greyscale Wert 0
  textAlign(LEFT,TOP);  //Textausrichtung
  if(benutzer==0)  //Wenn benutzer=slave, dann...
  {
    text("slave",600,10);  //schreibe Benutzername slave auf das Canvas
  }
  if(benutzer==1)  //Wenn benutzer=master, dann...
  {
    text("master",600,10);  //schreibe Benutzername master auf das Canvas
  }
  
  stroke(0);  //Strichfarbe Greyscale Wert 0
  strokeWeight(3);  //Strichstärke 3 Pixel
  strokeJoin(ROUND);  //Strichverbindung rund
  noFill();  //keine Füllung der folgenden Formen
  
  beginShape();  //zeichne Form (aufrechtes Dach)
  vertex(755,215);
  vertex(785,185);
  vertex(815,215);
  endShape();
  rect(750,180,70,40);  //zeichne Rechteck
  
  beginShape();  //zeichne Form (umgedrehtes Dach)
  vertex(755,235);
  vertex(785,265);
  vertex(815,235);
  endShape();
  rect(750,230,70,40);  //zeichne Rechteck
  
  textSize(20);  //setze die Schriftgröße auf 20pt
  fill(0);  //setze die Schriftfarbe auf Greyscale Wert 0
  textAlign(LEFT,CENTER);  //Textausrichtung
  text("Fenster auf/zu",550,225);  //schreibe "Fenster auf/zu" auf das Canvas
  
  textSize(40);  //setze die Schriftgröße auf 40pt
  fill(0);  //setze die Schriftfarbe auf Greyscale Wert 0
  textAlign(LEFT,TOP);  //Textausrichtung
  text("Sensor 1: "+sensor1,50,25);  //schreibe die Sensorbezeichnungen und Sensorwerte auf das Canvas
  text("Sensor 2: "+sensor2,50,125);
  text("Sensor 3: "+sensor3,50,225);
  text("Sensor 4: "+sensor4,50,325);
  
  
  int zeilenAnmeldung=tabelleAnmeldung.getRowCount();  //setze int-Variable zeilenAnmeldung gleich der Zeilenanzahl der Tabelle
  if(zeilenAnmeldung>0)  //Wenn Zeilenanzahl größer 0 (Einträge in Tabelle), dann...
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
  int zeilenAlarm=tabelleAlarm.getRowCount();  //setze int-Variable zeilenAlarm gleich der Zeilenanzahl der Tabelle
  if(zeilenAlarm>0)  //Wenn Zeilenanzahl größer 0 (Einträge in Tabelle), dann...
  {
    for(int i=0;i<zeilenAlarm;i++)  //gehe jede Zeile von Beginn an durch
    {
      textSize(20);  //setze die Schriftgröße auf 20pt
      fill(0);  //setze die Schriftfarbe auf Greyscale Wert 0
      textAlign(LEFT,TOP);  //Textausrichtung
      text(tabelleAlarm.getString(i,"Datum/Uhrzeit"),510,20*i+410);  //schreibe den Eintrag der Spalte Datum/Uhrzeit auf das Canvas
      text(tabelleAlarm.getString(i,"Ereignis"),750,20*i+410);  //schreibe den Eintrag der Spalte Ereignis auf das Canvas
      text(tabelleAlarm.getString(i,"Benutzer"),900,20*i+410);  //schreibe den Eintrag der Spalte Benutzer auf das Canvas     
    }
  }  
}

/** \brief Liest Daten aus seriellem Port aus und schreibt Daten in seriellen Port (serieller Handshake)
           wird nur aufgerufen, wenn serielle Daten vorliegen
*   \param seriellerPort Objekt aus Klasse Serial (ist der verwendete serielle Port)
*/
void serialEvent(Serial seriellerPort)
{
  int eingangsByte=seriellerPort.read();  //lies Daten aus seriellem Port aus und speichere sie in int-Variable eingangsByte
  
  if(seriellerKontakt==false)  //Wenn boolean-Variable seriellerKontakt identisch false ist, dann...
  {
    if(eingangsByte=='A')  //Wenn int-Variable eingangsByte identisch 'A' ist, dann...
    {
      seriellerPort.clear();  //lösche Daten aus seriellem Puffer
      seriellerKontakt=true;  //setze boolean-Variable seriellerKontakt auf true (Kontakt zwischen Arduino und Processing wurde hergestellt)
      println("Kontakt hergestellt");  //Information für Benutzer in Processing Konsole
      seriellerPort.write('A');  //schreibe 'A' in seriellen Port
    }
  }
  
  else  //Sonst (seriellerKontakt==true) = Kontakt bereits hergestellt
  {
    if(mousePressed==true&&mouseButton==LEFT)  //Wenn linke Maustaste gedrückt, dann...
    {
      if(mouseX>=750&&mouseX<=820&&mouseY>=180&&mouseY<=220)  //Wenn Mauscursor über Schaltfläche für Fenster auf, dann...
      {
        seriellerPort.write('2');  //schreibe '2' in seriellen Port
        //println("2");
      }
      if(mouseX>=750&&mouseX<=820&&mouseY>=230&&mouseY<=270)  //Wenn Mauscursor über Schaltfläche für Fenster zu, dann...
      {
        seriellerPort.write('1');  //schreibe '1' in seriellen Port
        //println("1");
      }
    }
      
      

    seriellerEingang[empfangeneSerielleBytes]=eingangsByte;  //speicher eingangsByte an der Stelle empfangeneSerielleBytes in int-Array seriellerEingang
    empfangeneSerielleBytes++;  //erhöhe int-Variable empfangeneSerielleBytes um 1 (hochzählen)

    if(empfangeneSerielleBytes>6)  //Wenn Zählvariable größer 6 (sieben Einträge in Array), dann...
    {
      taster1=seriellerEingang[0];  //setze int-Variable taster1 gleich 1. Eintrag
      taster2=seriellerEingang[1];  //setze int-Variable taster2 gleich 2. Eintrag
      
      sensor1=seriellerEingang[2];  //setze int-Variable sensor1 gleich 3. Eintrag
      sensor2=seriellerEingang[3];  //setze int-Variable sensor2 gleich 4. Eintrag
      sensor3=seriellerEingang[4];  //setze int-Variable sensor3 gleich 5. Eintrag
      sensor4=seriellerEingang[5];  //setze int-Variable sensor4 gleich 6. Eintrag
      
      benutzer=seriellerEingang[6];  //setze int-Variable benutzer gleich 7. Eintrag

      //println(taster1+"\t"+taster2);
       
      seriellerPort.write('A');  //schreibe 'A' in seriellen Port (bereit für weitere Daten)
      empfangeneSerielleBytes=0;  //setze Zählvariable gleich 0
     }
  }
}
