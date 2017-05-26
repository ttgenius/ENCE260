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

navswitch_init@navswitch.c: 

tinygl_font_set@tinygl.c: 

tinygl_text_mode_set@tinygl.c: 

tinygl_text_dir_set@tinygl.c: 

tinygl_draw_message@tinygl.c: strncpy strlen

tinygl_text@tinygl.c: tinygl_draw_message

timer_init@timer.c: 

pacer_init@pacer.c: timer_init

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

pio_config_get@pio.c: 

_delay_loop_1@navswitch.c: 

pio_input_get@pio.c: 

navswitch_update@navswitch.c: pio_config_get pio_config_set pio_config_set _delay_loop_1 pio_input_get pio_config_set

navswitch_push_event_p@navswitch.c: 

main@hello5.c: system_init tinygl_init navswitch_init tinygl_font_set tinygl_text_speed_set tinygl_text_mode_set tinygl_text_dir_set tinygl_text pacer_init pacer_wait tinygl_update navswitch_update navswitch_push_event_p tinygl_text_mode_set navswitch_push_event_p tinygl_text_mode_set

