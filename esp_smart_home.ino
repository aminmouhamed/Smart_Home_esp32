#include <Arduino.h>
#include "wifi_connections.h"
#include "DHT.h"
#include "firebase.h"

Room Bedroom ;

uint8_t Lamppin = 17 ;
uint8_t window = 16 ;
uint8_t gazpin = 32 ;
uint8_t doorpin = 5 ;
uint8_t alarmepin = 23 ;
#define DHTPIN 4 

int conter = 50 ;



#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void gettemp(){
  temp = dht.readTemperature();
  if (isnan(temp)) {
     Serial.println(F("Failed to read from DHT sensor!"));
     return; }
}

void mmain(){ // function contain all the code after connections secsess ---------------------------------------------------////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  if(conter == 50 ){
  gettemp();
  Bedroom.SetData(temp) ;
  conter = 0 ;
  }
  Bedroom.GetData() ;
  conter++;
}

void setup() {
  Serial.begin(115200) ;
  pinMode(18 , OUTPUT) ;
  pinMode(19 , OUTPUT) ;
  delay(10) ;
  dht.begin();
  setupWifi() ;
  delay(100) ;
  Bedroom.RoomName = "Bedroom" ;
  Bedroom.LampPin = &Lamppin ;
  Bedroom.WindowPin = &window ; 
  Bedroom.GazePin = &gazpin ;
  Bedroom.DoorPin = &doorpin ;
  Bedroom.AlarmePin = &alarmepin ;
  Bedroom.chek() ;
  SetupFireBase() ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {

  if ((WiFi.status() == WL_CONNECTED)) // test wifi status if is conected to any wifi 
  { 
      digitalWrite(18, true);
      digitalWrite(19, false);
      mmain() ; 
  }
  else
  {
  }

  if (testWifi())
  {
    //Serial.println(" connection status positive");
    return;
  }
  else
  {
    Serial.println("Connection Status Negative / D15 HIGH");
    Serial.println("Turning the HotSpot On");
    digitalWrite(19, true);
    digitalWrite(18, false);
    startAf();// Setup HotSpot
  }

  Serial.println();
  Serial.println("Waiting.");

  while ((WiFi.status() != WL_CONNECTED))
  {
    Serial.print(".");
    delay(100);
    startServer();
  }
  
 
}
