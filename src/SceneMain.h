#ifndef SCENE_MAIN_H
#define SCENE_MAIN_H

#include "Scene.h"
#include "Object.h"
#include <list>
#include <random>
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

    void spawEnemy(); //生成敌人
    void updateEnemies(float deltaTime);
    void renderEnemies();
    void shootEnemy(Enemy* enemy);
    SDL_FPoint getDirection(Enemy* enemy);//获取敌人朝向玩家的方向向量
    void updateEnemyProjectiles(float deltaTime);
    void renderEnemyProjectiles();
private:
    Game& game ;// = Game::getInstance();
    //不能在头文件中直接初始化单例对象，否则会导致循环依赖
    Player player;
    mt19937 gen;//随机数生成器
    uniform_real_distribution<float> dis;//生成随机数
    
    //创建子弹模板
    projectilePlayer projectilePlayerTemplate;
    list<projectilePlayer*> playerProjectiles;

    //创建敌人模板
    Enemy enemyTemplate;
    list<Enemy*> enemies;

    //创建敌机子弹模板
    projectileEnemy projectileEnemyTemplate;
    list<projectileEnemy*> enemyProjectiles;
};
#endif // SCENE_MAIN_H