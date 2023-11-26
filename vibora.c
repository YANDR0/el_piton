#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "ripes_system.h"
#define H LED_MATRIX_0_HEIGHT    //Y
#define W LED_MATRIX_0_WIDTH    //X

//Global variables
int snake[H*W];
int tail = 0;
int apple = 0;
int* leds = LED_MATRIX_0_BASE;
int* dpad = D_PAD_0_BASE;

//Draw an square of 2x2
void drawSquare(int coords, int color){
    int* p = coords + leds;
    *p = color;
    *(p+1) = color;
    *(p+H) = color;
    *(p+H+1) = color;
}

//Check with button was pressed in the pad
void checkPad(int* dir){
    if(dpad[0]) *dir = -2*H;
    else if(dpad[1]) *dir = 2*H;
    else if(dpad[2]) *dir = -2;
    else if(dpad[3]) *dir = 2;
}

void summonApple(){
    apple = (rand()%H/2)*W*2 + rand()%W/2*2;
    drawSquare(apple, 0x00FF00);
}

int checkCollision(int c1, int c2){
    return (c1 == c2);
}

void drawSnake(int tail, int dir){
    int next = snake[0];
    drawSquare(snake[0], 0xFF0000);
    snake[0] += dir;
    
    for(int i = 1; i < tail; i++){
        int temp = snake[i];
        drawSquare(snake[i], 0xFF0000);
        snake[i] = next;
        next = temp;
    }
}

void clear(){
    for(int i = 0; i < H*W; i++){
        drawSquare(i, 0);
    }
}

//Main :v
void main(void){
    
    snake[0] = (H/2-2)*W + (W/2-2);    //y*H + x
    snake[1] = (H/2-2)*W + (W/2-4);    //y*H + x
    snake[2] = (H/2-2)*W + (W/2-6);    //y*H + x
    tail = 2;
    int dir = 0;
    int wait = 2000;
    clear();
    
    while(dir == 0) checkPad(&dir);    //Esperar a tocar botón para iniciar
    
    
    while(1){    //Game loop
        checkPad(&dir);
        if(wait++ < 2000) continue;
        wait = 0;
        drawSquare(snake[tail], 0);
        drawSnake(tail+1, dir);
    }

}



    

