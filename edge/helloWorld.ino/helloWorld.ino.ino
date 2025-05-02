#include <WiFi.h>
#include <PubSubClient.h>
#include "credentials.h"
#include "conf.h"
#include "edge.h"

WiFiClient espClient;
PubSubClient client(espClient);

void setup()
{
  Serial.begin(115200);
  setup_wifi(ssid, password);
  client.setServer(mqtt_server, 1883);
  Serial.println("Setup completed successfully");
}

void loop()
{
  if (!client.connected())
  {
    reconnect_Mqtt();
  }
  client.loop();
  // Publish a message
  client.publish("test", "hello world");
  delay(2000); // Wait for 2 seconds before sending the next message
}
