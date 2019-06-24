#! /usr/bin/env Python3.4
# -*- coding: UTF-8 -*-

import paho.mqtt.client as mqtt
import json
import base64

APPEUI = "70B3D57ED001A498"
APPID  = "estacao_metereologica"
PSW    = 'ttn-account-v2.9kevgHg06WAbJDOQQmb43tQImE9pKcDWBbyX5maoKP4'

#Call back functions 

# gives connection message
def on_connect(mqttc, mosq, obj,rc):
    print("Connected with result code:"+str(rc))
    # subscribe for all devices of user
    mqttc.subscribe('+/devices/#')

# gives message from device
def on_message(mqttc,obj,msg):
    x = json.loads(msg.payload)
    device = x["dev_id"]
    payload_raw = x["payload_raw"]
    payload_plain = base64.b64decode(payload_raw)
    datetime = x["metadata"]["time"]
    #rssi = x["metadata"]["gateways"]["rssi"]  # <== this raises an error (why?)
    rssi = -1
    print(device + ": " + payload_raw + " ==> " + payload_plain + ", RSSI ["+ str(rssi) + "] @" + datetime )

def on_publish(mosq, obj, mid):
    print("mid: " + str(mid))

def on_subscribe(mosq, obj, mid, granted_qos):
    print("Subscribed: " + str(mid) + " " + str(granted_qos))

def on_log(mqttc,obj,level,buf):
    print("message:" + str(buf))
    print("userdata:" + str(obj))
    
mqttc= mqtt.Client()
# Assign event callbacks
mqttc.on_connect=on_connect
mqttc.on_message=on_message

mqttc.username_pw_set(APPID, PSW)
mqttc.connect("eu.thethings.network",1883,60)

# and listen to server
run = True
while run:
    mqttc.loop()

