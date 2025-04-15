from confluent_kafka import Consumer, TopicPartition
from confluent_kafka.avro import AvroConsumer
import matplotlib.pyplot as plt
from matplotlib import style
import socket
import matplotlib
import time
import json
from datetime import datetime


topics = [
    TopicPartition("test", 0, 0),
    TopicPartition("temperature", 0, 0),
    TopicPartition("humidity", 0, 0),
]
conf = {
    "bootstrap.servers": "localhost:9092",
    "client.id": socket.gethostname(),
    "group.id": "test-consumer-group",
    # "schema.registry.url": "http://localhost:8081",
}

graphing = "s"
while graphing not in ["y", "", "n"]:
    graphing = input("Would you like to graph the data? (y/n): ").lower()
if graphing != "y":
    graphing = False
else:
    graphing = True


def freq(T):
    S = 0
    for i in range(1, len(T)):
        S += T[i] - T[i - 1]
    if S == 0:
        return 0
    return (len(T) - 1) / S


matplotlib.use("TkAgg")

style.use("fivethirtyeight")

fig = plt.figure()
ax1 = fig.add_subplot(1, 1, 1)

xTemp = []
Temp = []
xHumi = []
Humi = []


# consumer = AvroConsumer(conf)
consumer = Consumer(conf)
# consumer.assign(topics)
consumer.subscribe(["test", "temperature", "humidity"])

if graphing:
    plt.ion()
    plt.show()

n = 100  # Number of last received values to display
nm = 250  # Number of last received values to calculate frequency
na = 2  # Number of last recived values to display at once
NA = 0


def update_plot():
    global NA
    if NA == 0:
        ax1.clear()
        ax1.plot(xTemp, Temp, "r", label="Temperature")
        ax1.plot(xHumi, Humi, "b", label="Humidity")
        plt.draw()
        plt.legend()
        plt.pause(0.000000000001)
        NA += 1
        NA %= na
    else:
        NA += 1
        NA %= na


def process_message(topic, value, timestamp):
    timestamp = msg.timestamp()[1] / 1000.0 - T0
    if topic == "temperature":
        xTemp.append(timestamp)
        Temp.append(float(value))
    elif topic == "humidity":
        xHumi.append(timestamp)
        Humi.append(float(value))
    # Keep only the last n elements
    if len(xTemp) > n:
        xTemp.pop(0)
    if len(Temp) > n:
        Temp.pop(0)
    if len(xHumi) > n:
        xHumi.pop(0)
    if len(Humi) > n:
        Humi.pop(0)
    update_plot()


T = [time.time()]
T0 = time.time()

try:
    while True:
        msg = consumer.poll(1.0)
        if msg is None:
            print("Waiting...")
        elif msg.error():
            print("ERROR: %s".format(msg.error()))
        else:
            T.append(time.time())
            if len(T) > nm:
                T.pop(0)
            value = msg.value()
            topic = msg.topic()
            timestamp = datetime.fromtimestamp(msg.timestamp()[1] / 1000.0).strftime(
                "%Y-%m-%d %H:%M:%S.%f"
            )[:-3]
            print(f"Received message: {value} at {timestamp:22}.", end="")
            print(
                f" Time since last message: {T[-1] - T[-2]:23}. Average frequency ({nm} messages): {freq(T):20}",
                end="",
            )
            print("")
            if graphing:
                process_message(topic, value, timestamp)
except KeyboardInterrupt:
    pass
finally:
    consumer.close()
