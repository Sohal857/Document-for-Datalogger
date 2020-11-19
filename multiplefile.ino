#include <WiFi.h>
#include "time.h"

const char* ssid       = "ESD";
const char* password   = "esd_2019";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 6*60*60;
const int   daylightOffset_sec = 0;




void setup()
{
  Serial.begin(115200);
  
  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

}

void loop()
{

  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  } 
  char timeHour[30];
  strftime(timeHour,20, "%Y-%m-%d %H:%M:%S", &timeinfo);
  Serial.println(timeHour);
   delay(1000);
}
