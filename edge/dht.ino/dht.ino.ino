#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <time.h>
#include <stdint.h>
#include "credentials.h"
#include "conf.h"
#include "edge.h"

#define DHTPIN 21     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22 // DHT 22 (AM2302)

DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

uint64_t T0NTP;
uint64_t T0Milis;

// NTP config
const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;     // Adjust for your timezone (e.g., -3600 for GMT-1)
const int daylightOffset_sec = 3600; // Adjust for daylight saving time

void setup()
{
  Serial.begin(115200);
  setup_wifi(ssid, password);
  client.setServer(mqtt_server, 1883);
  setupTime();
  dht.begin();
  Serial.println("Setup completed successfully");
}

void loop()
{
  if (!client.connected())
  {
    reconnect_Mqtt();
  }
  client.loop();

  // Read temperature and humidity
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  uint64_t timestamp = unixMilis() + 7200;
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t Temperature: ");
  Serial.print(t);
  Serial.print(" °C");
  Serial.print("\t Timestamp: ");
  Serial.println(timestamp);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t))
  {
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
  JsonObject timeFieldTemp = jsonDocTemp["schema"]["fields"].createNestedObject();
  timeFieldTemp["type"] = "int64";
  timeFieldTemp["optional"] = false;
  timeFieldTemp["field"] = "timestamp";
  jsonDocTemp["payload"] = JsonObject();
  jsonDocTemp["payload"]["temperature"] = t;
  jsonDocTemp["payload"]["timestamp"] = timestamp + 7200000; // To correct the GMT+2

  char jsonBufferTemp[400];
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
  JsonObject timeFieldHumi = jsonDocHumi["schema"]["fields"].createNestedObject();
  timeFieldHumi["type"] = "int64";
  timeFieldHumi["optional"] = false;
  timeFieldHumi["field"] = "timestamp";
  jsonDocHumi["payload"] = JsonObject();
  jsonDocHumi["payload"]["humidity"] = h;
  jsonDocHumi["payload"]["timestamp"] = timestamp + 7200000; // To correct the GMT+2

  char jsonBufferHumi[400];
  serializeJson(jsonDocHumi, jsonBufferHumi);
  client.publish("{\"humidity\":\"string\"}", jsonBufferHumi);

  delay(2000); // Wait for 2 seconds before sending the next message
}
