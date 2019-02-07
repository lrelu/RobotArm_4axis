# -*- coding: utf-8 -*-
import paho.mqtt.client as mqtt
import time

class mqtt_handler:
    client = mqtt.Client()

    def __init__(self):
        return

    @classmethod
    def subscribe(cls):
        # publish할 주소와 포트번호 연결
        cls.client.connect("broker.mqttdashboard.com", port=1883, keepalive=60)
        cls.client.loop_start()

    @classmethod
    def publish(cls, message):
        cls.client.publish("robotline/ra1", message)

    @classmethod
    def cancel(cls):
        cls.client.loop_stop()
        cls.client.disconnect()


