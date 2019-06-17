Table tabelle;  //erstelle ein Objekt tabelle in Klasse Table


void setup()
{
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
  
  tabelle=new Table();  //erstelle Tabelle tabelle
  
  tabelle.addColumn("Datum/Uhrzeit");  //erstelle Tabellenspalte Datum/Uhrzeit
  tabelle.addColumn("Ereignis");  //erstelle Tabellenspalte Ereignis
  tabelle.addColumn("Benutzer");  //erstelle Tabellenspalte Benutzer
  
  TableRow zeile=tabelle.addRow();  //erstelle Tabellenzeile
  zeile.setString("Datum/Uhrzeit",datumKomplettStr+" "+uhrzeitKomplettStr);  //setze Eintrag in Spalte Datum/Uhrzeit = aktuelles Datum/Uhrzeit
  zeile.setString("Ereignis","Test");  //setze Eintrag in Spalte Ereignis = Test
  zeile.setString("Benutzer","master");  //setze Eintrag in Spalte Benutzer = master
  
  saveTable(tabelle,"tabelleTest.csv","csv");  //speichere Tabelle als tabelleTest.csv
}
