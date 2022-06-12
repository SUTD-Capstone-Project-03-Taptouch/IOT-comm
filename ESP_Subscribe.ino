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
#define MQTT_SERIAL_PUBLISH_CH "/taptouch/tx"    //transmites and publishes data to broker cl
#define MQTT_SERIAL_RECEIVER_CH "/taptouch/instantaneous_sensorval/tx"  //subscribe -:) receives data from broker/esp --> receive pressure matrix values

WiFiClient wifiClient;
PubSubClient client(wifiClient);

void setup_wifi() {
//  delay(10);
//  // We start by connecting to a WiFi network
//  Serial.println();
//  Serial.print("Connecting to ");
//  Serial.println(ssid);
//  WiFi.begin(ssid, password);
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//  }
//  randomSeed(micros());
//  Serial.println("");
//  Serial.println("WiFi connected");
//  Serial.println("IP address: ");
//  Serial.println(WiFi.localIP());

  WiFiManager manager;
  bool success = manager.autoConnect("ESP32_subscribe", "password");  
  if (!success) {
    Serial.println("Failed to connect");
  }
  else {
    Serial.println("Connected");
  }
   
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("connected");
      //Once connected, publish an announcement...
      client.publish("/taptouch/setup", "connected");
      // ... and resubscribe
      client.subscribe(MQTT_SERIAL_RECEIVER_CH);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte *payload, unsigned int length) {
  Serial.println("-------new message from broker-----");
  Serial.print("channel:");
  Serial.println(topic);
  Serial.print("data:");
  Serial.write(payload, length);
  Serial.println();

}

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(500);// Set time out for
  setup_wifi();   //wifi connection
  client.setServer(mqtt_server, mqtt_port);      //sets the server details
  client.setCallback(callback);                  //sets message call back function --> If the client is used to subscribe to topics, a callback function must be provided in the constructor. This function is called when new messages arrive at the client.
  reconnect();     //mQtt connection
  //  measure_sensorval();
}

void publishSerialData(char *serialData) {
  if (!client.connected()) {
    reconnect();
  }
  client.publish(MQTT_SERIAL_PUBLISH_CH, serialData);
}


void loop() {
  client.loop();

}
