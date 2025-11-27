#ifndef SCENEEND_H
#define SCENEEND_H

#include "Scene.h"
#include <string>
#include <SDL_mixer.h>

class SceneEnd : public Scene {
public:
    void init() ;
    void update(float deltaTime) ;
    void render() ;
    void clean() ;
    void handleEvents(SDL_Event* event) ;
private:
    bool isTyping=true;
    std::string name="";
    float blinkTimer=1.0f;
    Mix_Music* bgm;

    void renderMyPayment();
    void renderPhase1();
    void renderPhase2();

    void removeLastUTFChar(std::string& str);
};

#endif // SCENEEND_H