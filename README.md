Данный проект представляет из себя конвертер J1708 -> RS232. 
MCU: STM8S103F3P6

Pin description:
--SOFT UART
PORTD2 - SOFT UART TX
PORTD3 - SOFT UART RX
--HW UART
PORTD5 - HW UART1 TX
PORTD6 - HW UART1 RX
--LED
PORTB5 - LED RUN
--BAUDRATE
For soft uart 9600
For hard UART 19200
Fclk = 16 MHz (internal RC)

