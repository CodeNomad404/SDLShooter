#include "SceneTitle.h"
#include "SceneMain.h"
#include "Game.h"
#include <string>

void SceneTitle::init()
{
    //读取并播放背景音乐
    bgm=Mix_LoadMUS("assets/music/06_Battle_in_Space_Intro.ogg");
    if(bgm==nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"Mix_LoadMUS Error:%s",Mix_GetError());
    }
    Mix_PlayMusic(bgm,-1);
}

void SceneTitle::update(float deltaTime)
{
    timer+=deltaTime;
    //每隔0.5秒切换一次背景音乐，制造节奏感
    if(timer>=1.0f)
    {
        timer=0.0f;
    }
}

void SceneTitle::render()
{
    //渲染标题文字
    std::string titleText="Space Invaders";
    game.renderTextCentered(titleText,0.4,true);
    //渲染普通文字  
    if(timer<0.5f)
    {
    std::string normalText="Press J to Start";
    game.renderTextCentered(normalText,0.8,false);
    }
}

void SceneTitle::clean()
{
    if(bgm!=nullptr)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(bgm);
    }
}

void SceneTitle::handleEvents(SDL_Event *event)
{
    if(event->type==SDL_KEYDOWN)
    {
        if(event->key.keysym.scancode==SDL_SCANCODE_J)
        {
            auto sceneMain=new SceneMain();
            game.changeScene(sceneMain);
        }
    }
}
