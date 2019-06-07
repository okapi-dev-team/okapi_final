class user  //Initalisierung einer Klasse user mit privaten Attributen und öffentlichen Methoden
{
  private:
    String nameStr,rechteStr,codeStr,rfidUserStr;
   
  public:
    void userAnlegen(String nameStr, String rechteStr, String codeStr, String rfidUserStr);
    String user[4];
};

/** \brief Legt ein String Array mit Strings nameStr, rechteStr, codeStr, rfidUserStr an
 *  \param nameStr Name des Benutzers
 *  \param rechteStr Zugriffsrechte des Benutzers (master oder slave)
 *  \param codeStr Pin-Code des Benutzers
 *  \param rfidUserStr RFID-Karten ID der Benutzer RFID-Karte
 */
void user::userAnlegen(String nameStr, String rechteStr, String codeStr, String rfidUserStr)
{
  user[0]=nameStr;
  user[1]=rechteStr;
  user[2]=codeStr;
  user[3]=rfidUserStr;
}

void setup()
{
  user user1,user2; //Definition der Objekte user1 und user2 in Klasse user

    
  Serial.begin(9600); //Starten der seriellen Kommunikation mit 9600 bauds per second

  user1.userAnlegen("Nutzer1","master","1234","133120249101");  //Anlegen des 1. Nutzers mit Benutzername, Zugriffsrechten, Pin-Code und RFID-Karten ID
  user2.userAnlegen("Nutzer2","slave","0000","1661716019"); //Anlegen des 2. Nutzers mit Benutzername, Zugriffsrechten, Pin-Code und RFID-Karten ID

  for(int i=0;i<4;i++)  //Ausgabe des 1. Nutzerprofils in seriellem Monitor
  {
    Serial.println(user1.user[i]);
  }

  for(int i=0;i<4;i++)  //Ausgabe des 2. Nutzerprofils in seriellem Monitor
  {
    Serial.println(user2.user[i]);
  }
}

void loop()
{
}
