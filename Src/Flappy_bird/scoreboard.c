
#include "scoreboard.h"

static void create_formated_string(char *msg, const char *prefix, uint16_t num)
{
    int i = 0, j = 0;
    char temp[10];

    while(prefix[i])
    {
        msg[i] = prefix[i];
        i++;
    }

    if(num == 0)
    {
        msg[i++] = '0';
        msg[i] = '\0';
        return;
    }

    while(num > 0)
    {
        temp[j++] = (num % 10) + '0';
        num /= 10;
    }

    while(j > 0)
    {
        msg[i++] = temp[--j];
    }

    msg[i] = '\0';
}

static void heading_render(){
    gfx_draw_string(SCOREBOARD_HEADING_POS_X, SCOREBOARD_HEADING_POS_Y, SCOREBOARD_HEADING,SCOREBOARD_HEADING_SIZE,1);
}

static void button_render(uint16_t btn_press){
    if(btn_press)
    gfx_draw_rect(SCOREBOARD_BUTTON_RECT_POS_X, SCOREBOARD_BUTTON_RECT_POS_Y, SCOREBOARD_BUTTON_RECT_POS_WIDTH, SCOREBOARD_BUTTON_RECT_POS_HEIGHT,1);
    else
        gfx_draw_fill_rect(SCOREBOARD_BUTTON_RECT_POS_X, SCOREBOARD_BUTTON_RECT_POS_Y, SCOREBOARD_BUTTON_RECT_POS_WIDTH, SCOREBOARD_BUTTON_RECT_POS_HEIGHT,1);
    
    gfx_draw_string(SCOREBOARD_BUTTON_STR_POS_X, SCOREBOARD_BUTTON_STR_POS_Y, SCOREBOARD_BUTTON,SCOREBOARD_BUTTON_SIZE,(btn_press & 0x01));
}

static void score_render(uint16_t score){
    char score_msg[50];
    create_formated_string(score_msg, "Score: ", score);
    gfx_draw_string(SCOREBOARD_SCORE_POS_X, SCOREBOARD_SCORE_POS_Y, score_msg,SCOREBOARD_SCORE_SIZE,1);
}

static void render(uint16_t score,uint8_t btn_press){
    gfx_clear_buffer();
    heading_render();
    score_render(score);
    button_render(btn_press);
    gfx_update_display();
}

void Scoreboard_render(uint16_t score){

    render(score,0);

}

void Scoreboard_Btn_clicked(uint16_t score){
    render(score,1);
}












