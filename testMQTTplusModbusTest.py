#!/usr/bin/env python3
import paho.mqtt.client as paho
import minimalmodbus
from time import *
#
import serial
import serial.tools.list_ports

ports = serial.tools.list_ports.comports() #automatic searching of ports
portArd = '0042'#'7523'#0042

for port1, desc, hwid in sorted(ports):
    print(hwid)
    if (hwid.find(portArd) != -1):
        print ("Arduino found")
        print (port1)
        arduino_port = port1
#

#mqtt auth
broker="127.0.0.1"
port=1883

#modbus start
minimalmodbus.baudrate = 4800
instr = minimalmodbus.Instrument(arduino_port, 1)
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
number_of_triggers=22
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
             "home/bathroom_light_toilet_floor_2/set":13,
             "home/bathroom_light_shower_floor_2/set":14,
             "home/towel_bathroom_floor_2/set":15,
             "home/bedroom_light_floor_2/set":16,
             "home/bedroom_wardrobe_light_floor_2/set":17,
             "home/bedroom_socket_floor_2/set":18,
             "home/room_medium_light_floor_2/set":19,
             "home/room_small_light_floor_2/set":20,
             "home/hall_light_floor_2/set":21,
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
                    "home/bathroom_light_toilet_floor_2/state":13,
                    "home/bathroom_light_shower_floor_2/state":14,
                    "home/towel_bathroom_floor_2/state":15,
                    "home/bedroom_light_floor_2/state":16,
                    "home/bedroom_wardrobe_light_floor_2/state":17,
                    "home/bedroom_socket_floor_2/state":18,
                    "home/room_medium_light_floor_2/state":19,
                    "home/room_small_light_floor_2/state":20,
                    "home/hall_light_floor_2/state":21,
                    }
id_to_topic_states = {v: k for k, v in topic_to_id_states.items()}

changed=[3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3]
def on_disconnect(client, userdata, rc):
    print("disonnected")
    client1.connect_async()
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
client1.on_disconnect=on_disconnect
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
countError=0
while True:
    sleep(1)
    #cb=instr.read_register(1,0)
    print("Started")
    for tr in range(0+number_of_triggers,number_of_triggers+number_of_triggers):
        sleep(1)
        try:
            print(tr)
            bb=instr.read_register(tr,0)
            client1.publish(id_to_topic_states[tr-number_of_triggers],bb,qos=1)
            countError=0
        except Exception as e:
            print(e)
            print(countError)
            countError=countError+1
            if countError>15:
                raise ValueError('A very specific bad thing happened.')


    
    
        
