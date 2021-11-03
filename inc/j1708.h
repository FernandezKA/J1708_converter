#ifndef _j1708_h_
#define _j1708_h_
//This enum for indicate state of bus
enum BUS_STATE{
  empty = 0, 
  transmit, 
  collision, 
  wait_time
};
extern enum BUS_STATE bus_state;


enum Receive_FSM{
  MID = 0, 
  DATA, 
  CRC
};
extern enum Receive_FSM R_FSM;


enum TSTATE{
  wait = 0, 
  stop_package,
  free_bus
};
extern enum TSTATE tState;
extern uint16_t u16cTime;
//Struct for j1708 packet
struct J1708{
  volatile uint8_t MID;
  volatile uint8_t data[21U];
  volatile uint8_t CRC;
};
typedef struct J1708 j1708;
//User variables
extern volatile j1708 jReceiveStr;
extern volatile j1708 jTransmitStr;
extern volatile uint8_t u8TimePrior;
//User function definition
//This function for receieve data from j1708 bus
j1708 jReceive(enum Receive_FSM *eFSM);
//This function for transmit data from j1708 bus
void jTransmit(volatile j1708* tStruct, uint8_t u8Priority);
//This function is IRQ handler for Tim1 
void Tim1_Handler(enum TSTATE* cState, uint16_t* cTime);
//This function is IRQ handler for UART1
void UART1_Rx_Handler(uint16_t* cTime, enum TSTATE* cState,enum Receive_FSM* R_FSM,volatile j1708* RPack);
#endif