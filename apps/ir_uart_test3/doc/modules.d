pio: 

navswitch: pio

ledmat: pio

display: ledmat

font: 

tinygl: display font

timer: 

pacer: timer

uint8toa: 

usart1: 

prescale: 

timer0: prescale

ir_uart: usart1 timer0 pio

ir_uart_test3: navswitch tinygl pacer uint8toa ir_uart

