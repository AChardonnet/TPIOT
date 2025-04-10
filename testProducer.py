from confluent_kafka import Producer
from random import randint
from time import time, sleep
import socket

LastMessage = 0

topic = "test"
conf = {
    "bootstrap.servers": "192.168.137.1:9093",
    "client.id": socket.gethostname(),
}
producer = Producer(conf)


def waitUntilNextMessage(interval):
    while time() - LastMessage < interval:
        print(time() - LastMessage)
        sleep(0.001)


def produceMessage(topic, key, value):
    global LastMessage
    LastMessage = time()
    producer.produce(topic, key=key, value=value, callback=deliveryCallback)
    producer.poll(1)


def deliveryCallback(err, msg):
    if err:
        print(f"ERROR: Message failed delivery: {err}")
    else:
        topic = msg.topic()
        key = msg.key().decode("utf-8")
        value = msg.value().decode("utf-8")
        print(f"Produced event to topic {topic}: key = {key:12} value = {value:12}")


def produceRandomMessages(min=0, max=100, interval=10):
    while True:
        produceMessage(topic, "key", str(randint(min, max)))
        waitUntilNextMessage(interval)


def produceSlopeMesssages(max=25, interval=10):
    i = 0
    while True:
        produceMessage(topic, "key", str(i))
        waitUntilNextMessage(interval)
        i += 1
        i %= max


def testProducer(interval=0.001, random=""):
    while True:
        while type(interval) != float:
            try:
                interval_input = input("Enter the interval between messages: ")
                if "/" in interval_input:
                    numerator, denominator = map(float, interval_input.split("/"))
                    interval = numerator / denominator
                else:
                    interval = float(interval_input)
            except ValueError:
                print("Please enter a valid number or fraction.")
        while random not in ["y", "", "n"]:
            random = input("Would you like to produce random messages? (y): ").lower()
        if random == "y":
            produceRandomMessages(interval=interval)
        else:
            produceSlopeMesssages(interval=interval)


if __name__ == "__main__":
    testProducer()
