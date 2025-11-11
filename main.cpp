#include<iostream>
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<SDL_mixer.h>
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
    if(IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG)  != (IMG_INIT_PNG|IMG_INIT_JPG))
    {
        cerr << "IMG_Init Error: " << IMG_GetError() << endl;
        return 1;
    }

    // 加载图片
    SDL_Texture *tex = IMG_LoadTexture(ren, "assets/image/bg.png");

    //初始化SDL_ttf
    if(TTF_Init() == -1){
        cerr << "TTF_Init Error: " << TTF_GetError() << endl;
        return 1;
    }

    // 加载字体
    TTF_Font *font = TTF_OpenFont("assets/font/VonwaonBitmap-12px.ttf", 24);
    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface *surface = TTF_RenderUTF8_Solid(font, "Hello SDL2!", color);
    SDL_Texture *text = SDL_CreateTextureFromSurface(ren, surface);

    // 初始化SDL_mixer
    if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048) < 0)
    {
        cerr << "Mix_OpenAudio Error: " << Mix_GetError() << endl;
        return 1;
    }

    // 加载音频
    Mix_Music *music = Mix_LoadMUS("assets/music/03_Racing_Through_Asteroids_Loop.ogg");
    Mix_PlayMusic(music, -1);

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

        //画一个图片
        SDL_Rect rect2 = {200, 200, 400, 300};
        SDL_RenderCopy(ren, tex, NULL, &rect2);

        //画一个文本
        SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
        SDL_Rect rect3 = {300, 300, surface->w, surface->h};
        SDL_RenderCopy(ren, text, NULL, &rect3);

        //更新屏幕
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderPresent(ren);
    }
    //释放字体资源
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(text);  
    TTF_CloseFont(font);
    TTF_Quit();

    //释放音频资源
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    Mix_Quit();

    //释放图片资源
    SDL_DestroyTexture(tex);
    IMG_Quit();

    //清理并关闭渲染器和窗口
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}