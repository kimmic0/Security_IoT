//client code
#include <ArduinoMqttClient.h>
#include <WiFi101.h>
//WiFi that you want to be connected
char ssid[] = "RojasFam"; // your network SSID
char pass[] = "L0v3sh0t"; // your network password
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);
const char broker[] = "test.mosquitto.org"; //your broker here is mosquitto
int port = 1883; //channel of the broker
//topics, you can change the name
const char topicDistance[] = "sensors/distance";
const char topicFlex[] = "sensors/flex";
const char topicLight[] = "sensors/light";
void setup() 
{
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
  ; 
  }
  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
  // failed, retry
  Serial.print(".");
  delay(5000);
  }
  Serial.println("You're connected to the network");
  Serial.println();
  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);
  if (!mqttClient.connect(broker, port)) {
  Serial.print("MQTT connection failed! Error code = ");
  Serial.println(mqttClient.connectError());
  while (1);
  }
  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  mqttClient.subscribe(topicDistance);
  mqttClient.subscribe(topicFlex);
  mqttClient.subscribe(topicLight);
  
  Serial.print("Subscribing to topics: ");
  Serial.println(topicDistance);
  Serial.println(topicFlex);
  Serial.println(topicLight);
  Serial.println();
  mqttClient.onMessage(onMqttMessage);
  Serial.println();
}
void loop() {
mqttClient.poll();
}
//print the MQTT message
void onMqttMessage(int messageSize) {
  String topic = mqttClient.messageTopic();
  String message = "";

  while(mqttClient.available())
  {
    message += (char)mqttClient.read();
  }
Serial.println("Received a message with topic '");
Serial.print(topic);
Serial.print("', length ");
Serial.print(messageSize);
Serial.println(" bytes:");

if(topic == "sensors/distance")
{
  Serial.print("Distance: ");
  Serial.println(message);
}
else if( topic == "sensors/flex")
{
  Serial.print("Flex: ");
  Serial.println(message);
}
else{
  Serial.print("Light: ");
  Serial.println(message);
}
while (mqttClient.available()) {
Serial.print((char)mqttClient.read());
}
Serial.println();
Serial.println();
}