#ifndef OBJECT_H
#define OBJECT_H
#include <SDL.h>

enum class ItemType
{
    Life,
    Shield,
    Time
};

struct Player
{
    SDL_Texture* texture=nullptr;
    SDL_FPoint position={0,0}; //x,y
    int width=0;
    int height=0;
    int speed=400;
    int maxHealth=3;
    int currentHealth=3;
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
    int damage=1;
};

struct Enemy
{
    SDL_Texture* texture=nullptr;
    SDL_FPoint position={0,0}; //x,y
    int width=0;
    int height=0;
    int speed=150;
    int currentHealth=2;
    Uint32 coolDown=2000; //子弹冷却时间
    Uint32 lastShootTime=0;
};

struct projectileEnemy//敌机子弹，自动跟踪玩家方向
{
    SDL_Texture* texture=nullptr;
    SDL_FPoint position={0,0}; //x,y
    SDL_FPoint direction={0,0}; //x,y
    int width=0;
    int height=0;
    int speed=300;
    int damage=1;
};

struct Explosion
{
    SDL_Texture* texture=nullptr;
    SDL_FPoint position={0,0}; //x,y
    int width=0;
    int height=0;
    int totalFrame=0; //总帧数
    int currentFrame=0; //当前帧
    Uint32 startTime=0; //爆炸开始时间
    Uint32 FPS=10; //帧率
};

struct Item//道具
{
    SDL_Texture* texture=nullptr;
    SDL_FPoint position={0,0}; //x,y
    SDL_FPoint direction={0,0}; //方向
    int width=0;
    int height=0;
    int speed=200;
    int bounceCount=3; //反弹次数
    ItemType type=ItemType::Life; //道具类型
};

struct Background
{
    SDL_Texture* texture=nullptr;
    SDL_FPoint position1={0,0}; //第一张图位置
    SDL_FPoint position2={0,-800}; //第二张图位置
    float offset=0; //滚动偏移量
    int width=0;
    int height=0;
    int speed=30; //滚动速度
};

struct MyPayment
{
    SDL_Texture* texture=nullptr;
    SDL_FPoint position={0,0}; //x,y
    int width=0;
    int height=0;
};
#endif