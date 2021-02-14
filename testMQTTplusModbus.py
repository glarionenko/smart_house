#!/usr/bin/env python3
import paho.mqtt.client as paho
import minimalmodbus
from time import *

#mqtt auth
broker="192.168.1.61"
port=1883

#modbus start
minimalmodbus.baudrate = 9600
instr = minimalmodbus.Instrument('/dev/ttyACM0', 1)
instr.serial.baudrate=9600
instr.debug=False
print(instr)
sleep(2)
#
number_of_triggers=21
topic_to_id={"home/triggers/kotel_to_light_kotel":0,"home/triggers/switch_floor1_to_light_street_floor1":1,"home/triggers/prachka_to_light_hall_floor1":2,"home/triggers/switch2_floor1_to_light_prachka_floor2":3,"home/triggers/switch1_kitchen_to_light_stairs_floor1":4,"home/triggers/switch2_kitchen_to_light1_kitchen":5,"home/triggers/hall_floor1_to_light2_kitchen_tv":6,"home/triggers/switch_input_to_light_hall_floor1":7,"home/triggers/street1_to_light_tambur":8,"home/triggers/switch_room3_floor2_to_light_street_floor1":9,"home/triggers/switch_room1_floor2_to_light_room3_floor2":10,"home/triggers/floor3_to_light_room1_floor2":11,"home/triggers/street2_to_light1_floor3":12,"home/triggers/wardrobe_to_light_street_floor2":13,"home/triggers/hall_floor2_to_light_wardrobe_floor2":14,"home/triggers/sw1_room2_floor2_to_light_on_stairs":15,"home/triggers/sw2_room2_floor2_to_light_room2_floor2":16,"home/triggers/badroom1_to_socket1_floor2":17,"home/triggers/badroom2_to_light_badroom_floor1":18,"home/triggers/room1_floor1_to_light_badroom_floor2":19,"home/triggers/switch2_input_to_light_room1_floor1":20}
id_to_topic = {v: k for k, v in topic_to_id.items()}
topic_to_id_states={"home/states/kotel_to_light_kotel":0,"home/states/switch_floor1_to_light_street_floor1":1,"home/states/prachka_to_light_hall_floor1":2,"home/states/switch2_floor1_to_light_prachka_floor2":3,"home/states/switch1_kitchen_to_light_stairs_floor1":4,"home/states/switch2_kitchen_to_light1_kitchen":5,"home/states/hall_floor1_to_light2_kitchen_tv":6,"home/states/switch_input_to_light_hall_floor1":7,"home/states/street1_to_light_tambur":8,"home/states/switch_room3_floor2_to_light_street_floor1":9,"home/states/switch_room1_floor2_to_light_room3_floor2":10,"home/states/floor3_to_light_room1_floor2":11,"home/states/street2_to_light1_floor3":12,"home/states/wardrobe_to_light_street_floor2":13,"home/states/hall_floor2_to_light_wardrobe_floor2":14,"home/states/sw1_room2_floor2_to_light_on_stairs":15,"home/states/sw2_room2_floor2_to_light_room2_floor2":16,"home/states/badroom1_to_socket1_floor2":17,"home/states/badroom2_to_light_badroom_floor1":18,"home/states/room1_floor1_to_light_badroom_floor2":19,"home/states/switch2_input_to_light_room1_floor1":20}
id_to_topic_states = {v: k for k, v in topic_to_id_states.items()}
changed=[3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3]
def on_publish(client,userdata,result):             #create function for callback
    print("data published \n")
    pass
def on_message(client, userdata, message):
    global changed
    print("message received " ,str(message.payload.decode("utf-8")))
    print("message topic=",message.topic)
    print("message qos=",message.qos)
    print("message retain flag=",message.retain)
    if(str(message.payload.decode("utf-8"))=="0"):
        print("zero")
        changed[topic_to_id[message.topic]]=0
    elif(str(message.payload.decode("utf-8"))=="1"):
        print("one")
        changed[topic_to_id[message.topic]]=1
    #client.publish(id_to_topic_states[topic_to_id[message.topic]],str(changed[topic_to_id[message.topic]]))
    
client1= paho.Client("openhabian1")                           #create client object
client1.username_pw_set("house","55566678")
client1.on_publish = on_publish                          #assign function to callback
client1.on_message=on_message
client1.connect(broker,port)
client1.loop_start()
for x in range(0,number_of_triggers):
    client1.subscribe(id_to_topic[x])
#дописать при получении сигнала обработку входящего топика и запись в соответсвующий топику элемент массива
#
read_tr=[3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3]
last_read_tr=[3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3]
last_cb=0
while True:
    sleep(0.3)
    cb=instr.read_register(7,0)
    #print(cb)
    for tr in range(0,number_of_triggers):
        if(changed[tr]!=2):
            if(changed[tr]==1):
                instr.write_register(tr,1,0)
                changed[tr]=2
            if(changed[tr]==0):
                instr.write_register(tr,0,0)
                changed[tr]=2
        if(changed[tr]==3):
            changed[tr]=2
            client1.publish(id_to_topic[tr],"begin")
            client1.publish(id_to_topic_states[tr],"begin")
        read_tr[tr]=instr.read_register(tr+21,0)
        if(read_tr[tr]!=last_read_tr[tr]):
            client1.publish(id_to_topic_states[tr],read_tr[tr])
            last_read_tr[tr]=read_tr[tr]
        
