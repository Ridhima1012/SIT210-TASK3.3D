#include <ArduinoMqttClient.h>
#if defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_AVR_UNO_WIFI_REV2)
#include <WiFiNINA.h>
#elif defined(ARDUINO_SAMD_MKR1000)
#include <WiFi101.h>
#elif defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#endif

// Establish Wi-Fi credentials 
char ssid[] = "RIDHIMA";  
char pass[] = "123456789";    

// the connection of pins of Utrasonic sensor 
const int trigPin = 2;
const int echoPin = 3;

float duration, distance;


//Building a WiFi connection and establishing a MQTT connection to communicate via the WiFi connection
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);


// Here are the credentials for MQTT broker 
const char broker[] = "broker.mqttdashboard.com";
int        port     = 1883;
const char topic[]  = "SIT210/waves";


const long interval = 1000;
unsigned long previousMillis = 0;

int count = 0;


//Establishing up serial transmission at a speed of 9600 baud and anticipating it to get complete.
void setup() {
  
  Serial.begin(9600);
  while (!Serial) {
    ;
  }


  Serial.print("Trying to establish a connection with a WPA SSID.: ");
  Serial.println(ssid);

  // Make a connection to link to a WiFi network.
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {

    Serial.print(".");
    delay(5000);
  }

  Serial.println("Succesful connection to network");
 
 // Connecting to MQTT
  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);


//If an MQTT connect with the brokers is unsuccessful, it publishes the incorrect information and starts an infinite loop.
  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
}

void loop() {
   // Inspect and process MQTT signals or occurrences via the MQTT broker.
  mqttClient.poll();

 //This track the period of time which has passed and performs a function once the defined period of time ends.
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval)
  {

    previousMillis = currentMillis;

// sensor measuring the distance
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = (duration * .0343) / 2;
    Serial.print("Distance: ");
    Serial.println(distance);


    if (distance < 12)   // applyin if else condition , that when distance is less than 12, a MQTT message sent
    {
      
      mqttClient.beginMessage(topic);
      mqttClient.print("Tanya Shiv : Wave is detected, ");
      mqttClient.print("Distance: ");
      mqttClient.print(distance);
      mqttClient.endMessage();
      delay(1000);
      Serial.print("SENT!!!");
    }

    Serial.println();

    count++;
  }
}