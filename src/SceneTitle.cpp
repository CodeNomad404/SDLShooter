#include "SceneTitle.h"

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
}

void SceneTitle::render()
{
}

void SceneTitle::clean()
{
}

void SceneTitle::handleEvents(SDL_Event *event)
{
}
