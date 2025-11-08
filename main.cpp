#include<iostream>
#include<SDL.h>
#include<SDL_image.h>
using namespace std;

int main(int, char**) {
    // 初始化SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) 
    {
        cerr << "SDL_Init Error: " << SDL_GetError() << endl;
        return 1;
    }

    // 创建窗口
    SDL_Window *win = SDL_CreateWindow("Hello SDL2", 100, 100, 800, 600, SDL_WINDOW_SHOWN);
    // 创建渲染器
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    //初始化SDL_image
    if(IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG)  != IMG_INIT_PNG|IMG_INIT_JPG)
    {
        cerr << "IMG_Init Error: " << IMG_GetError() << endl;
        return 1;
    }

    // 加载图片
    SDL_Texture *tex = IMG_LoadTexture(ren, "image.png");



    //循环渲染
    while(true) {
        SDL_Event event;
        if(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                break;
        }
        }

        // 清屏
        SDL_RenderClear(ren);

        //画一个长方形
        SDL_Rect rect = {100, 100, 200, 200};
        SDL_SetRenderDrawColor(ren, 255, 255, 0, 255);
        SDL_RenderFillRect(ren, &rect);
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderPresent(ren);
    }
    //清理并关闭
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
cout<<"hello world"<<endl;
    return 0;
}