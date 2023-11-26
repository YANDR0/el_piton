#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "ripes_system.h"
#define H LED_MATRIX_0_HEIGHT
#define W LED_MATRIX_0_WIDTH    

//Global variables
int snake[H*W];    //Position of snake's parts - Coords of a part is: y*H + x
int apple = 0;    //Position of apple
int* leds = LED_MATRIX_0_BASE;
int* dpad = D_PAD_0_BASE;

//Draw an square of 2x2
void drawSquare(int coords, int color){    //Draw 2x2 squares in certain coords
    int* p = coords + leds;
    *p = color;
    *(p+1) = color;
    *(p+H) = color;
    *(p+H+1) = color;
}

//Check with button was pressed in the pad
//Allow the movement of snakein certain directions
void checkPad(int* dir){
    if(dpad[0] && *dir != 2*H) *dir = -2*H;
    else if(dpad[1] && *dir != -2*H) *dir = 2*H;
    else if(dpad[2] && *dir != 2) *dir = -2;
    else if(dpad[3] && *dir != -2) *dir = 2;
}

//Por hacer
void summonApple(){
    apple = (rand()%H/2)*W*2 + rand()%W/2*2;
    drawSquare(apple, 0x00FF00);
}

//Por hacer
int checkCollision(int c1, int c2){
    return (c1 == c2);
}

//Draw the snake in the begining
void startSnake(int tail){
    for(int i = 0; i <= tail; i++)
        drawSquare(snake[i], 0xFF0000);
}

//Draw the snake and update the coords of each part
void drawSnake(int tail, int dir){
    int next = snake[0];
    snake[0] += dir;
    drawSquare(snake[0], 0xFF0000);
    drawSquare(snake[tail], 0);
    
    for(int i = 1; i <= tail; i++){
        int temp = snake[i];
        snake[i] = next;
        next = temp;
    }
}

//Clear leds 
void clear(){
    for(int i = 0; i < H*W; i++){
        drawSquare(i, 0);
    }
}

//Main :v
void main(void){
    
    //Initialization of snake and variables
    snake[0] = (H/2-2)*W + (W/2-2);    //Parts of snake
    snake[1] = (H/2-2)*W + (W/2-4);
    snake[2] = (H/2-2)*W + (W/2-6);
    int tail = 2;    //Position of tail in array
    int dir = 0;    //Direction of snake
    int wait = 2000;    //Slow the movement and process
    clear();
    
    startSnake(tail);
    while(dir == 0) checkPad(&dir);    //Esperar a tocar bot?n para iniciar
    
    while(1){    //Game loop
        checkPad(&dir);
        if(wait++ < 2000) continue;    //Cronometer :v
        wait = 0;
        drawSnake(tail, dir);
    }

}



    

