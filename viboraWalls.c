#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "ripes_system.h"
#define H LED_MATRIX_0_HEIGHT
#define W LED_MATRIX_0_WIDTH 
#define RED 0xFF0055
#define GREEN 0x00EE77
   

//Global variables
int snake[H*W/4];    //Position of snake's parts
int apple = 0;    //Position of apple
int walls[3] = {-4, -4, -4};
int points = 0;
int randomness = 0;
int* leds = LED_MATRIX_0_BASE;
int* dpad = D_PAD_0_BASE;

//Draw an square of 2x2
void drawSquare(int coords, int color){    //Draw 2x2 squares in certain coords
    int* p = coords + leds;                    // Y * Width + X
    *p = color;                                // Coords % Width = X 
    *(p+1) = color;                            // Coords / Width = Y
    *(p+W) = color;
    *(p+W+1) = color;
}

//Check which button was pressed in the pad
//Allow the movement of snake in certain directions
void checkPad(int* dir, int last){
    if(dpad[0] && last != 2*W) *dir = -2*W;        //Up
    else if(dpad[1] && last != -2*W) *dir = 2*W;    //Down
    else if(dpad[2] && last != 2) *dir = -2;        //Left
    else if(dpad[3] && last != -2) *dir = 2;        //Right
}

int screenLimits(int dir){    //Allow to cross leds border
    if(dir == -2*W && snake[0]/W == 0)    //Up wall: X + max-Y 
        return snake[0]%W + (H-2)*W;    
    if(dir == 2*W && snake[0]/W == H-2)    //Down wall: X + min-Y
        return snake[0]%W;
    if(dir == -2 && snake[0]%W == 0)    //Left wall Y: + max-X
        return (snake[0]/W)*W + W-2;
    if(dir == 2 && snake[0]%W == W-2)    //Right wall: Y + min-X
        return (snake[0]/W)*W;

    return snake[0] + dir;    //Default
}

//Creation of apple by randomness
void summonApple(tail){
    int flag = 0;
    do{
        flag = 0;
        apple = (((rand()+randomness)%H)/2*2)*W + ((rand()+randomness)%W)/2*2;    //Generation of coords
        for(int i = 0; i <= tail; i++)    //Cycle to avoid apples over the snake
            flag += snake[i] == apple? 1: 0;
    }while(flag);
    drawSquare(apple, GREEN);    //Show apple
    summonWalls(tail);
}

//Creation of wall by randomness
void summonWalls(tail){
    for(int i = 0; i < 3; i++){
        drawSquare(walls[i], 0);
        int flag = 0;
        do{
            flag = 0;
            walls[i] = (((rand()+randomness)%H)/2*2)*W + ((rand()+randomness)%W)/2*2;    //Generation of coords
            for(int j = 0; j <= tail; j++)    //Cycle to avoid apples over the snake
                flag += snake[j] == walls[i]? 1: 0;
        }while(flag);
        drawSquare(walls[i], 0xFFFFFF);    //Show  wall
    }
}

//Draw the snake and update the coords of each part
int drawSnake(int tail, int dir){
    int next = snake[0];    //Save next part
    snake[0] = screenLimits(dir);    //Move snake
    if(snake[tail] >= 0)    //Avoid delete in case of eat an apple
        drawSquare(snake[tail], 0);    //Delete tail
    drawSquare(snake[0], RED);    //Draw head
    
    for(int i = 0; i < 3; i++)
        if(snake[0] == walls[i])
            return -1;
    
    for(int i = 1; i <= tail; i++){    //Update coords of snake
        next ^= snake[i];
        snake[i] ^= next;    //Swap
        next ^= snake[i];
        if(snake[0] == snake[i]) return -1;    //Exit if collision
    }
    
    if(snake[0] == apple){    //If apple is consumed
        summonApple(tail);
        tail++;
        points++;
        snake[tail] = -1;
    }
    
    return tail;
}

//Main :v
void main(void){
    //Initialization of snake and variables
    snake[0] = 0;    //Parts of snake
    snake[0] = 2;
    int tail = 1;    //Position of tail in array
    int dir = 0;    //Direction of snake
    int lastDir = 2;    //Avoid incorrect directions for spam
    int wait = 1000;    //Slow the movement and process
    
    for(int i = 0; i < H*W; i++)    //Clear leds
        drawSquare(i, 0);
    for(int i = 0; i <= tail; i++)    //Draw snake
        drawSquare(snake[i], RED);
    
    while(dir == 0) checkPad(&dir, lastDir);    //Wait button to start
    randomness += dir;
    summonApple(tail);
    
    while(1){    //Game loop
        checkPad(&dir, lastDir);
        if(wait++ < 1000) continue;    //Cronometer :v
        randomness += dir;
        lastDir = dir;
        wait = 0;
        tail = drawSnake(tail, dir);
        if(tail < 0) break;    //Game over
    }
    
    printf("Final score: %d apples", points);

}



    

