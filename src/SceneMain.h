#ifndef SCENE_MAIN_H
#define SCENE_MAIN_H

#include "Scene.h"
#include "SDL.h"

class SceneMain : public Scene {
public:
    SceneMain();
    ~SceneMain();

    void update() override;
    void render() override;
    void init() override;
    void clean() override;
    void handleEvents(SDL_Event* event) override;
    
};
#endif // SCENE_MAIN_H