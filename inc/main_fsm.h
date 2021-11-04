#ifndef _main_fsm_h_
#define _main_fsm_h_
#include "stm8s_conf.h"
//User defines

//User variables
enum FSM{
  wait_action =0, 
  wait_mid, 
  wait_length, 
  wait_data, 
  calculate_crc, 
  send
} main_fsm;
//Function declaration

#endif