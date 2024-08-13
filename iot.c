#include <WiFi.h>
#include <HTTPClient.h>
#include <string.h>

#define UART Serial

  // incoming data Buffer
 char buffer[20];


// network credentials
const char* ssid = "Pixel 6";
const char* password = "10gameover";

//  ThingSpeak credentials
const char* serverName = "http://api.thingspeak.com/update";
const char* apiKey = "JBHGO5R0BXT5MCJM";
//Data Variables
int temp,humidity,rainwater;


void setup() {
  Serial.begin(115200);
  UART.begin(115200);
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);      
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to Wi-Fi");
}


void loop() {
  
  if (Serial.available() > 0) {
    // string buffer to hold the incoming data
    String buffer1 =Serial.readStringUntil('\n');
    buffer1.toCharArray(buffer, sizeof(buffer)-1);
    int length = sizeof(buffer);   
    buffer[length] = '\0'; // Null-terminate the string

    // Print the received string to the Serial Monitor
    Serial.print("Received: ");
    Serial.println(buffer);

    // convert string data to integers
    sscanf(buffer, "%d %d %d",&temp,&humidity,&rainwater);
    Serial.print("Received humid Value: ");
    Serial.println(humidity);
    Serial.print("Received temp Value: ");
    Serial.println(temp);
    Serial.print("Received Rainwater Value: ");
    Serial.println(rainwater);
  }
   


  if (WiFi.status() == WL_CONNECTED) {
   
    HTTPClient http;
   if(!(temp == 0 && rainwater == 0 && humidity == 0))
   {
    // URL with the API key and data
    String url = String(serverName) + "?api_key=" + apiKey + "&field1=" + String(temp)+"&field2=" + String(humidity)+"&field3=" + String(rainwater);

    http.begin(url);
    int httpResponseCode = http.GET();
    
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Response: " + response);
    } else {
      Serial.println("Error on HTTP request");
    }
    
    http.end();
  }// Wait before sending new data
  delay(15000); // 15 seconds 
  }

 
}