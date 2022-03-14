#include <PubSubClient.h>
#include <ESP8266WiFi.h>


const char *ssid =  "iswahana";     // replace with your wifi ssid and wpa2 key
const char *pass =  "87654321";
const char* mqtt_server = "test.mosquitto.org";
const int triggerPin=12;
const int echoPin=14;
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void callback(char* topic, byte* payload,unsigned int length){
    Serial.print((char)payload[0]);
    Serial.print("message arrived [");
    Serial.print(topic);
    Serial.print("]");

    for(int i=0;i<length;i++){
        Serial.print((char)payload[i]);
      }
      Serial.println();
    if ((char)payload[0] == '1') {
      digitalWrite(BUILTIN_LED,LOW);  
    }
     if ((char)payload[0] != '1') {
      digitalWrite(BUILTIN_LED,HIGH);  
    }
 }

void reconnect() {
  
  while(!client.connected()){
  Serial.println("Attempting MQTT connection");
  String clientId = "Esp8266Clients-";
  clientId += String(random(0xffff),HEX);
  
  Serial.println("Connected");
  if (client.connect(clientId.c_str())){

      Serial.println("connected");
      client.publish("firmansmoh/v1","hello Indonesia!!!!");
      client.subscribe("firmansmoh/v1");
      Serial.print("subscribed!");    
  }
  else
  {
  Serial.print("Failed, rc = ");
  Serial.print(client.state());
  Serial.println("Waiting for 5 seconds to try again");
  delay(5000);
   }
  }
}

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

   client.setServer(mqtt_server,1883);
   client.setCallback(callback);

}

void loop()
{
  
  int cm,distance;
 cm = readUltrasonicDistance(triggerPin, echoPin);
  

  distance = 0.01723 * cm;
//  Serial.print(distance); 
//  Serial.println("cm");
 //if the distance is further than 350 turn LED off
//  if (distance < 100) {
//   Serial.print("led nyala");
//  } else{
//    Serial.print("led mati");
//  }

    if (!client.connected()) {
    
      reconnect();
    }
    client.loop();

   unsigned long now = millis();
    if (now - lastMsg > 2000) {
        lastMsg = now;
        ++value;
         snprintf (msg, MSG_BUFFER_SIZE, "%ld", distance);
        Serial.print("publish message:  ");
        Serial.println(msg);
        client.publish("firmansmoh/v1",msg);
      }


  delay(500);

  
}
