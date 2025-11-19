#ifndef SCENE_MAIN_H
#define SCENE_MAIN_H

#include "Scene.h"
#include "Object.h"
#include<list>
using namespace std;

class Game;//不能包含Game.h，否则会循环依赖，在cpp文件中包含即可

class SceneMain : public Scene {
public:
    SceneMain();
    ~SceneMain();

    void update(float deltaTime) override;
    void render() override;
    void init() override;
    void clean() override;
    void handleEvents(SDL_Event* event) override;

    void keyboardControl(float deltaTime);
    void shootPlayer();

    void updateProjectiles(float deltaTime);
    void renderProjectiles();
private:
    Game& game ;// = Game::getInstance();
    //不能在头文件中直接初始化单例对象，否则会导致循环依赖
    Player player;
    projectilePlayer projectilePlayerTemplate;
    list<projectilePlayer*> playerProjectiles;
};
#endif // SCENE_MAIN_H