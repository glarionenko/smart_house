#define switch_kotel 34
#define switch_bathroom1 36
#define switch_bathroom2 38
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
#define light_bathroom_floor2 9
#define fan_bathroom_floor2 10

#define light2_kitchen_tv 11
#define light_bathroom_floor1 12
#define light1_floor3 49
#define towel_bathroom_floor1 51//полотенчик
#define towel_bathroom_floor2 53

#define light_room2_floor2 47
#define fan_bathroom_floor1 45
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


/* Подключение библиотеки модбас*/
#include <SimpleModbusSlave.h> //Подключение библиотеки модбас
SimpleModbusSlave slave(1);   // Слэйв с идентификатором 1
const int num = 27;//количество триггеров
uint16_t trigger[num + num];          // Триггеры
uint16_t state[num]; //статусы
uint16_t last_trigger[num];// предыдущие значения триггеров
/* НОВЫЕ ДЕФАЙНЫ */

#define tr_kontakt1 0
#define tr_light1_kitchen 1
#define tr_light2_kitchen_tv 2
#define tr_pir 3
#define tr_light_tambur 4
#define tr_light_hall_floor1 5
#define tr_light_on_stairs 6
#define tr_light_stairs_floor1 7
#define tr_kontakt2 8
#define tr_light_kotel 9
#define tr_light_bathroom_floor1 10
#define tr_fan_bathroom_floor1 11
#define tr_towel_bathroom_floor1 12
#define tr_light_room1_floor1 13
#define tr_light_street_floor1 14
#define tr_light_street_floor2 15
#define tr_light_prachka_floor2 16
#define tr_light_room1_floor2 17
#define tr_socket1_floor2 18
#define tr_light_bathroom_floor2 19
#define tr_fan_bathroom_floor2 20
#define tr_towel_bathroom_floor2 21
#define tr_light_room2_floor2 22
#define tr_light_room3_floor2 23
#define tr_light_wardrobe_floor2 24
#define tr_light1_floor3 25
#define tr_light2_floor3 26

/* НОВЫЕ ДЕФАЙНЫ КОНЕЦ*/
/*ФУНКЦИЯ ОБРАБОТКИ триггеров*/
/*
    if(trigger[sw1_room2_floor2]){
      digitalWrite(light_room2_floor2, !digitalRead(light_room2_floor2));
      trigger[sw1_room2_floor2]=0;
      state[sw1_room2_floor2]=digitalRead(light_room2_floor2);
      }
*/
unsigned long timer_1;
unsigned long timer_2;
boolean started_1;
boolean started_2;
int time_fan = 3000;
void simply_trigger_it(int tr_num, int pin_num) {

  if (trigger[tr_num]) {
    boolean readHere = digitalRead(pin_num);

    digitalWrite(pin_num, !readHere);
    trigger[tr_num] = 0;
    trigger[tr_num + num] = !readHere;
    //написать сюда дотполнительные условия
    if ((tr_num == light_bathroom_floor1) && (readHere == 0)) {
      digitalWrite(fan_bathroom_floor1, 1);
    }
    if ((tr_num == light_bathroom_floor2) && (readHere == 0)) {

      digitalWrite(fan_bathroom_floor2, 1);

    }
    //
    if ((tr_num == light_bathroom_floor2) && (readHere == 1)) {
      started_1 = 1;
      timer_1 = millis();
    }
    if ((tr_num == light_bathroom_floor2) && (readHere == 1)) {
      started_2 = 1;
      timer_2 = millis();
    }
    //
  }
  if (started_1) {
    if ((millis() - timer_1) > time_fan) {
      started_1 = 0;
      digitalWrite(fan_bathroom_floor1, 0);
    }
  }
  if (started_2) {
    if ((millis() - timer_2) > time_fan) {
      started_2 = 0;
      digitalWrite(fan_bathroom_floor2, 0);
    }
  }
}

void triggers_array() {

  simply_trigger_it(tr_kontakt1, kontakt1);
  simply_trigger_it(tr_light1_kitchen, light1_kitchen);
  simply_trigger_it(tr_light2_kitchen_tv, light2_kitchen_tv);
  simply_trigger_it(tr_pir, pir);
  simply_trigger_it(tr_light_tambur, light_tambur);
  simply_trigger_it(tr_light_hall_floor1, light_hall_floor1);
  simply_trigger_it(tr_light_on_stairs, light_on_stairs);
  simply_trigger_it(tr_light_stairs_floor1, light_stairs_floor1);
  simply_trigger_it(tr_kontakt2, kontakt2);
  simply_trigger_it(tr_light_kotel, light_kotel);
  simply_trigger_it(tr_light_bathroom_floor1, light_bathroom_floor1);
  simply_trigger_it(tr_fan_bathroom_floor1, fan_bathroom_floor1);
  simply_trigger_it(tr_towel_bathroom_floor1, towel_bathroom_floor1);
  simply_trigger_it(tr_light_room1_floor1, light_room1_floor1);
  simply_trigger_it(tr_light_street_floor1, light_street_floor1);
  simply_trigger_it(tr_light_street_floor2, light_street_floor2);
  simply_trigger_it(tr_light_prachka_floor2, light_prachka_floor2);
  simply_trigger_it(tr_light_room1_floor2, light_room1_floor2);
  simply_trigger_it(tr_socket1_floor2, socket1_floor2);
  simply_trigger_it(tr_light_bathroom_floor2, light_bathroom_floor2);
  simply_trigger_it(tr_fan_bathroom_floor2, fan_bathroom_floor2);
  simply_trigger_it(tr_towel_bathroom_floor2, towel_bathroom_floor2);
  simply_trigger_it(tr_light_room2_floor2, light_room2_floor2);
  simply_trigger_it(tr_light_room3_floor2, light_room3_floor2);
  simply_trigger_it(tr_light_wardrobe_floor2, light_wardrobe_floor2);
  simply_trigger_it(tr_light1_floor3, light1_floor3);
  simply_trigger_it(tr_light2_floor3, light2_floor3);
  //написать тут все триггеры
  //какой массив шлется в модбас

}

/*ФУНКЦИЯ ОБРАБОТКИ триггеров КОНЕЦ*/
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
int flag_bathroom1 = 0;
int flag_bathroom2 = 0;
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


void setup() {
  /* установка скорости соединения modbus*/
  slave.setup(9600);
  /* установка скорости соединения modbus КОНЕЦ*/
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


  Serial.begin(9600);


}

void sleep() {

  digitalWrite(kontakt1, 0);
  digitalWrite(kontakt2, 0);

  digitalWrite(socket1_floor2, 0);
  digitalWrite(light_stairs_floor1, 0);
  digitalWrite(light_room1_floor1, 0);
  digitalWrite(light_room1_floor1, 0);
  digitalWrite(light_kotel, 0);

  digitalWrite(light_tambur, 0);
  digitalWrite(light1_kitchen, 0);
  digitalWrite(light_hall_floor1, 0);
  digitalWrite(light_bathroom_floor2, 0);
  digitalWrite(fan_bathroom_floor2, 0);

  digitalWrite(light2_kitchen_tv, 0);
  digitalWrite(light_bathroom_floor1, 0);
  digitalWrite(light1_floor3, 0);
  digitalWrite(towel_bathroom_floor1, 0);
  digitalWrite(towel_bathroom_floor2, 0);

  digitalWrite(light_room2_floor2, 0);
  digitalWrite(fan_bathroom_floor1, 0);
  digitalWrite(light_street_floor2, 0);
  digitalWrite(light_on_stairs, 0);
  digitalWrite(light_street_floor1, 0);

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

  sleep_mode = 0;
}



void loop() {
  /* обновление триггеров по модбас */
  triggers_array();
  slave.loop(trigger, sizeof(trigger) / sizeof(trigger[0]));
  /* обновление триггеров по модбас КОНЕЦ*/
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
            if (digitalRead(light_tambur)) {
              digitalWrite(light_tambur, 1);
              delay(500);
            }
            digitalWrite(light_tambur, 0);
            delay(1000);
            digitalWrite(light_tambur, 1);

            digitalWrite(light_street_floor1, 1);
            delay(3000);
            digitalWrite(light_street_floor2, 1);

            digitalWrite(socket1_floor2, 0);
            digitalWrite(light_stairs_floor1, 0);
            digitalWrite(light_room1_floor1, 0);
            digitalWrite(light_room1_floor1, 0);
            digitalWrite(light_kotel, 0);

            //digitalWrite(light_tambur, 0);
            digitalWrite(light1_kitchen, 0);
            digitalWrite(light_hall_floor1, 0);
            digitalWrite(light_bathroom_floor2, 0);
            digitalWrite(fan_bathroom_floor2, 0);

            digitalWrite(light2_kitchen_tv, 0);
            digitalWrite(light_bathroom_floor1, 0);
            digitalWrite(light1_floor3, 0);
            digitalWrite(towel_bathroom_floor1, 0);
            digitalWrite(towel_bathroom_floor2, 0);

            digitalWrite(light_room2_floor2, 0);
            digitalWrite(fan_bathroom_floor1, 0);
            digitalWrite(light_street_floor2, 0);
            digitalWrite(light_on_stairs, 0);
            //digitalWrite(light_street_floor1, 0);

            //digitalWrite(light_prachka_floor2, 0);
            digitalWrite(light_room1_floor2, 0);
            digitalWrite(light_wardrobe_floor2, 0);
            digitalWrite(light_room3_floor2, 0);
            digitalWrite(light2_floor3, 0);
            flag_error = 0;
            for (long int i = 0; i < 60000; i++) {
              delay(1);
              if (flag_input_code == 1 && digitalRead(code)) {
                delay(200);
                if (digitalRead(code)) flag_input_code = 0;
              }

              if (i == 10000) digitalWrite(light_tambur, 0);

              if (!digitalRead(code) && flag_input_code == 0) {
                delay(300);
                if (!digitalRead(code)) {
                  wake_up();
                  flag_error = 1;
                  i = 63000;
                  flag_input_code = 1;
                }
              }

            }


            if (flag_error == 0) {
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

    if (!digitalRead(switch_bathroom1) && flag_bathroom1 == 0) {
      delay(40);
      if (!digitalRead(switch_bathroom1)) {
        if (digitalRead(light_bathroom_floor1)) {
          digitalWrite(light_bathroom_floor1, 0);
          flag_fan1 = 1;
          timer_fan1 = millis();
          digitalWrite(fan_bathroom_floor1, 1);
        }

        else {
          digitalWrite(light_bathroom_floor1, 1);
          flag_fan1 = 0;
          digitalWrite(fan_bathroom_floor1, 1);
        }

        flag_bathroom1 = 1;
      }
    }

    if (flag_bathroom1) {
      if (digitalRead(switch_bathroom1)) {
        delay(50);
        if (digitalRead(switch_bathroom1)) flag_bathroom1 = 0;
      }
    }

    if (flag_fan1) {
      if (millis() - timer_fan1 > const_timer_fan1) {
        flag_fan1 = 0;
        digitalWrite(fan_bathroom_floor1, 0);
      }
    }
    //--------------


    if (!digitalRead(switch_bathroom2) && flag_bathroom2 == 0) {
      delay(40);
      if (!digitalRead(switch_bathroom2)) {

        if (digitalRead(light_bathroom_floor2)) {
          digitalWrite(light_bathroom_floor2, 0);
          flag_fan2 = 1;
          timer_fan2 = millis();
          digitalWrite(fan_bathroom_floor2, 1);
        }

        else {
          digitalWrite(light_bathroom_floor2, 1);
          flag_fan1 = 0;
          digitalWrite(fan_bathroom_floor2, 1);
        }


        flag_bathroom2 = 1;
      }
    }



    if (flag_bathroom1) {
      if (digitalRead(switch_bathroom2)) {
        delay(50);
        if (digitalRead(switch_bathroom2)) flag_bathroom2 = 0;
      }
    }

    if (flag_fan2) {
      if (millis() - timer_fan2 > const_timer_fan2) {
        flag_fan2 = 0;
        digitalWrite(fan_bathroom_floor2, 0);
      }
    }
    //--------------


    //--------------


    if (!digitalRead(switch_bathroom2) && flag_bathroom2 == 0) {
      delay(40);
      if (!digitalRead(switch_bathroom2)) {
        digitalWrite(light_bathroom_floor2, !digitalRead(light_bathroom_floor2));
        digitalWrite(fan_bathroom_floor2, digitalRead(light_bathroom_floor2));
        flag_bathroom2 = 1;
      }
    }

    if (flag_bathroom2) {
      if (digitalRead(switch_bathroom2)) {
        delay(50);
        if (digitalRead(switch_bathroom2)) flag_bathroom2 = 0;
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
