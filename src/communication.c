#include "communication.h"

static inline void SendArray(char *pData, uint8_t Size);
//Function declaration
void ReflectPacket(enum DirectionReflect Direction, struct J1708 *packet, uint8_t priority)
{
      if (Direction == From_RS232_to_j1708)
      {
            //Send to j1708
            jTransmit(packet, priority);
      }
      else
      {
            uint8_t Length = packet->length - 1;
            //Send to RS232

            uint8_t u8CRC = u8CalcCRC(packet->data, packet->length);

            if (u8CRC == packet->CRC)
            {
                  while (test_status(transmit_data_reg_empty) != transmit_data_reg_empty)
                  {
                        asm("nop");
                  }
                  uart_send(packet->MID);
                  for (uint8_t i = 0; i < packet->length; ++i)
                  {
                        while (test_status(transmit_data_reg_empty) != transmit_data_reg_empty)
                        {
                              asm("nop");
                        }
                        uart_send(packet->data[i]);
                  }
                  while (test_status(transmit_data_reg_empty) != transmit_data_reg_empty)
                  {
                        asm("nop");
                  }
                  uart_send(packet->CRC);
                  while (test_status(transmit_data_reg_empty) != transmit_data_reg_empty)
                  {
                        asm("nop");
                  }
                  uart_send(0x0D);
                  while (test_status(transmit_data_reg_empty) != transmit_data_reg_empty)
                  {
                        asm("nop");
                  }
                  uart_send(0x0A);
            }
            else
            {
                  SendArray("Invalid CRC\n\r", 13);
                  while (test_status(transmit_data_reg_empty) != transmit_data_reg_empty)
                  {
                        asm("nop");
                  }
                  uart_send(packet->MID);
                  for (uint8_t i = 0; i < packet->length; ++i)
                  {
                        while (test_status(transmit_data_reg_empty) != transmit_data_reg_empty)
                        {
                              asm("nop");
                        }
                        uart_send(packet->data[i]);
                  }
                  while (test_status(transmit_data_reg_empty) != transmit_data_reg_empty)
                  {
                        asm("nop");
                  }
                  uart_send(packet->CRC);
                  while (test_status(transmit_data_reg_empty) != transmit_data_reg_empty)
                  {
                        asm("nop");
                  }
                  uart_send(0x0D);
                  while (test_status(transmit_data_reg_empty) != transmit_data_reg_empty)
                  {
                        asm("nop");
                  }
                  uart_send(0x0A);
            }
      }
}

uint8_t u8CalcCRC(uint8_t *pData, uint8_t size)
{
      uint8_t sum = 0x00;

      for (uint8_t i = 0; i < size; ++i)
      {
            if (sum + pData[i] > 0xFF)
            {
                  sum++;
                  sum += pData[i];
            }
            else
            {
                  sum += pData[i];
            }
      }

      return sum ^ 0xFF;
}
//This function send message
static inline void SendArray(char *pData, uint8_t Size)
{
      for (uint8_t i = 0; i < Size; ++i)
      {
            //Wait TXE flags
            while (test_status(transmit_data_reg_empty) != transmit_data_reg_empty)
            {
                  asm("nop");
            }
            uart_send(*(pData + i));
      }
}
