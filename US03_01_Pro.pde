import processing.serial.*;  //serial Bibliothek für serielle Kommunikation einbinden

int hintergrundFarbe=200;  //Hintergrundfarbe auf Greyscale Wert 200
int fuellFarbe=150;  //Füllfarbe auf Greyscale Wert 150

Serial seriellerPort;  //Erstellen eines Objekts seriellerPort
int[] seriellerEingang=new int[2];  //Definition eines int-Arrays seriellerEingang
int empfangeneSerielleBytes=0;  //Initalisierung der int-Variablen empfangeneSerielleBytes (Zählvariable)
int xPos,yPos;  //Definition int-Variablen xPos und yPos

boolean seriellerKontakt=false;  //Initalisierung der bool-Variable seriellerKontakt

void setup()
{
  size(300,300);  //Erstellen Canvas mit Größe 300x300 Pixel
  noStroke();  //keine Umrandung für gezeichnete Formen

  seriellerPort=new Serial(this,Serial.list()[7],9600);  //Definition des Objekts seriellerPort (8. Eintrag in Liste verfügbarer serieller Ports, baud rate=9600 bauds per second)
}

void draw()
{
  background(hintergrundFarbe);  //Füllen des Hintergrunds mit Hintergrundfarbe
  fill(fuellFarbe);  //Füllen der gezeichneten Objekte mit Füllfarbe
  if(mousePressed&&mouseButton==LEFT)  //Wenn eine Maustaste und die linke Maustaste gedrückt ist, dann...
  {
    fill(81,148,255);  //benutze als Füllfarbe RGB Farbe 81,148,255
  }
  if(mousePressed&&mouseButton==RIGHT)  //Wenn eine Maustaste und die rechte Maustaste gedrückt ist, dann...
  {
    fill(45,216,51);  //benutze als Füllfarbe RGB Farbe 45,216,51
  }
    
  ellipse(xPos,yPos,40,40);  //zeichne einen Kreis um Punkt xPos,yPos mit Radius 40 Pixel
  
  textSize(20);  //setze die Schriftgröße auf 20pt
  text(xPos,150,100);  //schreibe den Wert der Variablen xPos auf das Canvas
  text(yPos,150,120);  //schreibe den Wert der Variablen yPos auf das Canvas
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
        println("2");  //schreibe '2' in Konsole
      }
      if(mouseButton==RIGHT)  //Wenn die rechte Maustaste gedrückt ist, dann...
      {
        seriellerPort.write('1');  //schreibe '1' in seriellen Port
        println("1");  //schreibe '1' in Konsole
      }
    }
      
    seriellerEingang[empfangeneSerielleBytes]=eingangsByte;  //speicher eingangsByte an der Stelle empfangeneSerielleBytes in int-Array seriellerEingang
    empfangeneSerielleBytes++;  //erhöhe int-Variable empfangeneSerielleBytes um 1 (hochzählen)

    if(empfangeneSerielleBytes>1)  //Wenn Zählvariable größer 1 (zwei Einträge in Array), dann...
    {
      xPos=seriellerEingang[0];  //setze int-Variable xPos gleich 1. Eintrag
      yPos=seriellerEingang[1];  //setze int-Variable yPos gleich 2. Eintrag

      println(xPos+"\t"+yPos);  //schreibe beide int-Variablen in Konsole
       
      seriellerPort.write('A');  //schreibe 'A' in seriellen Port (bereit für weitere Daten)
      empfangeneSerielleBytes=0;  //setze Zählvariable gleich 0
    }
  }
}
