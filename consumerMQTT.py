import paho.mqtt.client as mqtt


# Callback when connection to broker is established
def on_connect(client, userdata, flags, rc, properties):
    if rc == 0:
        print("Connected to MQTT Broker")
    else:
        print("Failed to connect, return code %d\n", rc)
    client.subscribe("#")


# Callback when a message is received
def on_message(client, userdata, msg):
    print(f"Received message on topic {msg.topic} : \n{msg.payload.decode()} \n")


client_id = f"consumer"

# Create a client instance
client = mqtt.Client(
    protocol=mqtt.MQTTv311,
    callback_api_version=mqtt.CallbackAPIVersion.VERSION2,
    client_id=client_id,
)

# Attach callbacks
client.on_connect = on_connect
client.on_message = on_message

print("setup done")

# Connect to a broker
client.connect("localhost", 1883)

# Blocking loop to process network traffic and dispatch callbacks
client.loop_forever()
