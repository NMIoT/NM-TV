#ifndef DISPLAY_H
#define DISPLAY_H



void tft_bl_active(void);
void ui_switch_next_page_cb();
void ui_switch_previous_page_cb();
void ui_enter_or_exit_current_page_cb();
void display_thread(void *args);

#endif // DISPLAY_H
