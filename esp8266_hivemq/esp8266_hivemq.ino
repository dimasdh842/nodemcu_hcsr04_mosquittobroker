#include <PubSubClient.h>
#include <ESP8266WiFi.h>

const char* ssid = "iswahana";
const char* password = "87654321";
const char* mqtt_server = "test.mosquitto.org";




WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;


void setup_wifi()
{

delay(10);

Serial.print("connecting to");
 Serial.println(ssid);
 WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED)
 {
 delay(500);
 Serial.print("-");
 }

Serial.println();
 Serial.println("WiFi Connected");
 Serial.println("WiFi got IP");
 Serial.println();
 Serial.println(WiFi.localIP());
}

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
     if ((char)payload[0] == '0') {
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

void setup(){

   pinMode(BUILTIN_LED,OUTPUT);
   Serial.begin(9600);
   setup_wifi();
   client.setServer(mqtt_server,1883);
   client.setCallback(callback);

   
 }

void loop(){

  if (!client.connected()) {
    
      reconnect();
    }
    client.loop();

//    unsigned long now = millis();
//    if (now - lastMsg > 2000) {
//        lastMsg = now;
//        ++value;
//         snprintf (msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
//        Serial.print("publish message:  ");
//        Serial.println(msg);
//        client.publish("firmansmoh/v1",msg);
//      }

   
 }
