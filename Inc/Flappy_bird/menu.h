#ifndef MENU_H
#define MENU_H

#include "sh1106_graphics.h"
#define MENU_HEADING_POS_X 10
#define MENU_HEADING_POS_Y 8
#define MENU_HEADING "Flappy Bird"
#define MENU_HEADING_SIZE 2
#define MENU_BUTTON_RECT_POS_X 40
#define MENU_BUTTON_RECT_POS_Y 45
#define MENU_BUTTON_RECT_POS_WIDTH 40
#define MENU_BUTTON_RECT_POS_HEIGHT 15
#define MENU_BUTTON_STR_POS_X (MENU_BUTTON_RECT_POS_X + 3)
#define MENU_BUTTON_STR_POS_Y (MENU_BUTTON_RECT_POS_Y + 5)
#define MENU_BUTTON_SIZE 1
#define MENU_BUTTON "START"



void Menu_render();
void Menu_Btn_clicked();

#endif