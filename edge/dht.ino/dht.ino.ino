#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <time.h>
#include <stdint.h>
#include "credentials.h"
#include "conf.h"

#define DHTPIN 21     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22 // DHT 22 (AM2302)

DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

static uint64_t T0NTP;
static uint64_t T0Milis;

// NTP config
const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;     // Adjust for your timezone (e.g., -3600 for GMT-1)
const int daylightOffset_sec = 3600; // Adjust for daylight saving time

void setup_wifi()
{
  delay(10);
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client"))
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      JsonDocument jsonDocTest;
      jsonDocTest["test"] = "hello world";
      char jsonBufferTest[200];
      serializeJson(jsonDocTest, jsonBufferTest);
      client.publish("{\"test\": \"string\"}", jsonBufferTest);
      // ... and resubscribe
      client.subscribe("inTopic");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

uint64_t parseUint64(String s)
{
  uint64_t result = 0;
  for (int i = 0; i < s.length(); i++)
  {
    char c = s.charAt(i);
    if (c >= '0' && c <= '9')
    {
      result = result * 10 + (c - '0');
    } else {
      break;
    }
  }
  return result;
}

uint64_t trueMilis()
{
  uint64_t trueMillis = T0NTP + millis() - T0Milis;
  return trueMillis;
}

void setupTime()
{
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("Synchronizing time with NTP server...");
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    Serial.println("Please enter the current UNIX timestamp:");
    while (!Serial.available())
    {
      delay(100);
    }
    String input = Serial.readStringUntil('\n');
    T0NTP = parseUint64(input) * 1000; // Convert to milliseconds

    time_t manualTime = input.toInt();
    localtime_r(&manualTime, &timeinfo);
    Serial.print("Manual time set to: ");
    Serial.println(&timeinfo, "%Y-%m-%d %H:%M:%S");
  }
  else
  {
    T0NTP = mktime(&timeinfo) * 1000;
    Serial.println("Time synchronized successfully");
    Serial.print("Current time: ");
    Serial.println(&timeinfo, "%Y-%m-%d %H:%M:%S");
  }
  T0Milis = millis();
  Serial.print("T0NTP: ");
  Serial.println(T0NTP);
  Serial.print("T0Millis: ");
  Serial.println(T0Milis);
}

void setup()
{
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  setupTime();
  dht.begin();
  Serial.println("Setup completed successfully");
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  // Read temperature and humidity
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  uint64_t timestamp = trueMilis() + 7200;
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
  jsonDocTemp["payload"]["timestamp"] = timestamp + 7200000; //To correct the GMT+2

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
  jsonDocHumi["payload"]["timestamp"] = timestamp + 7200000; //To correct the GMT+2

  char jsonBufferHumi[400];
  serializeJson(jsonDocHumi, jsonBufferHumi);
  client.publish("{\"humidity\":\"string\"}", jsonBufferHumi);

  delay(2000); // Wait for 2 seconds before sending the next message
}
