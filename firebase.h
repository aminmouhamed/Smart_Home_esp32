#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

//defin hear api_kek and database_url 
////////////////////////////////
#define API_KEY "AIzaSyCa4HRX_e2emSo6L4mQNu4PgSgOwGYKPmo"
#define DATABASE_URL "https://samrthome-eadb8-default-rtdb.europe-west1.firebasedatabase.app/"

#define USER_EMAIL "esp32@gmail.com"
#define USER_PASSWORD "123456"

FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;
float temp = 0.1;
bool taskCompleted = false;

void SetupFireBase(){////////functin to setup firebase --------------------------------------------------------------------
    config.api_key = API_KEY;
  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;
  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  
  }

class Room{
  
  public : 
  String RoomName = ""; 
  uint8_t* WindowPin = NULL ;
  uint8_t* LampPin = NULL ;
  uint8_t* DorPin = NULL ;
  uint8_t* GazePin = NULL ;
  
  void chek() ;
  void GetData() ;
  void SetData(float temp) ;
  void SetHistory() ;
  bool swap(bool state) ;
  void OpenWindowIf(int smoke) ;
  private :

};



void Room::SetHistory(){
/*
 if (Firebase.ready() && !taskCompleted)
  {
    if (Firebase.RTDB.setFloat(&fbdo,"Bedroom/temp", temp)) {
    //Serial.println("PASSED");
    //Serial.println("PATH: " + fbdo.dataPath());
   // Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  
    if (Firebase.RTDB.setInt(&fbdo,"Bedroom/smoke", analogRead(*GazePin))) {
     

    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  }

*/
}

void Room::chek(){
  if (RoomName == ""){
    Serial.println("Room name is null !! .") ;
  }else
  {
    if(this->LampPin != NULL && this->WindowPin != NULL && this->GazePin != NULL && DorPin != NULL){
      Serial.println("pin confirations is don ! ") ;
      pinMode(*LampPin , OUTPUT) ;
      pinMode(*WindowPin , OUTPUT) ;
      pinMode(*DorPin , OUTPUT) ;
     
    }
    else{
      Serial.println("you are messing to config your pins !!") ;
    }
  }
}

bool Room::swap(bool state){ // this functions to swap values from true to false ext ......
  if (state)
  {
    return false ;
  }else{
    return true ;
  }
  

}

void Room::GetData(){ // that functions to get data from firebase an control the lamp and window
  
  if (Firebase.ready() && !taskCompleted) /// check if firebase is redy !.
  {
    //// /// /// ///    => Get value of light_baulb from firebase . 
     if (Firebase.RTDB.getBool(&fbdo,"Bedroom/light_baulb")) {
        if (fbdo.dataType() == "boolean") {
          bool BoolValue = fbdo.boolData();
          //Serial.println(*LampPin);
          //Serial.println(BoolValue);
          digitalWrite(*LampPin, swap(BoolValue));
        }
    }
    else {
      Serial.println(fbdo.errorReason());
    }
      ///// ///         /// => Get value of window from firebase .
     if (Firebase.RTDB.getBool(&fbdo,"Bedroom/window")) {
        if (fbdo.dataType() == "boolean") {
          bool BoolValue = fbdo.boolData();
          //Serial.println(*WindowPin);
         // Serial.println(BoolValue);
          digitalWrite(*WindowPin, swap(BoolValue));
        }
    }
    else {
      Serial.println(fbdo.errorReason());
    }
      /// ///// //// =>  Get value of dor from firebase .
     if (Firebase.RTDB.getBool(&fbdo,"Bedroom/Door")) {
        if (fbdo.dataType() == "boolean") {
          bool BoolValue = fbdo.boolData();
          //Serial.println(*LampPin);
          //Serial.println(BoolValue);
          digitalWrite(*DorPin, swap(BoolValue));
        }
    }
    else {
      Serial.println(fbdo.errorReason());
    }


  }else{
    Serial.println("firebase not redy !");
    }

  
}


void Room::SetData(float temp){
  if (Firebase.ready() && !taskCompleted)
  {
    if (Firebase.RTDB.setFloat(&fbdo,"Bedroom/temp", temp)) {
    //Serial.println("PASSED");
    //Serial.println("PATH: " + fbdo.dataPath());
   // Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  
    if (Firebase.RTDB.setInt(&fbdo,"Bedroom/smoke", analogRead(*GazePin))) {
      if(analogRead(*GazePin) >= 1000 ){
      digitalWrite(*WindowPin, swap(true));
       if (Firebase.RTDB.setBool(&fbdo,"Bedroom/window" , true)) {
       
       
         }
       else {
         Serial.println("FAILED");
            Serial.println(fbdo.errorReason());
    }
      }else{
         digitalWrite(*WindowPin, swap(false));
       if (Firebase.RTDB.setBool(&fbdo,"Bedroom/window" , false)) {
       
       
         }
       else {
         Serial.println("FAILED");
            Serial.println(fbdo.errorReason());
    }
        
        
        }
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  }

  
}
