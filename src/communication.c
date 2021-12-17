#include "communication.h"
//Function declaration
void ReflectPacket(enum DirectionReflect Direction,struct J1708* packet, uint8_t priority)
{
  if(Direction == From_RS232_to_j1708){
     //Send to j1708
    jTransmit(packet, priority);
  }
  else{
    uint8_t Length = packet ->length - 1;
    //Send to RS232
    for(uint8_t i = 0; i < packet->length; ++i){
      switch(i){
      case 0://MID
        while(test_status(transmit_data_reg_empty) != transmit_data_reg_empty) {asm("nop");}
        uart_send(packet->MID);
        break;
        
      default://Data 
        if (i < packet->length){
          while(test_status(transmit_data_reg_empty) != transmit_data_reg_empty) {asm("nop");}
          uart_send(packet->data[i]); 
        }
        else{
          while(test_status(transmit_data_reg_empty) != transmit_data_reg_empty) {asm("nop");}
          uart_send(packet->CRC);
        }
        break;
      }
    }
  }
}