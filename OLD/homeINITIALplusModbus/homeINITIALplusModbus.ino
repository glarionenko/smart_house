#include <EEPROM.h>
#include <SimpleModbusSlave.h>

#define switch_kotel 34
#define switch_badroom1 36
#define switch_badroom2 38
#define switch_prachka 40

#define switch_room1_floor1 42
#define switch1_floor1 44
#define switch2_floor1 46
#define switch1_kitchen 48
#define switch2_kitchen 50

#define switch_hall_floor1 52
#define switch_input A12
#define switch_street_floor1 A1
#define switch_room3_floor2 A2
#define switch_room1_floor2 A3

#define switch_floor3 A4
#define switch_street_floor2 A5
#define switch_wardrobe_floor2 A6
#define switch_hall_floor2 A7
#define switch1_room2_floor2 A8

#define switch2_room2_floor2 A9
#define switch1_bed A10
#define switch2_bed A11

#define switch2_input A0
////------------
//
#define kontakt1 25

#define kontakt2 23
#define socket1_floor2 2
#define light_stairs_floor1 3
#define light_room1_floor1 4
#define light_kotel 5

#define light_tambur 6
#define light1_kitchen 7
#define light_hall_floor1 8
#define light_badroom_floor2 9
#define fan_badroom_floor2 10

#define light2_kitchen_tv 11
#define light_badroom_floor1 12
#define light1_floor3 49
#define towel_badroom_floor1 51//полотенчик
#define towel_badroom_floor2 53

#define light_room2_floor2 47
#define fan_badroom_floor1 45
#define light_street_floor2 41
#define light_on_stairs 39
#define light_street_floor1 37

#define light_prachka_floor2 35
#define light_room1_floor2 33
#define light_wardrobe_floor2 31
#define light_room3_floor2 29
#define light2_floor3 27

#define pir A14
#define code A15


int flag_kotel = 0;
int flag_switch_floor1 = 0;
int flag_prachka = 0;
int flag_switch2_floor1 = 0;
int flag_switch1_kitchen = 0;
int flag_switch2_kitchen = 0;
int flag_hall_floor1 = 0;
int flag_switch_input = 0;
int flag_street1 = 0;
int flag_switch_room3_floor2 = 0;
int flag_switch_room1_floor2 = 0;
int flag_floor3 = 0;
int flag_street2 = 0;
int flag_wardrobe = 0;
int flag_hall_floor2 = 0;
int flag_sw1_room2_floor2 = 0;
int flag_sw2_room2_floor2 = 0;
int flag_badroom1 = 0;
int flag_badroom2 = 0;
int flag_room1_floor1 = 0;
int flag_switch2_input = 0;

int flag_input_code = 0;

int sleep_mode = 1;
int flag_error = 0;

long int timer_fan1 = 0;
long int timer_fan2 = 0;

long int const_timer_fan1 = 30000;
long int const_timer_fan2 = 30000;

int incomingByte = 0;
int Step = 0;

int break_flag = 0;

int flag_fan1 = 0;
int flag_fan2 = 0;

long int timers_button = 0;

//CONFIGURABLE
#define MODBUS_ID 1

enum
{
  CONTACT_1_SET,
  CONTACT_2_SET,
  LAMP_STAIRs_SET,
  LIGHT1_KITCHEN_SET,
  CONTACT_1_STATE,
  CONTACT_2_STATE,
  LAMP_STAIRS_STATE,
  LIGHT1_KITCHEN_STAITE,
  HOLDING_REGS_SIZE // leave this one
  // total number of registers for function 3 and 16 share the same register array
  // i.e. the same address space
};
unsigned int holdingRegs[HOLDING_REGS_SIZE];

void setup() {
  modbus_configure(&Serial, 4800, SERIAL_8N2, MODBUS_ID, 0, HOLDING_REGS_SIZE, holdingRegs);
  modbus_update_comms(4800, SERIAL_8N2, MODBUS_ID);
  for (int i = 34; i < 53; i += 2) {
    pinMode(i, INPUT_PULLUP);
  }
  for (int i = 23; i < 54; i += 2) {
    pinMode(i, OUTPUT);
    digitalWrite(i, 0);
  }
  for (int i = 2; i < 14; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, 0);
  }
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);
  pinMode(A5, INPUT_PULLUP);
  pinMode(A6, INPUT_PULLUP);
  pinMode(A7, INPUT_PULLUP);
  pinMode(A8, INPUT_PULLUP);
  pinMode(A9, INPUT_PULLUP);
  pinMode(A10, INPUT_PULLUP);
  pinMode(A11, INPUT_PULLUP);
  pinMode(A12, INPUT_PULLUP);
  pinMode(A13, INPUT_PULLUP);
  pinMode(A14, INPUT_PULLUP);
  pinMode(A15, INPUT_PULLUP);

  digitalWrite(kontakt2, 1);
  holdingRegs[CONTACT_2_STATE] = 1;
  holdingRegs[CONTACT_2_SET] = 1;
 // Serial.begin(9600);
}

void sleep() {

  digitalWrite(kontakt1, 0);
  digitalWrite(kontakt2, 1);

  holdingRegs[CONTACT_1_STATE] = 0;
  holdingRegs[CONTACT_2_STATE] = 1;

  holdingRegs[CONTACT_1_SET] = 0;
  holdingRegs[CONTACT_2_SET] = 1;

  digitalWrite(socket1_floor2, 0);
  digitalWrite(light_stairs_floor1, 0);
  holdingRegs[LAMP_STAIRs_SET] = 0;
  holdingRegs[LAMP_STAIRS_STATE] = 0;

  digitalWrite(light_room1_floor1, 0);
  digitalWrite(light_room1_floor1, 0);
  digitalWrite(light_kotel, 0);

  digitalWrite(light_tambur, 0);
  digitalWrite(light1_kitchen, 0);
  holdingRegs[LIGHT1_KITCHEN_SET] = digitalRead(light1_kitchen);
  holdingRegs[LIGHT1_KITCHEN_STAITE] = digitalRead(light1_kitchen);
  digitalWrite(light_hall_floor1, 0);
  digitalWrite(light_badroom_floor2, 0);
  digitalWrite(fan_badroom_floor2, 0);

  digitalWrite(light2_kitchen_tv, 0);
  digitalWrite(light_badroom_floor1, 0);
  digitalWrite(light1_floor3, 0);
  digitalWrite(towel_badroom_floor1, 0);
  digitalWrite(towel_badroom_floor2, 0);

  digitalWrite(light_room2_floor2, 0);
  digitalWrite(fan_badroom_floor1, 0);
  digitalWrite(light_street_floor2, 0);
  digitalWrite(light_on_stairs, 0);
  digitalWrite(light_street_floor1, 0);
  holdingRegs[LAMP_STAIRS_STATE] = digitalRead(light_street_floor1);
  holdingRegs[LAMP_STAIRs_SET] = digitalRead(light_street_floor1);


  digitalWrite(light_prachka_floor2, 0);
  digitalWrite(light_room1_floor2, 0);
  digitalWrite(light_wardrobe_floor2, 0);
  digitalWrite(light_room3_floor2, 0);
  digitalWrite(light2_floor3, 0);

  flag_fan1 = 0;
  flag_fan2 = 0;
  sleep_mode = 1;
}

void wake_up() {
  digitalWrite(kontakt1, 1);
  digitalWrite(kontakt2, 1);

  holdingRegs[CONTACT_1_STATE] = 1;
  holdingRegs[CONTACT_2_STATE] = 1;

  holdingRegs[CONTACT_1_SET] = 1;
  holdingRegs[CONTACT_2_SET] = 1;

  sleep_mode = 0;
}



void loop() {

  if(digitalRead(kontakt1) != holdingRegs[CONTACT_1_SET]) {
    digitalWrite(kontakt1, holdingRegs[CONTACT_1_SET]);
    holdingRegs[CONTACT_1_STATE] = digitalRead(kontakt1);
    holdingRegs[CONTACT_1_SET] = digitalRead(kontakt1);
  }
//-----
if(digitalRead(kontakt2) != holdingRegs[CONTACT_2_SET]) {
  digitalWrite(kontakt2, holdingRegs[CONTACT_2_SET]);
  holdingRegs[CONTACT_2_STATE] = digitalRead(kontakt2);
  holdingRegs[CONTACT_2_SET] = digitalRead(kontakt2);
}
//-----
if(digitalRead(light_stairs_floor1) != holdingRegs[LAMP_STAIRs_SET]) {
  digitalWrite(light_stairs_floor1, holdingRegs[LAMP_STAIRs_SET]);
  holdingRegs[LAMP_STAIRS_STATE] = digitalRead(light_stairs_floor1);
  holdingRegs[LAMP_STAIRs_SET] = digitalRead(light_stairs_floor1);
}
//-----
if(digitalRead(light1_kitchen) != holdingRegs[LIGHT1_KITCHEN_SET]) {
  digitalWrite(light1_kitchen, holdingRegs[LIGHT1_KITCHEN_SET]);
  holdingRegs[LIGHT1_KITCHEN_SET] = digitalRead(light1_kitchen);
  holdingRegs[LIGHT1_KITCHEN_STAITE] = digitalRead(light1_kitchen);
}
//-----

modbus_update();

if (sleep_mode == 1) {


  if (!digitalRead(code) && flag_input_code == 0) {
    delay(30);
    if (!digitalRead(code)) {
      delay(30);
      if (!digitalRead(code)) {
        delay(30);
        if (!digitalRead(code)) {
          flag_input_code = 1;
          wake_up();
          delay(300);
          digitalWrite(light_tambur, 1);
          delay(500);
          digitalWrite(light_hall_floor1, 1);

            //holdingRegs[LAMP_STAIRS_STATE] = 1;
            //holdingRegs[LAMP_STAIRs_SET] = digitalRead(light_stairs_floor1);
            
          }
        }
      }
    }
  }

  if (flag_input_code == 1) {
    if (digitalRead(code)) {
      delay(30);
      if (digitalRead(code)) {
        delay(30);
        if (digitalRead(code)) flag_input_code = 0;
      }
    }
  }

  if (sleep_mode == 0) {


    if (!digitalRead(code) && flag_input_code == 0) {
      delay(100);
      if (!digitalRead(code)) {
        delay(100);
        if (!digitalRead(code)) {
          delay(100);
          if (!digitalRead(code)) {
            flag_input_code = 1;
            if (!digitalRead(light_tambur)) {
              digitalWrite(light_tambur, 1);
              delay(500);
            }
            digitalWrite(light_tambur, 0);
            delay(1000);
            digitalWrite(light_tambur, 1);

            digitalWrite(light_street_floor1, 1);
            holdingRegs[LAMP_STAIRS_STATE] = digitalRead(light_street_floor1);
            holdingRegs[LAMP_STAIRs_SET] = digitalRead(light_street_floor1); 
            delay(3000);
            digitalWrite(light_street_floor2, 1);

            digitalWrite(socket1_floor2, 0);
            digitalWrite(light_stairs_floor1, 0);

            holdingRegs[LAMP_STAIRS_STATE] = 0;
            holdingRegs[LAMP_STAIRs_SET] = digitalRead(light_stairs_floor1);

            digitalWrite(light_room1_floor1, 0);
            digitalWrite(light_room1_floor1, 0);
            digitalWrite(light_kotel, 0);

          //digitalWrite(light_tambur, 0);
          digitalWrite(light1_kitchen, 0);
          holdingRegs[LIGHT1_KITCHEN_SET] = digitalRead(light1_kitchen);
          holdingRegs[LIGHT1_KITCHEN_STAITE] = digitalRead(light1_kitchen);
          digitalWrite(light_hall_floor1, 0);
          digitalWrite(light_badroom_floor2, 0);
          digitalWrite(fan_badroom_floor2, 0);

          digitalWrite(light2_kitchen_tv, 0);
          digitalWrite(light_badroom_floor1, 0);
          digitalWrite(light1_floor3, 0);
          digitalWrite(towel_badroom_floor1, 0);
          digitalWrite(towel_badroom_floor2, 0);

          digitalWrite(light_room2_floor2, 0);
          digitalWrite(fan_badroom_floor1, 0);
          digitalWrite(light_street_floor2, 0);
          digitalWrite(light_on_stairs, 0);
          //digitalWrite(light_street_floor1, 0);

          //digitalWrite(light_prachka_floor2, 0);
          digitalWrite(light_room1_floor2, 0);
          digitalWrite(light_wardrobe_floor2, 0);
          digitalWrite(light_room3_floor2, 0);
          digitalWrite(light2_floor3, 0);
          flag_error = 0;
          for(long int i = 0; i < 60000; i++) {
            delay(1);
            if(flag_input_code == 1 && digitalRead(code)) {
              delay(200);
              if(digitalRead(code)) flag_input_code = 0;
            }

            if(i == 10000) digitalWrite(light_tambur, 0);

            if(!digitalRead(code) && flag_input_code == 0) {
              delay(300);
              if(!digitalRead(code)) {
                wake_up();
                flag_error = 1;
                i = 63000;
                flag_input_code = 1;
              }
            }
            
          }


          if(flag_error == 0) {
            sleep();
          }

        }
      }
    }
  }

  if (!digitalRead(switch_kotel) && flag_kotel == 0) {
    delay(40);
    if (!digitalRead(switch_kotel)) {
      digitalWrite(light_kotel, !digitalRead(light_kotel));
      flag_kotel = 1;
    }
  }

  if (flag_kotel) {
    if (digitalRead(switch_kotel)) {
      delay(50);
      if (digitalRead(switch_kotel)) flag_kotel = 0;
    }
  }


  if (!digitalRead(switch2_input) && flag_switch2_input == 0) {
    delay(40);
    if (!digitalRead(switch2_input)) {
      digitalWrite(light_street_floor1, !digitalRead(light_street_floor1));
      holdingRegs[LAMP_STAIRS_STATE] = digitalRead(light_street_floor1);
      holdingRegs[LAMP_STAIRs_SET] = digitalRead(light_stairs_floor1);
      flag_switch2_input = 1;
    }
  }

  if (flag_switch2_input) {
    if (digitalRead(switch2_input)) {
      delay(50);
      if (digitalRead(switch2_input)) flag_switch2_input = 0;
    }
  }
    //--------------

    if (!digitalRead(switch1_floor1) && flag_switch_floor1 == 0) {
      delay(40);
      if (!digitalRead(switch1_floor1)) {
        digitalWrite(light_hall_floor1, !digitalRead(light_hall_floor1));
        flag_switch_floor1 = 1;
      }
    }

    if (flag_switch_floor1) {
      if (digitalRead(switch1_floor1)) {
        delay(50);
        if (digitalRead(switch1_floor1))  flag_switch_floor1 = 0;
      }
    }
    //--------------


    if (!digitalRead(switch_prachka) && flag_prachka == 0) {
      delay(40);
      if (!digitalRead(switch_prachka)) {
        digitalWrite(light_prachka_floor2, !digitalRead(light_prachka_floor2));
        flag_prachka = 1;
      }
    }

    if (flag_prachka) {
      if (digitalRead(switch_prachka)) {
        delay(50);
        if (digitalRead(switch_prachka)) flag_prachka = 0;
      }
    }
    //--------------



    if (!digitalRead(switch2_floor1) && flag_switch2_floor1 == 0) {
      delay(40);
      if (!digitalRead(switch2_floor1)) {
        digitalWrite(light_stairs_floor1, !digitalRead(light_stairs_floor1));
        flag_switch2_floor1 = 1;
        holdingRegs[LAMP_STAIRS_STATE] = digitalRead(light_stairs_floor1);
        holdingRegs[LAMP_STAIRs_SET] = digitalRead(light_stairs_floor1);
      }
    }

    if (flag_switch2_floor1) {
      if (digitalRead(switch2_floor1)) {
        delay(50);
        if (digitalRead(switch2_floor1)) flag_switch2_floor1 = 0;
      }
    }
    //--------------


    if (!digitalRead(switch1_kitchen) && flag_switch1_kitchen == 0) {
      delay(40);
      if (!digitalRead(switch1_kitchen)) {
        digitalWrite(light1_kitchen, !digitalRead(light1_kitchen));
        flag_switch1_kitchen = 1;
        holdingRegs[LIGHT1_KITCHEN_SET] = digitalRead(light1_kitchen);
        holdingRegs[LIGHT1_KITCHEN_STAITE] = digitalRead(light1_kitchen);
      }
    }

    if (flag_switch1_kitchen) {
      if (digitalRead(switch1_kitchen)) {
        delay(50);
        if (digitalRead(switch1_kitchen)) flag_switch1_kitchen = 0;
      }
    }
    //--------------




    if (!digitalRead(switch2_kitchen) && flag_switch2_kitchen == 0) {
      delay(40);
      if (!digitalRead(switch2_kitchen)) {
        digitalWrite(light2_kitchen_tv, !digitalRead(light2_kitchen_tv));
        flag_switch2_kitchen = 1;
      }
    }

    if (flag_switch2_kitchen) {
      if (digitalRead(switch2_kitchen)) {
        delay(50);
        if (digitalRead(switch2_kitchen)) flag_switch2_kitchen = 0;
      }
    }
    //--------------




    if (!digitalRead(switch_hall_floor1) && flag_hall_floor1 == 0) {
      delay(40);
      if (!digitalRead(switch_hall_floor1)) {
        digitalWrite(light_hall_floor1, !digitalRead(light_hall_floor1));
        flag_hall_floor1 = 1;
      }
    }

    if (flag_hall_floor1) {
      if (digitalRead(switch_hall_floor1)) {
        delay(50);
        if (digitalRead(switch_hall_floor1))  flag_hall_floor1 = 0;
      }
    }
    //--------------

    if (!digitalRead(switch_input) && flag_switch_input == 0) {
      delay(10);
      if (!digitalRead(switch_input)) {
        timers_button = millis();
        flag_switch_input = 1;
      }
    }


    if (flag_switch_input) {
      if (digitalRead(switch_input)) {
        delay(30);
        if (digitalRead(switch_input))
        flag_switch_input = 0;
        digitalWrite(light_tambur, !digitalRead(light_tambur));
      }
    }

    //--------------


    if (!digitalRead(switch_street_floor1) && flag_street1 == 0) {
      delay(40);
      if (!digitalRead(switch_street_floor1)) {
        digitalWrite(light_street_floor1, !digitalRead(light_street_floor1));
        flag_street1 = 1;
      }
    }

    if (flag_street1) {
      if (digitalRead(switch_street_floor1)) {
        delay(50);
        if (digitalRead(switch_street_floor1)) flag_street1 = 0;
      }
    }

    //--------------


    if (!digitalRead(switch_room3_floor2) && flag_switch_room3_floor2 == 0) {
      delay(40);
      if (!digitalRead(switch_room3_floor2)) {
        digitalWrite(light_room3_floor2, !digitalRead(light_room3_floor2));
        flag_switch_room3_floor2 = 1;
      }
    }

    if (flag_switch_room3_floor2) {
      if (digitalRead(switch_room3_floor2)) {
        delay(50);
        if (digitalRead(switch_room3_floor2)) flag_switch_room3_floor2 = 0;
      }
    }

    //--------------


    if (!digitalRead(switch_room1_floor2) && flag_switch_room1_floor2 == 0) {
      delay(40);
      if (!digitalRead(switch_room1_floor2)) {
        digitalWrite(light_room1_floor2, !digitalRead(light_room1_floor2));
        flag_switch_room1_floor2 = 1;
      }
    }

    if (flag_switch_room1_floor2) {
      if (digitalRead(switch_room1_floor2)) {
        delay(50);
        if (digitalRead(switch_room1_floor2)) flag_switch_room1_floor2 = 0;
      }
    }
    //--------------

    if (!digitalRead(switch_floor3) && flag_floor3 == 0) {
      delay(40);
      if (!digitalRead(switch_floor3)) {
        digitalWrite(light1_floor3, !digitalRead(light1_floor3));
        flag_floor3 = 1;
      }
    }

    if (flag_floor3) {
      if (digitalRead(switch_floor3)) {
        delay(50);
        if (digitalRead(switch_floor3)) flag_floor3 = 0;
      }
    }
    //--------------

    if (!digitalRead(switch_street_floor2) && flag_street2 == 0) {
      delay(40);
      if (!digitalRead(switch_street_floor2)) {
        digitalWrite(light_street_floor2, !digitalRead(light_street_floor2));
        flag_street2 = 1;
      }
    }

    if (flag_street2) {
      if (digitalRead(switch_street_floor2)) {
        delay(50);
        if (digitalRead(switch_street_floor2)) flag_street2 = 0;
      }
    }
    //--------------

    if (!digitalRead(switch_wardrobe_floor2) && flag_wardrobe == 0) {
      delay(40);
      if (!digitalRead(switch_wardrobe_floor2)) {
        digitalWrite(light_wardrobe_floor2, !digitalRead(light_wardrobe_floor2));
        flag_wardrobe = 1;
      }
    }

    if (flag_wardrobe) {
      if (digitalRead(switch_wardrobe_floor2)) {
        delay(50);
        if (digitalRead(switch_wardrobe_floor2)) flag_wardrobe = 0;
      }
    }
    //--------------

    if (!digitalRead(switch_hall_floor2) && flag_hall_floor2 == 0) {
      delay(40);
      if (!digitalRead(switch_hall_floor2)) {
        digitalWrite(light_on_stairs, !digitalRead(light_on_stairs));
        flag_hall_floor2 = 1;
      }
    }

    if (flag_hall_floor2) {
      if (digitalRead(switch_hall_floor2)) {
        delay(50);
        if (digitalRead(switch_hall_floor2)) flag_hall_floor2 = 0;
      }
    }
    //--------------

    if (!digitalRead(switch1_room2_floor2) && flag_sw1_room2_floor2 == 0) {
      delay(40);
      if (!digitalRead(switch1_room2_floor2)) {
        digitalWrite(light_room2_floor2, !digitalRead(light_room2_floor2));
        flag_sw1_room2_floor2 = 1;
      }
    }

    if (flag_sw1_room2_floor2) {
      if (digitalRead(switch1_room2_floor2)) {
        delay(50);
        if (digitalRead(switch1_room2_floor2)) flag_sw1_room2_floor2 = 0;
      }
    }
    //--------------

    if (!digitalRead(switch2_room2_floor2) && flag_sw2_room2_floor2 == 0) {
      delay(40);
      if (!digitalRead(switch2_room2_floor2)) {
        digitalWrite(socket1_floor2, !digitalRead(socket1_floor2));
        flag_sw2_room2_floor2 = 1;
      }
    }

    if (flag_sw2_room2_floor2) {
      if (digitalRead(switch2_room2_floor2)) {
        delay(50);
        if (digitalRead(switch2_room2_floor2)) flag_sw2_room2_floor2 = 0;
      }
    }
    //--------------

    if (!digitalRead(switch_badroom1) && flag_badroom1 == 0) {
      delay(40);
      if (!digitalRead(switch_badroom1)) {
        if (digitalRead(light_badroom_floor1)) {
          digitalWrite(light_badroom_floor1, 0);
          flag_fan1 = 1;
          timer_fan1 = millis();
          digitalWrite(fan_badroom_floor1, 1);
        }

        else {
          digitalWrite(light_badroom_floor1, 1);
          flag_fan1 = 0;
          digitalWrite(fan_badroom_floor1, 1);
        }

        flag_badroom1 = 1;
      }
    }

    if (flag_badroom1) {
      if (digitalRead(switch_badroom1)) {
        delay(50);
        if (digitalRead(switch_badroom1)) flag_badroom1 = 0;
      }
    }

    if (flag_fan1) {
      if (millis() - timer_fan1 > const_timer_fan1) {
        flag_fan1 = 0;
        digitalWrite(fan_badroom_floor1, 0);
      }
    }
    //--------------


    if (!digitalRead(switch_badroom2) && flag_badroom2 == 0) {
      delay(40);
      if (!digitalRead(switch_badroom2)) {

        if (digitalRead(light_badroom_floor2)) {
          digitalWrite(light_badroom_floor2, 0);
          flag_fan2 = 1;
          timer_fan2 = millis();
          digitalWrite(fan_badroom_floor2, 1);
        }

        else {
          digitalWrite(light_badroom_floor2, 1);
          flag_fan1 = 0;
          digitalWrite(fan_badroom_floor2, 1);
        }


        flag_badroom2 = 1;
      }
    }



    if (flag_badroom1) {
      if (digitalRead(switch_badroom2)) {
        delay(50);
        if (digitalRead(switch_badroom2)) flag_badroom2 = 0;
      }
    }

    if (flag_fan2) {
      if (millis() - timer_fan2 > const_timer_fan2) {
        flag_fan2 = 0;
        digitalWrite(fan_badroom_floor2, 0);
      }
    }
    //--------------


    //--------------


    if (!digitalRead(switch_badroom2) && flag_badroom2 == 0) {
      delay(40);
      if (!digitalRead(switch_badroom2)) {
        digitalWrite(light_badroom_floor2, !digitalRead(light_badroom_floor2));
        digitalWrite(fan_badroom_floor2, digitalRead(light_badroom_floor2));
        flag_badroom2 = 1;
      }
    }

    if (flag_badroom2) {
      if (digitalRead(switch_badroom2)) {
        delay(50);
        if (digitalRead(switch_badroom2)) flag_badroom2 = 0;
      }
    }



    if (!digitalRead(switch_room1_floor1) && flag_room1_floor1 == 0) {
      delay(40);
      if (!digitalRead(switch_room1_floor1)) {
        digitalWrite(light_room1_floor1, !digitalRead(light_room1_floor1));
        flag_room1_floor1 = 1;
      }
    }

    if (flag_room1_floor1) {
      if (digitalRead(switch_room1_floor1)) {
        delay(50);
        if (digitalRead(switch_room1_floor1)) flag_room1_floor1 = 0;
      }
    }

  }

  //--------------

  // if (Serial.available() > 0)
  // {

  //   incomingByte = Serial.read();
  //   while (Serial.available()) Serial.read();
  //   switch (incomingByte)
  //   {
  //   }
  //   while (Serial.available()) Serial.read();
  // }//--end-serial

}//--end-loop
