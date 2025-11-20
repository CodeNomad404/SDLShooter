#include "SceneMain.h"
#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <random>

SceneMain::SceneMain():game(Game::getInstance())
{
}

void SceneMain::update(float deltaTime)
{
    keyboardControl(deltaTime);
    updatePlayerProjectiles(deltaTime);
    updateEnemyProjectiles(deltaTime);
    spawEnemy();
    updateEnemies(deltaTime);
    updatePlayer(deltaTime);
    updateExplosions();
    updateItems(deltaTime);
}

void SceneMain::render()
{
    renderProjectiles();//先渲染子弹，防止飞机被遮挡
    if(!isDead)
    {
        SDL_Rect playerRect=
        {
            static_cast<int>(player.position.x),
            static_cast<int>(player.position.y),
            player.width,
            player.height
        };
    SDL_RenderCopy(game.getRenderer(),player.texture,NULL,&playerRect);
    }
    //渲染敌机
    renderEnemyProjectiles();
    renderEnemies();

    //渲染道具
    renderItems();

    renderExplosions();//最后渲染爆炸效果
}

void SceneMain::init()
{
    //获取敌机初始化位置
    std::random_device rd;
    gen=std::mt19937(rd());
    dis=std::uniform_real_distribution<float>(0.0f, 1.0f);
    auto r=dis(gen);

    player.texture=IMG_LoadTexture(game.getRenderer(),"assets/image/SpaceShip.png");
    SDL_QueryTexture(player.texture, NULL, NULL, &player.width, &player.height);
    //玩家飞机缩小
    player.width/=4;
    player.height/=4;
    //玩家飞机初始位置,在屏幕底部中间
    player.position.x=game.getWindowWidth()/2-player.width/2;
    player.position.y=game.getWindowHeight()-player.height;

    //初始化模板
    projectilePlayerTemplate.texture=IMG_LoadTexture(game.getRenderer(),"assets/image/laser-1.png");
    SDL_QueryTexture(projectilePlayerTemplate.texture, NULL, NULL, &projectilePlayerTemplate.width, &projectilePlayerTemplate.height);
    //子弹缩小
    projectilePlayerTemplate.width/=4;
    projectilePlayerTemplate.height/=4;

    //初始化敌机模板
    enemyTemplate.texture=IMG_LoadTexture(game.getRenderer(),"assets/image/insect-2.png");
    SDL_QueryTexture(enemyTemplate.texture, NULL, NULL, &enemyTemplate.width, &enemyTemplate.height);
    //敌机缩小
    enemyTemplate.width/=4;
    enemyTemplate.height/=4;

    //初始化敌机子弹模板
    projectileEnemyTemplate.texture=IMG_LoadTexture(game.getRenderer(),"assets/image/bullet-1.png");
    SDL_QueryTexture(projectileEnemyTemplate.texture, NULL, NULL, &projectileEnemyTemplate.width, &projectileEnemyTemplate.height);
    //敌机子弹缩小
    projectileEnemyTemplate.width/=4;
    projectileEnemyTemplate.height/=4;

    //初始化爆炸模板
    explosionTemplate.texture=IMG_LoadTexture(game.getRenderer(),"assets/effect/explosion.png");
    SDL_QueryTexture(explosionTemplate.texture, NULL, NULL, &explosionTemplate.width, &explosionTemplate.height);
    explosionTemplate.totalFrame=explosionTemplate.width/explosionTemplate.height;
    explosionTemplate.width=explosionTemplate.height;

    //初始化道具
    itemLifeTemplate.texture=IMG_LoadTexture(game.getRenderer(),"assets/image/bonus_life.png");
    SDL_QueryTexture(itemLifeTemplate.texture, NULL, NULL, &itemLifeTemplate.width, &itemLifeTemplate.height);
    itemLifeTemplate.width/=4;
    itemLifeTemplate.height/=4;
}

void SceneMain::clean()
{
    //清理玩家子弹
    for(auto& projectile : playerProjectiles)
    {
        if(projectile!=nullptr)
        {
            delete projectile;
        }
    }
    playerProjectiles.clear();

    //清理敌机
    for(auto& enemy : enemies)
    {
        if(enemy!=nullptr)
        {
            delete enemy;
        }
    }
    enemies.clear();

    //清理敌机子弹
    for(auto& enemyProjectile : enemyProjectiles)
    {
        if(enemyProjectile!=nullptr)
        {
            delete enemyProjectile;
        }
    }
    enemyProjectiles.clear();

    //清理爆炸
    for(auto& explosion : explosions)
    {
        if(explosion!=nullptr)
        {
            delete explosion;
        }
    }
    explosions.clear();

    //清理道具
    for(auto& item : items)
    {
        if (item!=nullptr)
        {
            delete item;
        }
        
    }

    //清理texture
    if(player.texture!=NULL)
    {
        SDL_DestroyTexture(player.texture);
    }

    if(projectilePlayerTemplate.texture!=NULL)
    {
        SDL_DestroyTexture(projectilePlayerTemplate.texture);
    }

    if(enemyTemplate.texture!=NULL)
    {
        SDL_DestroyTexture(enemyTemplate.texture);
    }

    if(projectileEnemyTemplate.texture!=NULL)
    {
        SDL_DestroyTexture(projectileEnemyTemplate.texture);
    }

    if(explosionTemplate.texture!=NULL)
    {
        SDL_DestroyTexture(explosionTemplate.texture);
    }

    if(itemLifeTemplate.texture!=NULL)
    {
        SDL_DestroyTexture(itemLifeTemplate.texture);
    }
}

void SceneMain::handleEvents(SDL_Event *event)
{
}

void SceneMain::keyboardControl(float deltaTime)
{
    if(isDead){
        return;
    }
    auto keyBoardStates=SDL_GetKeyboardState(NULL);
    if(keyBoardStates[SDL_SCANCODE_W])
    {
        player.position.y-=deltaTime*player.speed;
    }
    if(keyBoardStates[SDL_SCANCODE_S])
    {
        player.position.y+=deltaTime*player.speed;
    }
    if(keyBoardStates[SDL_SCANCODE_A])
    {
        player.position.x-=deltaTime*player.speed;
    }
    if(keyBoardStates[SDL_SCANCODE_D])
    {
        player.position.x+=deltaTime*player.speed;
    }

    if(player.position.x<0)
        player.position.x=0;
    if(player.position.x>game.getWindowWidth()-player.width)
        player.position.x=game.getWindowWidth()-player.width;
    if(player.position.y<0)
        player.position.y=0;
    if(player.position.y>game.getWindowHeight()-player.height)
        player.position.y=game.getWindowHeight()-player.height;

    if(keyBoardStates[SDL_SCANCODE_J])
    {
        Uint32 currentTime=SDL_GetTicks();
        if(currentTime - player.lastShootTime >= player.coolDown)
        {
            //发射子弹的代码
            shootPlayer();
            //可以发射子弹
            player.lastShootTime=currentTime;
        }
    }
}

void SceneMain::shootPlayer()
{
    auto projectile=new projectilePlayer(projectilePlayerTemplate);
    projectile->position.x=player.position.x + player.width / 2 - projectile->width / 2;
    projectile->position.y=player.position.y ;
    playerProjectiles.push_back(projectile);
}

void SceneMain::updatePlayerProjectiles(float deltaTime)
{
    for(auto it=playerProjectiles.begin();it!=playerProjectiles.end();)
    {
        auto& projectile = *it;
        projectile->position.y -= projectile->speed * deltaTime;
        //如果子弹飞出屏幕，则删除
        if(projectile->position.y + projectile->height < 0)
        {
            delete projectile;
            it=playerProjectiles.erase(it);
        }
        else
        {
            bool hit=false;
            for(auto enemy : enemies)
            {
                SDL_Rect enemyRect={
                    static_cast<int>(enemy->position.x),
                    static_cast<int>(enemy->position.y),
                    enemy->width,
                    enemy->height
                };
                SDL_Rect projectileRect={
                    static_cast<int>(projectile->position.x),
                    static_cast<int>(projectile->position.y),
                    projectile->width,
                    projectile->height
                };
                if(SDL_HasIntersection(&enemyRect,&projectileRect))
                {
                    enemy->currentHealth -= projectile->damage;
                    delete projectile;
                    it=playerProjectiles.erase(it);
                    hit=true;
                    break;
                }
            }
            if(!hit)
            {
                ++it;
            }
        }
    }
}

void SceneMain::renderProjectiles()
{
    for(auto projectile : playerProjectiles)
    {
        SDL_Rect projectileRect=
        {
            static_cast<int>(projectile->position.x),
            static_cast<int>(projectile->position.y),
            projectile->width,
            projectile->height
        };
        SDL_RenderCopy(game.getRenderer(),projectile->texture,NULL,&projectileRect);
    }
}

void SceneMain::spawEnemy()
{
    if(dis(gen)>1/60.0f)
    {
        return;
    }

    //生成敌机
    Enemy* enemy=new Enemy(enemyTemplate);
    enemy->position.x=dis(gen)*(game.getWindowWidth()-enemy->width);
    enemy->position.y=-enemy->height; //从屏幕上方出现
    enemies.push_back(enemy);
}

void SceneMain::updateEnemies(float deltaTime)
{
    for(auto it=enemies.begin();it!=enemies.end();)
    {
        auto& enemy=*it;
        enemy->position.y+=enemy->speed*deltaTime;
        if(enemy->position.y>game.getWindowHeight())
        {
            delete enemy;
            it=enemies.erase(it);
        }
        else
        {
            auto currentTime=SDL_GetTicks();
            if(currentTime-enemy->lastShootTime>enemy->coolDown&&!isDead)
            {
                //敌机发射子弹
                shootEnemy(enemy);
                enemy->lastShootTime=currentTime;
            }

            if(enemy->currentHealth<=0)
            {
                enemyExplode(enemy);
                it=enemies.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}

void SceneMain::updatePlayer(float deltaTime)
{
    if(isDead){
        return;
    }
    if(player.currentHealth<=0)
    {
        isDead=true;
        auto currentTime=SDL_GetTicks();
        auto explosion=new Explosion(explosionTemplate);
        explosion->position.x=player.position.x+player.width/2-explosion->width/2;
        explosion->position.y=player.position.y+player.height/2-explosion->height/2;
        explosion->startTime=currentTime;
        explosions.push_back(explosion);
    }
    for(auto& enemy:enemies)
    {
        SDL_Rect enemyRect={
            static_cast<int>(enemy->position.x),
            static_cast<int>(enemy->position.y),
            enemy->width,
            enemy->height
        };
        SDL_Rect playerRect={
            static_cast<int>(player.position.x),
            static_cast<int>(player.position.y),
            player.width,
            player.height
        };
        if(SDL_HasIntersection(&enemyRect,&playerRect)&&!isDead)//检测碰撞
        {
            player.currentHealth-=1;
            enemy->currentHealth=0;
        }
    }
}

void SceneMain::updateExplosions()
{
    Uint32 currentTime=SDL_GetTicks();
    for(auto it=explosions.begin();it!=explosions.end();)
    {
        auto& explosion = *it;
        //计算当前帧
        explosion->currentFrame=(currentTime - explosion->startTime)/(1000/explosion->FPS);
        if(explosion->currentFrame>=explosion->totalFrame)
        {
            //爆炸结束，删除
            delete explosion;
            it=explosions.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void SceneMain::updateItems(float deltaTime)
{
    for(auto it=items.begin();it!=items.end();)
    {
        auto& item = *it;
        item->position.x+=item->direction.x*item->speed*deltaTime;
        item->position.y+=item->direction.y*item->speed*deltaTime;
        //处理屏幕反弹
        if(item->position.x<0&&item->bounceCount>0)
        {
            item->direction.x=-item->direction.x;
            item->bounceCount--;
        }
        else if(item->position.x+item->width>game.getWindowWidth()&&item->bounceCount>0)
        {
            item->direction.x=-item->direction.x;
            item->bounceCount--;
        }
        if(item->position.y<0&&item->bounceCount>0)
        {
            item->direction.y=-item->direction.y;
            item->bounceCount--;
        }
        else if(item->position.y+item->height>game.getWindowHeight()&&item->bounceCount>0)
        {
            item->direction.y=-item->direction.y;
            item->bounceCount--;
        }
        //检测是否超出屏幕
        if
        (
            item->position.y>game.getWindowHeight()||
            item->position.x+item->width<0||
            item->position.x>game.getWindowWidth()||
            item->position.y+item->height<0
        )
        {
            delete item;
            it=items.erase(it);
        }
        else
        {
            SDL_Rect itemRect=
            {
                static_cast<int>(item->position.x),
                static_cast<int>(item->position.y),
                item->width,
                item->height
            };

            SDL_Rect playerRect=
            {
                static_cast<int>(player.position.x),
                static_cast<int>(player.position.y),
                player.width,
                player.height
            };
            //检测碰撞
            if(SDL_HasIntersection(&itemRect,&playerRect))
            {
                //玩家获得道具
                playerGetItem(item);
                delete item;
                it=items.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}

void SceneMain::renderEnemies()
{
    for(auto enemy : enemies)
    {
        SDL_Rect enemyRect=
        {
            static_cast<int>(enemy->position.x),
            static_cast<int>(enemy->position.y),
            enemy->width,
            enemy->height
        };
        SDL_RenderCopy(game.getRenderer(),enemy->texture,NULL,&enemyRect);
    }
}

void SceneMain::shootEnemy(Enemy* enemy)
{   
    auto projectile=new projectileEnemy(projectileEnemyTemplate);
    projectile->position.x=enemy->position.x+enemy->width/2-projectile->width/2;
    projectile->position.y=enemy->position.y+enemy->height-projectile->height/2;
    //计算方向向量，指向玩家位置
    projectile->direction=getDirection(enemy);
    enemyProjectiles.push_back(projectile);
}

SDL_FPoint SceneMain::getDirection(Enemy *enemy)
{
    auto x=player.position.x+player.width/2-enemy->position.x-enemy->width/2;
    auto y=player.position.y+player.height/2-enemy->position.y-enemy->height/2;
    auto length=std::sqrt(x*x+y*y);
    x=x/length;
    y=y/length;
    return SDL_FPoint{x,y};
}

void SceneMain::updateEnemyProjectiles(float deltaTime)
{
    for(auto it=enemyProjectiles.begin();it!=enemyProjectiles.end();)
    {
        auto& projectile = *it;
        projectile->position.x += projectile->direction.x * projectile->speed * deltaTime;
        projectile->position.y += projectile->direction.y * projectile->speed * deltaTime;
        //如果子弹飞出屏幕，则删除
        if(projectile->position.y > game.getWindowHeight() ||
           projectile->position.x < 0 ||
           projectile->position.x > game.getWindowWidth()||
           projectile->position.y < 0)
        {
            delete projectile;
            it=enemyProjectiles.erase(it);
        }
        else
        {
            SDL_Rect playerRect={
                static_cast<int>(player.position.x),
                static_cast<int>(player.position.y),
                player.width,
                player.height
            };
            SDL_Rect projectileRect={
                static_cast<int>(projectile->position.x),
                static_cast<int>(projectile->position.y),
                projectile->width,
                projectile->height
            };
            if(SDL_HasIntersection(&playerRect,&projectileRect)&&!isDead)//检测碰撞
            {
                
                player.currentHealth-=projectile->damage;
                delete projectile;
                it=enemyProjectiles.erase(it);//删除子弹
            }
            else
            {
                ++it;
            }
        }
    }
}

void SceneMain::renderEnemyProjectiles()
{
    for(auto projectile : enemyProjectiles)
    {
        SDL_Rect projectileRect=
        {
            static_cast<int>(projectile->position.x),
            static_cast<int>(projectile->position.y),
            projectile->width,
            projectile->height
        };
        //SDL_RenderCopy(game.getRenderer(),projectile->texture,NULL,&projectileRect);
        float angle = atan2(projectile->direction.y,projectile->direction.x) * 180 / M_PI-90.0f;
        SDL_RenderCopyEx(game.getRenderer(),projectile->texture,NULL,&projectileRect,angle,NULL,SDL_FLIP_NONE);
    }
}

void SceneMain::renderExplosions()
{
    for(auto explosion : explosions)
    {
        SDL_Rect src=
        {
            explosion->currentFrame * explosion->width,
            0,
            explosion->width,
            explosion->height
        };
        SDL_Rect dst=
        {
            static_cast<int>(explosion->position.x),
            static_cast<int>(explosion->position.y),
            explosion->width,
            explosion->height
        };
        SDL_RenderCopy(game.getRenderer(),explosion->texture,&src,&dst);
    }
}

void SceneMain::renderItems()
{
    for(auto item : items)
    {
        SDL_Rect itemRect=
        {
            static_cast<int>(item->position.x),
            static_cast<int>(item->position.y),
            item->width,
            item->height
        };
        
        SDL_RenderCopy(game.getRenderer(),item->texture,NULL,&itemRect);
    }
}

void SceneMain::enemyExplode(Enemy *enemy)
{
    auto currentTime = SDL_GetTicks();
    auto explosion=new Explosion(explosionTemplate);
    explosion->position.x=enemy->position.x+enemy->width/2-explosion->width/2;
    explosion->position.y=enemy->position.y+enemy->height/2-explosion->height/2;
    explosion->startTime=currentTime;
    explosions.push_back(explosion);
    if(dis(gen)<0.5f) //50%概率掉落道具
    dropItem(enemy);
    delete enemy;
}

void SceneMain::dropItem(Enemy *enemy)
{
    auto item=new Item(itemLifeTemplate);
    item->position.x=enemy->position.x+enemy->width/2-item->width/2;
    item->position.y=enemy->position.y+enemy->height/2-item->height/2;
    float angle=dis(gen)*2*M_PI;
    item->direction.x=cos(angle); //随机方向
    item->direction.y=sin(angle);
    items.push_back(item);
}

void SceneMain::playerGetItem(Item* item)
{
    if(item->type==ItemType::Life)
    {
        player.currentHealth++;
        if(player.currentHealth>player.maxHealth)
        {
            player.currentHealth=player.maxHealth;
        }
    }
}
