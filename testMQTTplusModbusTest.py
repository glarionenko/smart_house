#!/usr/bin/env python3
import paho.mqtt.client as paho
import minimalmodbus
from time import *

#mqtt auth
broker="127.0.0.1"
port=1883

#modbus start
minimalmodbus.baudrate = 4800
instr = minimalmodbus.Instrument('/dev/ttyUSB0', 1)
instr.serial.baudrate=4800
#instr.serial.bytesize = 8 # Data bits.
#instr.serial.stopbits = 1 # Stop bits.
#instr.serial.parity = minimalmodbus.serial.PARITY_NONE
#instr.serial.bytesize = 8 # Data bits.
#instr.serial.stopbits = 1# Stop bits.
#instr.serial.parity = serial.PARITY_NONE
instr.debug=False
print(instr)
sleep(2)
#
number_of_triggers=13
topic_to_id={"home/contact_1/set":0,
             "home/contact_2/set":1,
             "home/lamp_stairs/set":2,
             "home/kitchen_1/set":3,
             "home/kitchen_2/set":4,
             "home/hall_main/set":5,
             "home/tambur_light/set":6,
             "home/outside_light_down/set":7,
             "home/outside_light_up/set":8,
             "home/room_1_flor_1/set":9,
             "home/bathroom_light_flor_1/set":10,
             "home/towel_bathroom_flor_1/set":11,
             "home/boiling_light/set":12,
             }     
id_to_topic = {v: k for k, v in topic_to_id.items()}
topic_to_id_states={"home/contact_1/state":0,
                    "home/contact_2/state":1,
                    "home/lamp_stairs/state":2,
                    "home/kitchen_1/state":3,
                    "home/kitchen_2/state":4,
                    "home/hall_main/state":5,
                    "home/tambur_light/state":6,
                    "home/outside_light_down/state":7,
                    "home/outside_light_up/state":8,
                    "home/room_1_flor_1/state":9,
                    "home/bathroom_light_flor_1/state":10,
                    "home/towel_bathroom_flor_1/state":11,
                    "home/boiling_light/state":12,
                    }
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
        change(topic_to_id[message.topic],0)
    elif(str(message.payload.decode("utf-8"))=="1"):
        print("one")
        change(topic_to_id[message.topic],1)
        changed[topic_to_id[message.topic]]=1
    #client.publish(id_to_topic_states[topic_to_id[message.topic]],str(changed[topic_to_id[message.topic]]))
    
client1= paho.Client("openhabian12")                           #create client object
client1.username_pw_set("house","55566678")
client1.on_publish = on_publish                          #assign function to callback
client1.on_message=on_message
client1.connect(broker,port)
client1.loop_start()
for x in range(0,number_of_triggers):
    client1.subscribe(id_to_topic[x])
    
#дописать при получении сигнала обработку входящего топика и запись в соответсвующий топику элемент массива

read_tr=[3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3]
last_read_tr=[3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3]
last_cb=0
test=1
def change(_register,_value):
    try:
        instr.write_register(_register,_value,0)
        done = 1
        sleep(10)
    except KeyboardInterrupt:
        sys.exit()
    except Exception as e:
        value=0
        print(e)
        #continue
while True:
    sleep(1)
    #cb=instr.read_register(1,0)
    print("Started")
    for tr in range(0+number_of_triggers,number_of_triggers+number_of_triggers):
        sleep(1)
        try:
            print(tr)
            bb=instr.read_register(tr,0)
            client1.publish(id_to_topic_states[tr-number_of_triggers],bb)
        except Exception as e:
            print(e)

    
    
        
