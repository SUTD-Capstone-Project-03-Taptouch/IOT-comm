#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiManager.h>


// Update these with values suitable for your network.
const char* ssid = "Akshaya";
const char* password = "1Abhikshaya";
const char* mqtt_server = "hairdresser.cloudmqtt.com";
#define mqtt_port 15772    //change this everytime u refresh mQtt hedge dog
#define MQTT_USER "qhoiayhk"
#define MQTT_PASSWORD "HZv4WwXWR6Y6"
#define MQTT_SERIAL_PUBLISH_CH1 "/sensor/instant/sensor1"    //transmites and publishes data to broker cl //"/taptouch/instantaneous_sensorval/tx"


//sensor pins
const int veloPins[] = {33,32,35,34,39};   //this code uses 32

WiFiClient wifiClient;
PubSubClient client(wifiClient);


void setup() {
  Serial.begin(115200);
  Serial.setTimeout(500);// Set time out for
  setup_wifi();   //wifi connection
  client.setServer(mqtt_server, mqtt_port);      //sets the server details
  client.setCallback(callback);                  //sets message call back function --> If the client is used to subscribe to topics, a callback function must be provided in the constructor. This function is called when new messages arrive at the client.
  reconnect();     //mQtt connection
  //  measure_sensorval();

  //init sensor pins
  pinMode(veloPins[0], INPUT_PULLUP);
  pinMode(veloPins[1], INPUT_PULLUP);
  pinMode(veloPins[2], INPUT_PULLUP);
  pinMode(veloPins[3], INPUT_PULLUP);
  pinMode(veloPins[4], INPUT_PULLUP);
  pinMode(veloPins[5], INPUT_PULLUP);
  pinMode(veloPins[6], INPUT_PULLUP);
  pinMode(veloPins[7], INPUT_PULLUP);
  pinMode(veloPins[8], INPUT_PULLUP);
  pinMode(veloPins[9], INPUT_PULLUP);

}



void setup_wifi() {
  delay(10);
  //WiFi setup built in
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  //WiFi setup using WiFi manager library
  //  WiFiManager manager;
  //  bool success = manager.autoConnect("ESP32_publish", "password");
  //  if (!success) {
  //    Serial.println("Failed to connect");
  //  }
  //  else {
  //    Serial.println("Connected");
  //  }

}

// Function to establish a connection to MQTT
void reconnect() {
  // Loop until we're reconnected to MQTT
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("connected");
      //Once connected, publish an announcement...
      client.publish("/flask/mqtt/ESP1", "ESP_publish is connected");
      // ... and resubscribe
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte * payload, unsigned int length) {
  Serial.println("-------new message from broker-----");
  Serial.print("channel:");
  Serial.println(topic);
  Serial.print("data:");
  Serial.write(payload, length);
  Serial.println();

}


//void publishSerialData(char *serialData) {
//  if (!client.connected()) {
//    reconnect();
//  }
//  client.publish(MQTT_SERIAL_PUBLISH_CH, serialData);
//}
 
//sensor read function
float analogToVoltageESP32(int analogVal, float voltageValue)
{
  return (analogVal * voltageValue) / 4095;
}

void loop() {
  client.loop();
  //  fsrReading = analogRead(fsrPin);


  int val1 = int(analogToVoltageESP32(analogRead(veloPins[1]), 3.3) / .33);
  Serial.print(val1);
  Serial.print("     ");
  char buf[8];
  const char *CC = itoa(val1, buf, 10);
  Serial.println(*CC);

  //const char *fsrReading = "unencrypted";
  client.publish(MQTT_SERIAL_PUBLISH_CH1, CC);
  delay(500);
  //  publishSerialData(*sensorval);

  //  if (Serial.available() > 0) {
  //    char mun[501];
  //    memset(mun, 0, 501);
  //    Serial.readBytesUntil( '\n', mun, 500);
  //    publishSerialData(mun);
  //  }
}
