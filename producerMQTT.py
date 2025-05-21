import time

import paho.mqtt.client as mqtt

# MQTT broker details
broker = "localhost"
port = 1883
topic = "test"

# Create an MQTT client instance
client = mqtt.Client(
    protocol=mqtt.MQTTv311,
    callback_api_version=mqtt.CallbackAPIVersion.VERSION2,
    client_id="producer",
)

# Connect to the MQTT broker
client.connect(broker, port, 60)

# Publish a message
message = "Hello, World!"
result = client.publish(topic, message)
status = result[0]
if status == 0:
    print(f"Send `{message}` to topic `{topic}`")
else:
    print(f"Failed to send message to topic {topic}")

# Disconnect from the broker
client.disconnect()
