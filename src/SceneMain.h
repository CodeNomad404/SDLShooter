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
    void updatePlayerProjectiles(float deltaTime);//更新玩家子弹
    void updateEnemyProjectiles(float deltaTime);//更新敌人子弹
    void updateEnemies(float deltaTime);//更新敌人
    void updatePlayer(float deltaTime);//更新玩家
    void updateExplosions();//更新爆炸效果
    void updateItems(float deltaTime);//更新道具

    void render() override;
    void renderProjectiles();//渲染子弹
    void renderEnemies();//渲染敌人
    void renderEnemyProjectiles();//渲染敌人子弹
    void renderExplosions();//渲染爆炸效果
    void renderItems();//渲染道具

    void init() override;
    void clean() override;
    void handleEvents(SDL_Event* event) override;

    void keyboardControl(float deltaTime);

    void shootPlayer();//玩家射击

    void spawEnemy(); //生成敌人
    void shootEnemy(Enemy* enemy);
    SDL_FPoint getDirection(Enemy* enemy);//获取敌人朝向玩家的方向向量
    void enemyExplode(Enemy* enemy);//敌机爆炸
    void dropItem(Enemy* enemy);//敌机掉落道具
    void playerGetItem(Item* item);//玩家获取道具
private:
    Game& game ;// = Game::getInstance();
    //不能在头文件中直接初始化单例对象，否则会导致循环依赖
    Player player;
    bool isDead= false;
    
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

    //创建爆炸模板
    Explosion explosionTemplate;
    list<Explosion*> explosions;

    //创建道具模板
    Item itemLifeTemplate; //生命道具
    Item itemSpeedTemplate; //速度道具
    Item itemShieldTemplate; //护盾道具
    list<Item*> items;
};
#endif // SCENEMAIN_H