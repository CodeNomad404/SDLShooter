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
    Uint32 coolDown=100; //子弹冷却时间
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

struct Enemy
{
    SDL_Texture* texture=nullptr;
    SDL_FPoint position={0,0}; //x,y
    int width=0;
    int height=0;
    int speed=200;
    Uint32 coolDown=1000; //子弹冷却时间
    Uint32 lastShootTime=0;
};

struct projectileEnemy//敌机子弹，自动跟踪玩家方向
{
    SDL_Texture* texture=nullptr;
    SDL_FPoint position={0,0}; //x,y
    SDL_FPoint direction={0,0}; //x,y
    int width=0;
    int height=0;
    int speed=600;
};
#endif