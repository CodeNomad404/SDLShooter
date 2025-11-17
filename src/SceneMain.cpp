#include "SceneMain.h"
#include "Game.h"
SceneMain::SceneMain():game(Game::getInstance())
{
}

void SceneMain::update()
{
}

void SceneMain::render()
{
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
}

void SceneMain::clean()
{
    //清理texture
    if(player.texture!=NULL)
    {
    SDL_DestroyTexture(player.texture);
    }
}

void SceneMain::handleEvents(SDL_Event *event)
{
}
