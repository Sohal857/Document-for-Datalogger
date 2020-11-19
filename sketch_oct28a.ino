#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h> 
#include "time.h"


const char* ssid = "ESD";
const char* password =  "esd_2019";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 6*60*60;
const int   daylightOffset_sec = 0;

  
void setup() {
  
  Serial.begin(115200);
  delay(400);   //Delay needed before calling the WiFi.begin
  
  WiFi.begin(ssid, password); 
  
  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  
  Serial.println("Connected to the WiFi network");
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  
}
  
void loop() {
  
 if(WiFi.status()== WL_CONNECTED)
 {  
  //Check WiFi connection status
  
     HTTPClient http;   
  
     http.begin("http://192.168.1.13:8000/datas/");  //Specify destination for HTTP request
     http.addHeader("Content-Type", "application/json");             //Specify content-type header


     struct tm timeinfo;
     if(!getLocalTime(&timeinfo)){
     Serial.println("Failed to obtain time");
     return;
     } 
     char timeHour[20];
     strftime(timeHour,20, "%Y-%m-%d %H:%M:%S", &timeinfo);
     Serial.println(timeHour);
     String send_str = "{\"temperature\":\"23.0\",\"relative_humidity\":\"45.0\",\"barometric_pressure\":\"54.0\",\"precipitation\":\"45.0\",\"incoming_solar_radiation\":\"54.0\", \"outgoing_solar_radiation\":\"45.0\",\"wind_speed\":\"56.0\",\"wind_direction\":\"546\",\"photosynthetically_active_radiation\":\"45.0\", \"captured_sensor_time_date\":\"";
     send_str += timeHour;
     send_str += "\"}";
     
     int httpResponseCode = http.POST(send_str);   //Send the actual POST request
  //"{",
 //     ,
  //   }"
     if(httpResponseCode>0){
  
        String response = http.getString();                       //Get the response to the request
  
        Serial.println(httpResponseCode);   //Print return code
        Serial.println(response);           //Print request answer
  
    }
    else{
  
        Serial.print("Error on sending POST: ");
        Serial.println(httpResponseCode);
  
    }
  
   http.end();  //Free resources
  
 }else{
  
    Serial.println("Error in WiFi connection");   
  
 }
  
  delay(10000);  //Send a request every 10 seconds
  
}
