#ifndef SCENEEND_H
#define SCENEEND_H

#include "Scene.h"
#include <string>

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

    void renderPhase1();
    void renderPhase2();

    void removeLastUTFChar(std::string& str);
};

#endif // SCENEEND_H