#ifndef OBJECT_H
#define OBJECT_H
#include <SDL.h>

struct Player
{
    SDL_Texture* texture=nullptr;
    SDL_FPoint position={0,0}; //x,y
    int width=0;
    int height=0;
    int speed=400;
    Uint32 coolDown=500; //子弹冷却时间
    Uint32 lastShootTime=0;
};

struct projectilePlayer
{
    SDL_Texture* texture=nullptr;
    SDL_FPoint position={0,0}; //x,y
    int width=0;
    int height=0;
    int speed=800;
};
#endif