#ifndef _j1708_h_
#define _j1708_h_
//This enum for indicate state of bus
enum BUS_STATE{
  empty = 0, 
  transmit, 
  collision, 
  wait_time
} bus_state;
//Struct for j1708 packet
struct J1708{
  volatile uint8_t MID;
  volatile uint8_t data[21U];
  volatile uint8_t CRC;
};
typedef struct J1708 j1708;
//This function for receieve data from j1708 bus

#endif