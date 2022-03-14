#include <PubSubClient.h>
#include <ESP8266WiFi.h>


const char *ssid =  "iswahana";     // replace with your wifi ssid and wpa2 key
const char *pass =  "87654321";

const int triggerPin=12;
const int echoPin=14;

WiFiClient client;

long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}

void setup() {
  
  pinMode(triggerPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
  
   WiFi.begin(ssid, pass); 
       while (WiFi.status() != WL_CONNECTED) 
          {
            delay(500);
            Serial.print(".");
          }
      Serial.println("");
      Serial.println("WiFi connected"); 
}

void loop()
{
  
  int cm,distance;
 cm = readUltrasonicDistance(triggerPin, echoPin);
  

  distance = 0.01723 * cm;
  Serial.print(distance); 
  Serial.println("cm");
 //if the distance is further than 350 turn LED off
  if (distance < 100) {
   Serial.print("led nyala");
  } else{
    Serial.print("led mati");
  }
  delay(500);
}
