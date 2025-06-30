#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <time.h>
#include <stdint.h>
#include "credentials.h"
#include "conf.h"
#include "edge.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define DS18B20 13

OneWire ourWire(DS18B20);
DallasTemperature sensor(&ourWire);

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
  sensor.begin();
  Serial.println("Setup completed successfully");
}

void loop()
{
  if (!client.connected())
  {
    reconnect_Mqtt();
  }
  client.loop();

  // Read temperature
  sensor.requestTemperatures();
  float temperature = sensor.getTempCByIndex(0);
  uint64_t timestamp = unixMilis() + 7200;
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("\t Timestamp: ");
  Serial.println(timestamp);

  // Publish Temperature data
  JsonDocument jsonDocTemperature;
  jsonDocTemperature["schema"] = JsonObject();
  jsonDocTemperature["schema"]["type"] = "struct";
  jsonDocTemperature["schema"]["fields"] = JsonArray();
  JsonObject temperatureField = jsonDocTemperature["schema"]["fields"].createNestedObject();
  temperatureField["type"] = "float";
  temperatureField["optional"] = false;
  temperatureField["field"] = "temperature_soil";
  JsonObject timeFieldTemperature = jsonDocTemperature["schema"]["fields"].createNestedObject();
  timeFieldTemperature["type"] = "int64";
  timeFieldTemperature["optional"] = false;
  timeFieldTemperature["field"] = "timestamp";
  jsonDocTemperature["payload"] = JsonObject();
  jsonDocTemperature["payload"]["temperature_soil"] = temperature;
  jsonDocTemperature["payload"]["timestamp"] = timestamp + 7200000; // To correct the GMT+2

  char jsonBufferTemperature[400];
  serializeJson(jsonDocTemperature, jsonBufferTemperature);
  client.publish("{\"temperature_soil\":\"string\"}", jsonBufferTemperature);

  delay(2000); // Wait for 2 seconds before sending the next message
}
