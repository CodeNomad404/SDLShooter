#ifndef GAME_H
#define GAME_H

#include "Scene.h"
#include "Object.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

class Game {
public:
    ~Game();
    
    static Game& getInstance(){
        static Game instance;
        return instance;
    };//单例模式

    void run();
    void init();
    void clean();
    void changeScene(Scene* scene);
    void handeleEvent(SDL_Event* event);
    void update(float deltaTime);
    void render();
    
    //getters
    SDL_Window* getWindow() {return window;}
    SDL_Renderer* getRenderer() {return renderer;}
    
    int getWindowWidth() const {return windowWidth;}
    int getWindowHeight() const {return windowHeight;}

    
private:
    Game();
    //删除拷贝构造函数和赋值运算符重载，防止拷贝
    Game(const Game& other)=delete;
    Game& operator=(const Game&)=delete;

    TTF_Font* titleFont;
    TTF_Font* textFont;

    bool isRunning=true;
    Scene* currentScene=nullptr;
    SDL_Renderer* renderer=nullptr;
    SDL_Window* window=nullptr;
    int windowWidth=600;
    int windowHeight=800;
    int FPS=60;
    Uint32 frameTime;
    float deltaTime;

    Background nearStars;
    Background farStars;

    void backgroundupdate(float deltaTime);
    void renderBackground();
};

#endif // GAME_H