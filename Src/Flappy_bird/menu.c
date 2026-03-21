
#include "menu.h"


static void heading_render(){
    gfx_draw_string(MENU_HEADING_POS_X, MENU_HEADING_POS_Y, MENU_HEADING,MENU_HEADING_SIZE,1);
}

static void button_render(uint8_t btn_press){
    if(btn_press)
    gfx_draw_rect(MENU_BUTTON_RECT_POS_X, MENU_BUTTON_RECT_POS_Y, MENU_BUTTON_RECT_POS_WIDTH, MENU_BUTTON_RECT_POS_HEIGHT,1);
    else
        gfx_draw_fill_rect(MENU_BUTTON_RECT_POS_X, MENU_BUTTON_RECT_POS_Y, MENU_BUTTON_RECT_POS_WIDTH, MENU_BUTTON_RECT_POS_HEIGHT,1);
    
    gfx_draw_string(MENU_BUTTON_STR_POS_X, MENU_BUTTON_STR_POS_Y, MENU_BUTTON,MENU_BUTTON_SIZE,(btn_press & 0x01));
}

static void render(uint8_t btn_press){
    gfx_clear_buffer();
    heading_render();
    button_render(btn_press);
    gfx_update_display();
}

void Menu_render(){

    render(0);

}

void Menu_Btn_clicked(){
    render(1);
}










