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

int light = 2;


//Building a WiFi connection and establishing a MQTT connection to communicate via the WiFi connection
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);


// Here are the credentials for MQTT broker
const char broker[] = "mqtt-dashboard.com";
int        port     = 1883;
const char topic[]  = "wave";

void setup() {

//Building up serial connectivity, setting an illuminated pin as results, and awaiting for a sequential connection to be formed.
  Serial.begin(9600);
  pinMode(light, OUTPUT);
  while (!Serial) {
    ; 
  }


//Trying a connection to the stipulated WPA SSID and display status indicators till success.

  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);


//When the MQTT link is unsuccessful, display a message and start a never-ending loop.

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }


//It indicates a successful MQTT connect communication, specifies subject subscription, and puts break lines for clarification.
  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  Serial.print("Subscribing to topic: ");
  Serial.println(topic);
  Serial.println();

  mqttClient.subscribe(topic);

//It suggests the programme is expecting data on the given MQTT subject.
  Serial.print("Waiting for messages on topic: ");
  Serial.println(topic);
  Serial.println();
}

void loop() {

  //When I get a message, display its subject and length in bytes.
  int messageSize = mqttClient.parseMessage();
  if (messageSize) {
    
    Serial.print("Received a message with topic '");
    Serial.print(mqttClient.messageTopic());
    Serial.print("', length ");
    Serial.print(messageSize);
    Serial.println(" bytes:");

  //It checks and provides MQTT message data character by character by blinking a light at certain times.

    while (mqttClient.available()) {
      Serial.print((char)mqttClient.read());
    }
    Serial.println();
    digitalWrite(light, HIGH);   
    delay(200);                       
    digitalWrite(light, LOW);    
    delay(200);
    digitalWrite(light, HIGH);   
    delay(200);                  
     digitalWrite(light, LOW);    
    delay(200);
    digitalWrite(light, HIGH);   
    delay(200);                      
    digitalWrite(light, LOW);    
      delay(200);

    Serial.println();
  }
}
