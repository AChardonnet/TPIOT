#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include "credentials.h"
#include "conf.h"

#define DHTPIN 21     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22 (AM2302)

DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      JsonDocument jsonDocTest;
      jsonDocTest["test"] = "hello world";
      char jsonBufferTest[200];
      serializeJson(jsonDocTest, jsonBufferTest);
      client.publish("{\"test\": \"string\"}", jsonBufferTest);
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  dht.begin();
  Serial.println("Setup completed successfully");
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Read temperature and humidity
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t Temperature: ");
  Serial.print(t);
  Serial.println(" °C");

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000); // Wait for 2 seconds before sending the next message
    return;
  }

  // Publish temperature and humidityJsonDocument jsonDocTemp;
  JsonDocument jsonDocTemp;
  jsonDocTemp["schema"] = JsonObject();
  jsonDocTemp["schema"]["type"] = "struct";
  jsonDocTemp["schema"]["fields"] = JsonArray();
  JsonObject tempField = jsonDocTemp["schema"]["fields"].createNestedObject();
  tempField["type"] = "float";
  tempField["optional"] = false;
  tempField["field"] = "temperature";
  jsonDocTemp["payload"] = JsonObject();
  jsonDocTemp["payload"]["temperature"] = t;

  char jsonBufferTemp[200];
  serializeJson(jsonDocTemp, jsonBufferTemp);
  client.publish("{\"temperature\":\"string\"}", jsonBufferTemp);

  JsonDocument jsonDocHumi;
  jsonDocHumi["schema"] = JsonObject();
  jsonDocHumi["schema"]["type"] = "struct";
  jsonDocHumi["schema"]["fields"] = JsonArray();
  JsonObject humField = jsonDocHumi["schema"]["fields"].createNestedObject();
  humField["type"] = "float";
  humField["optional"] = false;
  humField["field"] = "humidity";
  jsonDocHumi["payload"] = JsonObject();
  jsonDocHumi["payload"]["humidity"] = h;

  char jsonBufferHumi[200];
  serializeJson(jsonDocHumi, jsonBufferHumi);
  client.publish("{\"humidity\":\"string\"}", jsonBufferHumi);

  delay(2000); // Wait for 2 seconds before sending the next message
}
