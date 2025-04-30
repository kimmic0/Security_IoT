//publisher code
#include "ThingSpeak.h"
#include <ArduinoMqttClient.h>
#include <WiFi101.h>
#define BLYNK_TEMPLATE_ID "TMPL2KIo5FwVI"
#define BLYNK_TEMPLATE_NAME "Kimberly"
#define BLYNK_AUTH_TOKEN "9hP9yGXO3NndpLtPTAjbZ_Ekuo-yn7BH"
#define BLYNK_PRINT SerialUSB
#include <BlynkSimpleWiFiShield101.h>
#include <SPI.h>
#define lightPin  A2
#define flexPin  A3
char auth[] = "9hP9yGXO3NndpLtPTAjbZ_Ekuo-yn7BH";

const int echoPin = 7;
const int trigPin = 6;

long duration = 0.0;
float distance = 0.0;
int lightval = 0;
int data = 0;

unsigned long myChannelNumber = 2784201;
const char * myWriteAPIKey = "88FWY1MDQ6BNEGQL";

char ssid[] = "RojasFam";
char pass[] = "L0v3sh0t";
int status = WL_IDLE_STATUS;
WiFiClient wifiClient;
MqttClient mqttClient (wifiClient);
const char broker[] = "test.mosquitto.org";
int port = 1883;

const char topicDistance[] = "sensors/distance";
const char topicFlex[] = "sensors/flex";
const char topicLight[] = "sensors/light";

const long interval = 8000;
unsigned long prevMill = 0;
int count = 0;


void setup() {
  // put your setup code here, to run once:
  // Debug console
  Serial.begin(9600);
  while (!Serial) {
  ; // wait for serial port to connect. Needed for native USB port only
}
  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
  // failed, retry
  Serial.print(".");
  delay(5000);
}
  Serial.println("You're connected to the network");
  Serial.println();
  //connection to the broker
  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);
  //connection to the broker failed
  if (!mqttClient.connect(broker, port)) {
  Serial.print("MQTT connection failed! Error code = ");
  Serial.println(mqttClient.connectError());
  while (1);
  }
  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
  Blynk.begin(auth, ssid, pass);
  ThingSpeak.begin(wifiClient);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(flexPin, INPUT);
  pinMode(lightPin, INPUT);
  delay(15000);

}

void loop() {
  Blynk.run();
  status = WiFi.begin(ssid, pass);
  mqttClient.poll();
  unsigned long currentMillis = millis();
  if (currentMillis - prevMill >= interval) {
  // save the last time a message was sent
  prevMill = currentMillis;  
    //acquire distance 
  digitalWrite(trigPin, LOW);
  delay(1);
  digitalWrite(trigPin, HIGH);
  delay(1);
  digitalWrite(trigPin, LOW);

  //distance calculation
  duration = pulseIn(echoPin, HIGH);
  distance = (duration * .034) /2;

  //prints the distance

  if(distance <= 30)
  {
    Serial.print("unknown object in proximity.");
  }
  Serial.print("Sending message to topicDistance: ");
  Serial.println(topicDistance);
  Serial.println(distance); 
  mqttClient.beginMessage(topicDistance);
  mqttClient.println(distance);
  mqttClient.endMessage();   
  Blynk.virtualWrite(V1, distance);
  ThingSpeak.setField(1, distance);

  data = analogRead(flexPin);
  if( data < 10 )
  {
    Serial.println("object detected on floor");
  }

  Serial.print("Sending message to topicFlex: ");
  Serial.println(topicFlex);
  Serial.println(data); 
  mqttClient.beginMessage(topicFlex);
  mqttClient.println(data);
  mqttClient.endMessage();
  Blynk.virtualWrite(V3, data);  
  ThingSpeak.setField(2, data);

  lightval = analogRead(lightPin);
  if(lightval > 20 )
  {
    Serial.println("Suspicious activity");
    Serial.print("Light: ");
    Serial.println(lightval);
  }

  Serial.print("Sending message to topicLight: ");
  Serial.println(topicLight);
  Serial.println(lightval); 
  mqttClient.beginMessage(topicLight);
  mqttClient.println(lightval);
  mqttClient.endMessage(); 
   Blynk.virtualWrite(V2, lightval);  
   ThingSpeak.setField(3, lightval);
   ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  }
}
