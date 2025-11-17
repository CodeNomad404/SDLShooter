#ifndef OBJECT_H
#define OBJECT_H
#include <SDL.h>

struct Player{
    SDL_Texture* texture=nullptr;
    SDL_FPoint position={0,0}; //x,y
    int width=0;
    int height=0;
};
#endif