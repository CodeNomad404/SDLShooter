#include "SceneMain.h"
#include "Game.h"

SceneMain::SceneMain():game(Game::getInstance())
{
}

void SceneMain::update(float deltaTime)
{
    keyboardControl(deltaTime);
    updateProjectiles(deltaTime);
}

void SceneMain::render()
{
    renderProjectiles();//先渲染子弹，防止飞机被遮挡
    SDL_Rect playerRect={
        static_cast<int>(player.position.x),
        static_cast<int>(player.position.y),
        player.width,
        player.height};
    SDL_RenderCopy(game.getRenderer(),player.texture,NULL,&playerRect);
}

void SceneMain::init()
{
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
}

void SceneMain::clean()
{
    for(auto& projectile : playerProjectiles)
    {
        if(projectile!=nullptr)
        {
            delete projectile;
        }
    }
    playerProjectiles.clear();
    //清理texture
    if(player.texture!=NULL)
    {
        SDL_DestroyTexture(player.texture);
    }
    if(projectilePlayerTemplate.texture!=NULL)
    {
        SDL_DestroyTexture(projectilePlayerTemplate.texture);
    }
}

void SceneMain::handleEvents(SDL_Event *event)
{
}

void SceneMain::keyboardControl(float deltaTime)
{
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

void SceneMain::updateProjectiles(float deltaTime)
{
    for(auto it=playerProjectiles.begin();it!=playerProjectiles.end();)
    {
        auto& projectile = *it;
        projectile->position.y -= projectile->speed * deltaTime;
        //如果子弹飞出屏幕，则删除
        if(projectile->position.y + projectile->height < 0)
        {
            
        }
        ++it;
        
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
