#include "SceneEnd.h"
#include "Game.h"
#include "SceneMain.h"
#include <string>

void SceneEnd::init()
{
    if(!SDL_IsTextInputActive())
    {
        SDL_StartTextInput();
        if(!SDL_IsTextInputActive())
        {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to start text input: %s", SDL_GetError());
        }
    }
}

void SceneEnd::update(float deltaTime)
{
    blinkTimer-=deltaTime;
    if(blinkTimer<=0.0f){
        blinkTimer+=1.0f;
    }
}

void SceneEnd::render()
{
    if(isTyping)
    {
       renderPhase1();
    } 
    else
    {
       renderPhase2();
    }
}

void SceneEnd::clean()
{
}

void SceneEnd::handleEvents(SDL_Event *event)
{
    if(isTyping)
    {
        if(event->type==SDL_TEXTINPUT)
        {
            name+=event->text.text;
        }
        if(event->type==SDL_KEYDOWN)
        {
            if(event->key.keysym.scancode==SDL_SCANCODE_RETURN)
            {
                isTyping=false;
                SDL_StopTextInput();
                if(name=="")
                {
                    name="NoName";
                }
                game.insertLeaderBoard(name,game.getFinalScore());
            }
            if(event->key.keysym.scancode==SDL_SCANCODE_BACKSPACE)
            {
                removeLastUTFChar(name);
            }
        }
    }
    else
    {
        if(event->type==SDL_KEYDOWN)
        {
            if(event->key.keysym.scancode==SDL_SCANCODE_J)
            {
                auto sceneMian=new SceneMain();
                game.changeScene(sceneMian);
            }
        }
    }
}

void SceneEnd::renderPhase1()
{
    auto score=game.getFinalScore();
    std::string scoreTest="Your Score:"+std::to_string(score);
    std::string gameOver="Game Over";
    std::string instructionText="Enter Your Name";
    game.renderTextCentered(scoreTest,0.1,false);
    game.renderTextCentered(gameOver,0.4,true);
    game.renderTextCentered(instructionText,0.6,false);

    if(name.length()>0)
    {
        SDL_Point p=game.renderTextCentered(name,0.8,false);
        if(blinkTimer<0.5f){
            game.renderTextPos("_",p.x,p.y);
        }
    }
    else
    {
        if(blinkTimer<0.5f){
            game.renderTextCentered("_",0.8,false);
        }
    }
}

void SceneEnd::renderPhase2()
{
    game.renderTextCentered("得分榜",0.1,true);
    auto posY=0.2*game.getWindowHeight();
    auto i=1;
    for(auto item : game.getLeaderBoard())
    {
        std::string name =std::to_string(i)+". "+item.second;
        std::string score=std::to_string(item.first);
        game.renderTextPos(name,100,posY);
        game.renderTextPos(score,100,posY,false);
        posY+=40;
        ++i;
    }
    game.renderTextCentered("按 J 键重新开始游戏",0.85,false);
}

void SceneEnd::removeLastUTFChar(std::string& str)
{ 
    if(str.empty()) return;

    auto lastchar=str.back();
    if((lastchar & 0b10000000)== 0b10000000)//中文字符的后续字节
    {
        str.pop_back();
        while((str.back() & 0b11000000)!= 0b11000000)//找到中文字符的开始字节
        {
            str.pop_back();
        }
    }
    str.pop_back();
    
}
