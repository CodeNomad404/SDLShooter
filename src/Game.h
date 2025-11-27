#ifndef GAME_H
#define GAME_H

#include "Scene.h"
#include "Object.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>
#include <map>

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

    //渲染工具函数
    SDL_Point renderTextCentered(std::string text, float posY,bool isTitle);
    void renderTextPos(std::string text, int posX, int posY, bool isLeft=true);

    //setters
    void setFinalScore(int score){finalScore=score;}
    
    //getters

    int getFinalScore(){return finalScore;}

    SDL_Window* getWindow() {return window;}
    SDL_Renderer* getRenderer() {return renderer;}
    
    int getWindowWidth() const {return windowWidth;}
    int getWindowHeight() const {return windowHeight;}

    std::multimap<int, std::string,std::greater<int>>& getLeaderBoard(){ return leaderBoard; };

    void insertLeaderBoard(std::string name, int score);
private:
    Game();
    //删除拷贝构造函数和赋值运算符重载，防止拷贝
    Game(const Game& other)=delete;
    Game& operator=(const Game&)=delete;

    TTF_Font* titleFont;
    TTF_Font* textFont;

    bool isRunning=true;
    bool isFullscreen=false;
    Scene* currentScene=nullptr;
    SDL_Renderer* renderer=nullptr;
    SDL_Window* window=nullptr;
    int windowWidth=600;
    int windowHeight=800;
    int FPS=60;
    Uint32 frameTime;
    float deltaTime;
    int finalScore=0;

    Background nearStars;
    Background farStars;

    std::multimap<int, std::string,std::greater<int>> leaderBoard; //排行榜

    void backgroundupdate(float deltaTime);
    void renderBackground();
    void saveData();
    void loadData();
};

#endif // GAME_H