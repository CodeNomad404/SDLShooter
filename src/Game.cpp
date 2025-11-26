#include "Game.h"
#include "SceneMain.h"
#include "SceneTitle.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

Game::Game()
{
}

Game::~Game()
{
    clean();
}

void Game::run()
{
    while(isRunning)
    {
        Uint32 frameStart = SDL_GetTicks();
        SDL_Event event;

        handeleEvent(&event);

        update(deltaTime);
        render();
        Uint32 frameEnd = SDL_GetTicks();
        Uint32 diff = frameEnd - frameStart;
        if(diff < frameTime)
        {
            SDL_Delay(frameTime - diff);
            deltaTime = frameTime / 1000.0f;
        }
        else{
            deltaTime = diff/1000.0f;
        }
    }
}

void Game::init()
{
    frameTime = 1000/FPS;
    //初始化
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Unable to initialize SDL: %s", SDL_GetError());
        isRunning = false;
    }

    //创建窗口
    window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if(window == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Window could not be created: %s", SDL_GetError());
        isRunning = false;
    }

    //创建渲染器
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Renderer could not be created: %s", SDL_GetError());
        isRunning = false;
    }
    //初始化SDL_image
    if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"SDL_image could not be initialized: %s", SDL_GetError());
        isRunning = false;
    }

    //初始化SDL_mixer
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_mixer could not initialize! Error: %s", Mix_GetError());
        isRunning = false;
    }
    //设置音效channel数量
    Mix_AllocateChannels(32);

    //设置音乐音量
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4); //设置音乐音量
    Mix_Volume(-1, MIX_MAX_VOLUME / 8); //设置音效音量

    //初始化SDL_ttf
    if(TTF_Init() == -1)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_ttf could not initialize! Error: %s", TTF_GetError());
        isRunning = false;
    }

    //创建场景
    nearStars.texture=IMG_LoadTexture(renderer,"assets/image/Stars-A.png");
    if(nearStars.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Failed to load texture: %s", SDL_GetError());
        isRunning=false;
    }
    SDL_QueryTexture(nearStars.texture, NULL,NULL, &nearStars.width, &nearStars.height);
    nearStars.width/=2;
    nearStars.height/=2;
    
    farStars.texture=IMG_LoadTexture(renderer,"assets/image/Stars-B.png");
    if(farStars.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Failed to load texture: %s", SDL_GetError());
        isRunning=false;
    }
    SDL_QueryTexture(farStars.texture, NULL,NULL, &farStars.width, &farStars.height);
    farStars.speed=20;
    farStars.width/=2;
    farStars.height/=2;

    //载入字体
    titleFont= TTF_OpenFont("assets/font/VonwaonBitmap-16px.ttf", 64);
    textFont= TTF_OpenFont("assets/font/VonwaonBitmap-16px.ttf", 32);
    if(titleFont == nullptr || textFont == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_ttf could not initialize! Error: %s", TTF_GetError());
        isRunning = false;
    }

    currentScene = new SceneTitle();
    currentScene->init();
}

void Game::clean()
{
    if(currentScene != nullptr)
    {
        currentScene->clean();
        delete currentScene;
        currentScene = nullptr;  // 设置为nullptr
    }

    if(nearStars.texture != nullptr)
    {
        SDL_DestroyTexture(nearStars.texture);
        nearStars.texture = nullptr;
    }
    if(farStars.texture != nullptr)
    {
        SDL_DestroyTexture(farStars.texture);
        farStars.texture = nullptr;
    }
    if(titleFont != nullptr)
    {
        TTF_CloseFont(titleFont);
        titleFont = nullptr;
    }
    if(textFont != nullptr)
    {
        TTF_CloseFont(textFont);
        textFont = nullptr;
    }

    //退出SDL_image
    IMG_Quit();
    if(renderer != nullptr)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if(window != nullptr)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    //清理SDL_mixer
    Mix_CloseAudio();
    Mix_Quit();
    //清理SDL_ttf
    TTF_Quit();
    //退出SDL
    SDL_Quit();
}

void Game::changeScene(Scene *scene)
{
    if(currentScene)
    {
        currentScene->clean();
        delete currentScene;
    }
    currentScene = scene;
    currentScene->init();
}

void Game::handeleEvent(SDL_Event *event)
{
    while(SDL_PollEvent(event))
        {
            if(event->type == SDL_QUIT)
            {
                isRunning = false;
            }
        currentScene->handleEvents(event);
        }
}

void Game::update(float deltaTime)
{
    backgroundupdate(deltaTime);
    currentScene->update(deltaTime);
}

void Game::render()
{
    //清空屏幕
    SDL_RenderClear(renderer);

    //渲染场景
    renderBackground();

    currentScene->render();
    
    //更新屏幕
    SDL_RenderPresent(renderer);
}

void Game::renderTextCentered(std::string text, float posY, bool isTitle)
{
    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface *surface = nullptr;
    if(isTitle)
    {
        surface = TTF_RenderText_Solid(titleFont, text.c_str(), color);
    }
    else
    {
        surface = TTF_RenderText_Solid(textFont, text.c_str(), color);
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    int y=static_cast<int>(getWindowHeight()-surface->h)*posY;
    SDL_Rect rect=
    {
        static_cast<int>(getWindowWidth()/2 - surface->w/2),
        y,
        surface->w,
        surface->h
    };
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void Game::backgroundupdate(float deltaTime)
{
    nearStars.offset+=nearStars.speed*deltaTime;
    if(nearStars.offset>=0)
    {
        nearStars.offset-=nearStars.height;
    }

    farStars.offset+=farStars.speed*deltaTime;
    if(farStars.offset>=0)
    {
        farStars.offset-=farStars.height;
    }
}

void Game::renderBackground()
{
    //渲染远处的星星
    for(int posY=static_cast<int>(farStars.offset); posY<getWindowHeight(); posY+=farStars.height)
    {
        for(int posX=0; posX<getWindowWidth(); posX+=farStars.width)
        {
            SDL_Rect destRect={posX,posY,farStars.width, farStars.height};
            SDL_RenderCopy(renderer, farStars.texture, NULL, &destRect);
        }
    }

    //渲染近处的星星
    for(int posY=static_cast<int>(nearStars.offset); posY<getWindowHeight(); posY+=nearStars.height)
    {
        for(int posX=0; posX<getWindowWidth(); posX+=nearStars.width)
        {
            SDL_Rect destRect={posX,posY,nearStars.width,nearStars.height};
            SDL_RenderCopy(renderer,nearStars.texture,NULL,&destRect);
        }

    }
}
