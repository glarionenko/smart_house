#include <SimpleModbusSlave.h>
//CONFIGURABLE
#define MODBUS_ID 1
enum
{
  CONTACT_1_SET,
  CONTACT_2_SET,
  LAMP_STAIRs_SET,
  CONTACT_1_STATE,
  CONTACT_2_STATE,
  LAMP_STAIRS_STATE,
  HOLDING_REGS_SIZE // leave this one
  // total number of registers for function 3 and 16 share the same register array
  // i.e. the same address space
};
unsigned int holdingRegs[HOLDING_REGS_SIZE];
// the setup routine runs once when you press reset:
void setup() {   
modbus_configure(&Serial, 4800, SERIAL_8N2, MODBUS_ID, 1, HOLDING_REGS_SIZE, holdingRegs);
  modbus_update_comms(4800, SERIAL_8N2, MODBUS_ID);
    
  pinMode(23, OUTPUT);   
pinMode(25, OUTPUT);  
digitalWrite(23,HIGH);
digitalWrite(25,HIGH);
pinMode(7, OUTPUT);     
}

// the loop routine runs over and over again forever:
void loop() {
modbus_update();
digitalWrite(7,holdingRegs[CONTACT_1_SET]);
}
