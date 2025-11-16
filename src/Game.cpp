#include "Game.h"
#include "Scene.h"
#include "SceneMain.h"
#include <SDL.h>
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
        SDL_Event event;

        handeleEvent(&event);

        update();
        render();
    }
}

void Game::init()
{
    //初始化
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Unable to initialize SDL: %s", SDL_GetError());
        isRunning = false;
        return;
    }

    //创建窗口
    window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if(window == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Window could not be created: %s", SDL_GetError());
        isRunning = false;
        return;
    }

    //创建渲染器
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Renderer could not be created: %s", SDL_GetError());
        isRunning = false;
        return;
    }

    //创建场景
    currentScene = new SceneMain();
}

void Game::clean()
{
    if(currentScene != nullptr)
    {
        currentScene->clean();
        delete currentScene;
        currentScene = nullptr;  // 设置为nullptr
    }
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

void Game::update()
{
    currentScene->update();
}

void Game::render()
{
    //清空屏幕
       SDL_RenderClear(renderer);

       currentScene->render();
       //更新屏幕
       SDL_RenderPresent(renderer);
}
