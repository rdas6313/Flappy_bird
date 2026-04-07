#ifndef GAME_H
#define GAME_H

#include<stdint.h>
#include<stddef.h>
#include "../Bsp/header/sh1106_graphics.h"

#define MAX_SCENE_ITEM 8

typedef enum Input{
    IDLE,PRESSED
} BUTTON;

typedef enum SceneType{
    FLOOR,PIPE,BIRD
} SceneType;

typedef enum GameState{
    GAME_MENU,GAME_LOOP,GAME_OVER
} GameState;

void Game_Timer_Start();
void Game_Timer_Stop();
void Game_Start();
uint8_t Game_Input();
void Game_Delay(uint16_t ms);

#define BIRD_JUMP_INITIAL 4
#define GAME_SPEED_INITIAL 30
#define BIRD_JUMP_INCREMENT 1
#define GAME_SPEED_DECREMENT 2
#define MAX_BIRD_JUMP 9
#define MIN_GAME_SPEED 4
extern uint8_t current_bird_jump;
extern uint8_t current_game_speed;


#include "pipe.h"
#include "floor.h"
#include "bird.h"
#include "menu.h"
#include "scoreboard.h"
#endif