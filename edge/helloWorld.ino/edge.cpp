#include <WiFi.h>
#include <PubSubClient.h>
#include "edge.h"

extern PubSubClient client;

void setup_wifi(const char *ssid, const char *password)
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

void reconnect_Mqtt()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client"))
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("test", "hello world");
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

uint64_t unixMilis()
{
  uint64_t trueMillis = T0NTP + millis() - T0Milis;
  return trueMillis;
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
    }
    else
    {
      break;
    }
  }
  return result;
}