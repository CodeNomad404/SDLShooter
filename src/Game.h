#ifndef GAME_H
#define GAME_H
#include "Scene.h"
class Game {
public:
    Game();
    ~Game();

    void run();
    void init();
    void clean();
    void changeScene(Scene* scene);
    void handeleEvent(SDL_Event* event);
    void update();
    void render();
private:
    bool isRunning=true;
    Scene* currentScene=nullptr;
    SDL_Renderer* renderer=nullptr;
    SDL_Window* window=nullptr;
    int windowWidth=600;
    int windowHeight=800;
};

#endif // GAME_H