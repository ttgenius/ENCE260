system_clock_init@system.c: 

system_watchdog_timer_init@system.c: 

system_init@system.c: system_clock_init system_watchdog_timer_init

pio_config_set@pio.c: 

ledmat_init@ledmat.c: pio_config_set pio_config_set

display_clear@display.c: 

display_init@display.c: ledmat_init display_clear

tinygl_text_speed_set@tinygl.c: 

tinygl_clear@tinygl.c: display_clear

tinygl_init@tinygl.c: display_init tinygl_text_speed_set tinygl_clear

tinygl_font_set@tinygl.c: 

tinygl_text_mode_set@tinygl.c: 

tinygl_text_dir_set@tinygl.c: 

navswitch_init@navswitch.c: 

timer0_mode_set@timer0.c: 

timer0_init@timer0.c: timer0_mode_set

prescale_select@prescale.c: 

timer0_prescaler_get@timer0.c: 

timer0_running_p@timer0.c: timer0_prescaler_get

timer0_prescaler_set@timer0.c: 

timer0_period_set@timer0.c: prescale_select timer0_running_p timer0_prescaler_set

timer0_output_set@timer0.c: 

timer0_start@timer0.c: timer0_prescaler_set

usart1_baud_divisor_set@usart1.c: 

usart1_init@usart1.c: usart1_baud_divisor_set

ir_uart_init@ir_uart.c: pio_config_set timer0_init timer0_period_set timer0_mode_set timer0_output_set timer0_start usart1_init

timer_init@timer.c: 

pacer_init@pacer.c: timer_init

uint8toa@uint8toa.c: 

tinygl_draw_message@tinygl.c: strncpy strlen

tinygl_text@tinygl.c: tinygl_draw_message

show_byte@ir_uart_test3.c: uint8toa tinygl_text

timer_get@timer.c: 

timer_wait_until@timer.c: timer_get

pacer_wait@pacer.c: timer_wait_until

tinygl_point@tinygl.c: 

font_pixel_get@font.c: 

tinygl_font_pixel_get@tinygl.c: font_pixel_get

display_pixel_set@display.c: 

tinygl_draw_point@tinygl.c: display_pixel_set

tinygl_draw_char@tinygl.c: tinygl_point tinygl_font_pixel_get tinygl_draw_point tinygl_point tinygl_draw_point tinygl_point tinygl_font_pixel_get tinygl_draw_point tinygl_point tinygl_draw_point

tinygl_draw_string@tinygl.c: tinygl_draw_char

tinygl_text_advance@tinygl.c: tinygl_draw_string tinygl_draw_string

pio_output_high@pio.c: 

pio_output_low@pio.c: 

ledmat_display_column@ledmat.c: pio_output_high pio_output_low pio_output_high pio_output_low

display_update@display.c: ledmat_display_column

tinygl_update@tinygl.c: tinygl_text_advance display_update

usart1_read_ready_p@usart1.c: 

ir_uart_read_ready_p@ir_uart.c: usart1_read_ready_p

usart1_getc@usart1.c: usart1_read_ready_p

ir_uart_getc@ir_uart.c: usart1_getc

pio_config_get@pio.c: 

_delay_loop_1@navswitch.c: 

pio_input_get@pio.c: 

navswitch_update@navswitch.c: pio_config_get pio_config_set pio_config_set _delay_loop_1 pio_input_get pio_config_set

navswitch_push_event_p@navswitch.c: 

usart1_write_ready_p@usart1.c: 

usart1_putc@usart1.c: usart1_write_ready_p

usart1_write_finished_p@usart1.c: 

ir_uart_write_finished_p@ir_uart.c: usart1_write_finished_p

ir_uart_putc@ir_uart.c: usart1_putc ir_uart_write_finished_p _delay_loop_1 ir_uart_read_ready_p ir_uart_getc

main@ir_uart_test3.c: system_init tinygl_init tinygl_font_set tinygl_text_speed_set tinygl_text_mode_set tinygl_text_dir_set navswitch_init ir_uart_init pacer_init show_byte pacer_wait tinygl_update ir_uart_read_ready_p ir_uart_getc show_byte navswitch_update navswitch_push_event_p ir_uart_putc navswitch_push_event_p ir_uart_putc

