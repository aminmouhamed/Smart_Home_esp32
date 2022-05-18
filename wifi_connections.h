#include <WiFi.h>
#include <EEPROM.h>

// define hear ssid and password FOR ACCESS POINT .
////////////////////////////////
#define SSID "SMART-HOME"
#define PASSWORD NULL 
////////////////////////////////
////////////////////////////////
//EEPROM VARIABEL
////////////////////////////////
String esid;
String epass = "";
////////////////////////////////
/////////////////////////////////

// define tcp server informations .
////////////////////////////////
#define PORT 8080
WiFiServer Server(PORT) ;
///////////////////////////////
//////////////////////////////


void startAf(){ // functions to start access point . ----------------------------------------------------------------
    WiFi.disconnect();
    delay(100);
    Serial.println("\n[*] Creating AP");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(SSID, PASSWORD);
    Serial.print("[+] AP Created with IP Gateway ");
    Serial.println(WiFi.softAPIP());
  
  }
void startServer(){ // functions to start tcp server . ----------------------------------------------------------------
  
  Server.begin();
  WiFiClient client = Server.available();
  String qssid ;
  String qpass ;

  if (client) {
    if(client.connected())
    {
      Serial.println("Client Connected");
    }
    
    while(client.connected()){   
      delay(1000) ;   
      while(client.available()>0){
        // read data from the connected client
        qssid += char(client.read()); 
        
      }
       delay(1000);
        client.write('o');
   delay(1000);
      
       while(client.available()>0){
        // read data from the connected client
        qpass += char(client.read()); 
       
      }
      
    }
    client.stop();
    Serial.println("Client disconnected");   
     Serial.println(qssid);
     Serial.println(qpass);
  if (qssid.length() > 0 && qpass.length() > 0) {
        Serial.println("clearing eeprom");
        for (int i = 0; i < 96; ++i) {
          EEPROM.write(i, 0);
        }
        Serial.println(qssid);
        Serial.println("");
        Serial.println(qpass);
        Serial.println("");
  }
     // save wifi info to EEPROM -------------------------------------------------------------------------
    Serial.println("writing eeprom ssid:");
  for (int i = 0; i < qssid.length(); ++i)
      {
        EEPROM.write(i, qssid[i]);
        Serial.print("Wrote: ");
        Serial.println(qssid[i]);
      }
  Serial.println("writing eeprom pass:");
  for (int i = 0; i < qpass.length(); ++i)
      {
        EEPROM.write(32 + i, qpass[i]);
        Serial.print("Wrote: ");
        Serial.println(qpass[i]);
      }
  EEPROM.commit();
  ESP.restart();
  }
  
}
bool testWifi(){ // functions to test wifi connections ----------------------------------------------------------------------
  
 int c = 0;
  while ( c < 20 ) {
    if (WiFi.status() == WL_CONNECTED)
    {
      return true;
    }
    delay(500);
    Serial.print("*");
    c++;
  }
  Serial.println("");
  Serial.println("Connect timed out, opening AP");
  return false;
  }

  void setupWifi(){
      WiFi.disconnect();  
  
        EEPROM.begin(512); /////Initialasing EEPROM
        delay(10);
        // read ssid and password from the EEPROM
        for (int i = 0; i < 32; ++i)
        {
            esid += char(EEPROM.read(i));
        }
        Serial.println();
        Serial.print("SSID: ");
        Serial.println(esid.c_str());
        Serial.println("Reading EEPROM pass");

        for (int i = 32; i < 96; ++i)
        {
            epass += char(EEPROM.read(i));
        }
        Serial.print("PASS: ");
        Serial.println(epass.c_str());
        WiFi.begin(esid.c_str(), epass.c_str());
        delay(100) ;

  }
