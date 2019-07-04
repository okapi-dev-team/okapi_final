import processing.serial.*;  //serial Bibliothek für serielle Kommunikation einbinden

Serial seriellerPort;  //Erstellen eines Objekts seriellerPort
int[] seriellerEingang=new int[2];  //Definition eines int-Arrays seriellerEingang
int empfangeneSerielleBytes=0;  //Initalisierung der int-Variablen empfangeneSerielleBytes (Zählvariable)
int taster1,taster2;  //Definition int-Variablen taster1 und taster2 (0=nicht gedrückt, 1=gedrückt)

boolean seriellerKontakt=false;  //Initalisierung der bool-Variable seriellerKontakt

Table tabelle;  //erstelle ein Objekt tabelle in Klasse Table


void setup()
{
  size(800,1200);  //Erstellen Canvas mit Größe 800x1200 Pixel
  background(200);  //Füllen des Hintergrunds mit Greyscale Wert 200
  
  seriellerPort=new Serial(this,Serial.list()[7],9600);  //Definition des Objekts seriellerPort (8. Eintrag in Liste verfügbarer serieller Ports, baud rate=9600 bauds per second)
  
  tabelle=new Table();  //erstelle Tabelle tabelle
  
  tabelle.addColumn("Datum/Uhrzeit");  //erstelle Tabellenspalte Datum/Uhrzeit
  tabelle.addColumn("Ereignis");  //erstelle Tabellenspalte Ereignis
  tabelle.addColumn("Benutzer");  //erstelle Tabellenspalte Benutzer
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
    TableRow zeile=tabelle.addRow();  //erstelle Tabellenzeile
    zeile.setString("Datum/Uhrzeit",datumKomplettStr+" "+uhrzeitKomplettStr);  //setze Eintrag in Spalte Datum/Uhrzeit = aktuelles Datum/Uhrzeit
    zeile.setString("Ereignis","Test");  //setze Eintrag in Spalte Ereignis = Test
    zeile.setString("Benutzer","master");  //setze Eintrag in Spalte Benutzer = master
  
    saveTable(tabelle,"tabelleTest.csv","csv");  //speichere Tabelle als tabelleTest.csv
  }
  if(taster2==1)  //Wenn taster2 gedrückt, dann...
  {
    TableRow zeile=tabelle.addRow();  //erstelle Tabellenzeile
    zeile.setString("Datum/Uhrzeit",datumKomplettStr+" "+uhrzeitKomplettStr);  //setze Eintrag in Spalte Datum/Uhrzeit = aktuelles Datum/Uhrzeit
    zeile.setString("Ereignis","Test");  //setze Eintrag in Spalte Ereignis = Test
    zeile.setString("Benutzer","slave");  //setze Eintrag in Spalte Benutzer = slave
  
    saveTable(tabelle,"tabelleTest.csv","csv");  //speichere Tabelle als tabelleTest.csv
  }
  
  int zeilen=tabelle.getRowCount();  //setze int-Variable zeilen gleich der Zeilenanzahl der Tabelle
  //int spalten=tabelle.getColumnCount();
  if(zeilen>0)  //Wenn Zeilenanzahl größer 0 (Einträge in Tabelle), dann...
  {
    for(int i=0;i<zeilen;i++)  //gehe jede Zeile von Beginn an durch
    {
      textSize(20);  //setze die Schriftgröße auf 20pt
      //for(int k=0;k<spalten;k++)
      //{
        text(tabelle.getString(i,"Datum/Uhrzeit"),10,20*i+20);  //schreibe den Eintrag der Spalte Datum/Uhrzeit auf das Canvas
        text(tabelle.getString(i,"Ereignis"),280,20*i+20);  //schreibe den Eintrag der Spalte Ereignis auf das Canvas
        text(tabelle.getString(i,"Benutzer"),500,20*i+20);  //schreibe den Eintrag der Spalte Benutzer auf das Canvas
      //}      
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
    if(mousePressed==true)  //Wenn eine Maustaste gedrückt ist, dann...
    {
      if(mouseButton==LEFT)  //Wenn die linke Maustaste gedrückt ist, dann...
      {
        seriellerPort.write('2');  //schreibe '2' in seriellen Port
        //println("2");
      }
      if(mouseButton==RIGHT)  //Wenn die rechte Maustaste gedrückt ist, dann...
      {
        seriellerPort.write('1');  //schreibe '1' in seriellen Port
        //println("1");
      }
    }
      
    seriellerEingang[empfangeneSerielleBytes]=eingangsByte;  //speicher eingangsByte an der Stelle empfangeneSerielleBytes in int-Array seriellerEingang
    empfangeneSerielleBytes++;  //erhöhe int-Variable empfangeneSerielleBytes um 1 (hochzählen)

    if(empfangeneSerielleBytes>1)  //Wenn Zählvariable größer 1 (zwei Einträge in Array), dann...
    {
      taster1=seriellerEingang[0];  //setze int-Variable taster1 gleich 1. Eintrag
      taster2=seriellerEingang[1];  //setze int-Variable taster2 gleich 1. Eintrag

      //println(taster1+"\t"+taster2);
       
      seriellerPort.write('A');  //schreibe 'A' in seriellen Port (bereit für weitere Daten)
      empfangeneSerielleBytes=0;  //setze Zählvariable gleich 0
     }
  }
}
