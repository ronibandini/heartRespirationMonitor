/*
Heart and breath sensor
MIT License
Roni Bandini, August 2024, @RoniBandini
Server ESP32-C6 + C1001 mmWave Human Detection Sensor
Client Unihiker with Carrier Board
Note: It will take some minutes to warm up

 */

#include <WiFi.h>
#include "DFRobot_HumanDetection.h"

DFRobot_HumanDetection hu(&Serial1);

const char *ssid = "";
const char *password = "";

NetworkServer server(80);

int heartRate=0;
int breathValue=0;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 4, 5);

  Serial.println("Start initialization");
  while (hu.begin() != 0) {
    Serial.println("init error!!!");
    delay(1000);
  }
  Serial.println("Sensor initialization successful");


Serial.println("Start switching work mode");
  while (hu.configWorkMode(hu.eSleepMode) != 0) {
    Serial.println("error!!!");
    delay(1000);
  }
  Serial.println("Work mode switch successful");

  Serial.print("Current work mode:");
  switch (hu.getWorkMode()) {
    case 1:
      Serial.println("Fall detection mode");
      break;
    case 2:
      Serial.println("Sleep detection mode");
      break;
    default:
      Serial.println("Read error");
  }

  hu.configLEDLight(hu.eHPLed, 1);   
  hu.sensorRet();                    

  Serial.print("HP LED status:");
  switch (hu.getLEDLightState(hu.eHPLed)) {
    case 0:
      Serial.println("Off");
      break;
    case 1:
      Serial.println("On");
      break;
    default:
      Serial.println("Read error");
  }

  delay(10);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {

  NetworkClient client = server.accept();  

  if (client) {                     
    Serial.println("New Client.");   
    String currentLine = "";         
    while (client.connected()) {     
      if (client.available()) {     
        char c = client.read();      
        Serial.write(c);             
        if (c == '\n') {            

          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.print(String(heartRate)+","+breathValue);            
            client.println();
            break;
          } else {  / 
            currentLine = "";
          }
        } else if (c != '\r') {   
          currentLine += c;       
        }

      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }

   Serial.print("Existing information:");
  switch (hu.smHumanData(hu.eHumanPresence)) {
    case 0:
      Serial.println("No one is present");
      break;
    case 1:
      Serial.println("Someone is present");
      break;
    default:
      Serial.println("Read error");
  }

  Serial.print("Motion information:");
  switch (hu.smHumanData(hu.eHumanMovement)) {
    case 0:
      Serial.println("None");
      break;
    case 1:
      Serial.println("Still");
      break;
    case 2:
      Serial.println("Active");
      break;
    default:
      Serial.println("Read error");
  }

  Serial.printf("Body movement parameters:%d\n", hu.smHumanData(hu.eHumanMovingRange));
  Serial.printf("Respiration rate:%d\n", hu.getBreatheValue());
  Serial.printf("Heart rate:%d\n", hu.gitHeartRate());
  Serial.println();

   // Set values to send
  heartRate = hu.gitHeartRate();
  breathValue=hu.getBreatheValue();
  
    delay(1000);
}
