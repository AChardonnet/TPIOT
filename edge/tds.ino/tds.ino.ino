#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <time.h>
#include <stdint.h>
#include "credentials.h"
#include "conf.h"
#include "edge.h"

#define TDSPIN 21        // Digital pin connected to the TDS sensor
const float DEFAULT_TEMPERATURE = 25.0; // Default temperature for TDS calculation

WiFiClient espClient;
PubSubClient client(espClient);

uint64_t T0NTP;
uint64_t T0Milis;

// NTP config
const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;     // Adjust for your timezone (e.g., -3600 for GMT-1)
const int daylightOffset_sec = 3600; // Adjust for daylight saving time

float computeTDS(float voltage, float temperature)
{
  float Cc = 1 + 0.2 * (temperature - 25);
  float Cv = voltage / Cc;                                          // Convert voltage to volts
  float t = 133.42 * Cv * Cv * Cv - 255.86 * Cv * Cv + 857.39 * Cv; // TDS in ppm
  return t / 2;
}

float tds()
{
  // Read the TDS sensor
  int rawValue = analogRead(TDSPIN);
  float voltage = rawValue * (5.0 / 1023.0); // Convert to voltage
  return computeTDS(voltage, DEFAULT_TEMPERATURE);
}

void setup()
{
  Serial.begin(115200);
  setup_wifi(ssid, password);
  client.setServer(mqtt_server, 1883);
  setupTime();
  Serial.println("Setup completed successfully");
}

void loop()
{
  if (!client.connected())
  {
    reconnect_Mqtt();
  }
  client.loop();

  // Read TDS
  float t = tds();
  uint64_t timestamp = unixMilis() + 7200;
  Serial.print("TDS: ");
  Serial.print(t);
  Serial.print("\t Timestamp: ");
  Serial.println(timestamp);

  // Publish TDS data
  JsonDocument jsonDocTDS;
  jsonDocTDS["schema"] = JsonObject();
  jsonDocTDS["schema"]["type"] = "struct";
  jsonDocTDS["schema"]["fields"] = JsonArray();
  JsonObject tdsField = jsonDocTDS["schema"]["fields"].createNestedObject();
  tdsField["type"] = "float";
  tdsField["optional"] = false;
  tdsField["field"] = "tds";
  JsonObject timeFieldTDS = jsonDocTDS["schema"]["fields"].createNestedObject();
  timeFieldTDS["type"] = "int64";
  timeFieldTDS["optional"] = false;
  timeFieldTDS["field"] = "timestamp";
  jsonDocTDS["payload"] = JsonObject();
  jsonDocTDS["payload"]["tds"] = t;
  jsonDocTDS["payload"]["timestamp"] = timestamp + 7200000; // To correct the GMT+2

  char jsonBufferTDS[400];
  serializeJson(jsonDocTDS, jsonBufferTDS);
  client.publish("{\"tds\":\"string\"}", jsonBufferTDS);

  delay(2000); // Wait for 2 seconds before sending the next message
}
