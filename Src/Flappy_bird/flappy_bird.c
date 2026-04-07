#include<stdio.h>
#include "stm32f4xx_gpio_driver.h"
#include "stm32f4xx_i2c_driver.h"
#include "stm32f4xx_timer.h"
#include "game.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"

#define HARDWARE_TIMER 0
#define TASK_STACK_SIZE 512

#define MASTER_ADDR 0x31
#define SLAVE_ADDR  SH1106_I2C_ADDRESS
#define MAX_BUFFER_SIZE 25

#define LOW 0
#define HIGH 1
#define I2C_DISABLE_SR 0
#define I2C_ENABLE_SR 1

// I2C1_SCL -> PB6
// I2C1_SDA -> PB7

void BtnClickedCallback();
void timerCallback( TimerHandle_t xTimer );


I2C_Handle_t I2CHandle1;

TimerHandle_t timer_handle;
TaskHandle_t GameTaskHandle;
TaskHandle_t InputHandlerTaskHandle;

typedef enum{
    BTN_IDLE,
    BTN_PRESSED
} BtnState;

#if (defined(HARDWARE_TIMER) && (HARDWARE_TIMER == 1))
static void Delay_ms(volatile uint16_t delay){
    Start_basic_timer(TIM6);
    while(delay > 0){
        while(!Get_Timer_update_status(TIM6)); // Wait for timer update event
        delay--;
    }
    Stop_basic_timer(TIM6);
}

void Init_Timer(){
    uint16_t prescaler = 100 -1;
    uint16_t arr = 160 - 1; // 1ms delay at 16MHz clock with prescaler of 100
    Init_basic_timer(TIM6, prescaler, arr);
    
}
#else
static void Delay_ms(volatile uint16_t delay){
    vTaskDelay(pdMS_TO_TICKS(delay));
}
#endif

void init_Gpios(){
    GPIO_Handle_t I2CGpioHandle1;
    memset(&I2CGpioHandle1,0,sizeof(I2CGpioHandle1));
    I2CGpioHandle1.pGPIOx = GPIOB;
    I2CGpioHandle1.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    I2CGpioHandle1.GPIO_PinConfig.GPIO_PinAltFunMode = GPIO_AF_4;
    I2CGpioHandle1.GPIO_PinConfig.GPIO_PinOPType = GPIO_OUT_OD;
    I2CGpioHandle1.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NO;
    I2CGpioHandle1.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OUT_SPEED_HIGH;
    // enable GPIOB clock
    GPIO_PeriClockControl(GPIOB, ENABLE);

    // I2C1_SCL
    I2CGpioHandle1.GPIO_PinConfig.GPIO_PinNumber = 6;
    GPIO_Init(&I2CGpioHandle1);
    // I2C1_SDA
    I2CGpioHandle1.GPIO_PinConfig.GPIO_PinNumber = 7;
    GPIO_Init(&I2CGpioHandle1);

    // Button Gpio Config
    GPIO_Handle_t ButtonHandle;
    memset(&ButtonHandle,0,sizeof(ButtonHandle));
    ButtonHandle.pGPIOx = GPIOA;
    ButtonHandle.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_INT_RISE;
    ButtonHandle.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NO;
    ButtonHandle.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OUT_SPEED_HIGH;

    // enable GPIOA clock
    GPIO_PeriClockControl(GPIOA, ENABLE);
    // User Button PA0
    ButtonHandle.GPIO_PinConfig.GPIO_PinNumber = 0;
    GPIO_Init(&ButtonHandle);

    GPIO_IRQConfig(EXTI0, 6, ENABLE);
}

// master i2c1 initialization
void I2C1_Init(){
    
    I2CHandle1.pI2Cx = I2C1;
    I2CHandle1.i2cConfig.I2C_SCLSpeed = I2C_SCL_SPEED_FM;
    I2CHandle1.i2cConfig.I2C_DeviceAddress = MASTER_ADDR;
    I2CHandle1.i2cConfig.I2C_ACKControl = I2C_ACK_ENABLE;
    I2C_Init(&I2CHandle1);
}

void i2c_send_SH1106(uint8_t address,uint8_t *data, uint16_t size){
    I2C_MasterSendData(&I2CHandle1, data, size, SLAVE_ADDR, I2C_DISABLE_SR);
}

void delay(uint16_t ms){
    Delay_ms(ms);
}

void Game_Delay(uint16_t ms){
    Delay_ms(ms);
}

uint8_t Game_Input(){
    BaseType_t status = xTaskNotifyWait(0,0, NULL, 0);
    if(status == pdTRUE){
        return 1; // button was pressed
    }
    return 0; // no input
}

void GameTask(void *params){
    
    while(1){
        Game_Start();
    }
}

void Game_Timer_Start(){
    current_bird_jump = BIRD_JUMP_INITIAL;
    current_game_speed = GAME_SPEED_INITIAL;
    xTimerStart(timer_handle, 0); // Start the timer for increasing difficulty
}


void Game_Timer_Stop(){
    xTimerStop(timer_handle, 0); // Stop the timer for increasing difficulty
}

void InputHandlerTask(void *params){
    static uint8_t BtnState = BTN_IDLE;
    while(1){
        
        switch(BtnState){
            case BTN_IDLE:{
                xTaskNotifyWait(0,0, NULL, portMAX_DELAY);
                vTaskDelay(pdMS_TO_TICKS(10)); // debounce delay
                if(GPIO_ReadFromInputPin(GPIOA,0) == 1){ // check if button is still pressed
                    BtnState = BTN_PRESSED;
                }
                break;
            }
            case BTN_PRESSED:{
                
                xTaskNotify(GameTaskHandle, 0, eNoAction);
                BtnState = BTN_IDLE;
                break;
            }
        }
    }
}

void BtnClickedCallback(){
    // called from ISR when button is pressed
    
    GPIO_IRQHandling(0); // Clear the EXTI line pending bit
    BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
    xTaskNotifyFromISR(InputHandlerTaskHandle, 0, eNoAction,&pxHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
    
    
}

void timerCallback( TimerHandle_t xTimer ){
    static uint8_t time = 0;

    if(time == 3 && (current_bird_jump + BIRD_JUMP_INCREMENT) < MAX_BIRD_JUMP)
        current_bird_jump += BIRD_JUMP_INCREMENT;
    if((current_game_speed - GAME_SPEED_DECREMENT) > MIN_GAME_SPEED)    
        current_game_speed -= GAME_SPEED_DECREMENT;
    
    time = (time + 1) % 4;
}

int main(){

    init_Gpios();

    #if (defined(HARDWARE_TIMER) && (HARDWARE_TIMER == 1))
    Init_Timer();
    #endif
    
    I2C1_Init();
    timer_handle = xTimerCreate("Timer", pdMS_TO_TICKS(2000), pdTRUE, NULL, timerCallback); // create a timer for button debounce
    BaseType_t status = xTaskCreate(GameTask, "Game Task", TASK_STACK_SIZE, NULL, 1, &GameTaskHandle);
    configASSERT(status == pdPASS);
    status = xTaskCreate(InputHandlerTask, "Input Handler Task", TASK_STACK_SIZE, NULL, 2, &InputHandlerTaskHandle);
    configASSERT(status == pdPASS);


    vTaskStartScheduler();    
    
    while(1);

    return 0;
}