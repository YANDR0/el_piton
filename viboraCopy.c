#include <stdio.h>
#include "ripes_system.h"
#define H LED_MATRIX_0_HEIGHT    //Y
#define W LED_MATRIX_0_WIDTH    //X

//Global variables
int snake[H*W];
int tail = 0;
int* leds = LED_MATRIX_0_BASE;
int* dpad = D_PAD_0_BASE;

//Draw an square of 2x2
void drawSquare(int* coords, int color){
    int* p = *coords + leds;
    *p = color;
    *(p+1) = color;
    *(p+H) = color;
    *(p+H+1) = color;
}

//Check with button was pressed in the pad
void checkPad(int* dir){
    if(*dpad){
        *dir = -2*H;
    } else if(*(dpad+1)){
        *dir = 2*H;
    } else if(*(dpad+2)){
        *dir = -2;
    } else if(*(dpad+3)){
        *dir = 2;
    }
}

//Main :v
void main(void){
    
    snake[0] = (H/2-2)*W + (W/2-2);    //y*H + x
    int dir = 0;
    int wait = 0;
    
    while(1){    //Game loop
        checkPad(&dir);
        if(wait < 2000){
            wait++;
            continue;
        }
        wait = 0;
        int* draw = snake[0] + leds;
        drawSquare(&snake[tail], 0);
        snake[0] += dir;
        draw = snake[0] + leds;
        drawSquare(&snake[0], 0xFFFFFF);
    }

}



    

