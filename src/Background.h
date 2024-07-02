//
// Created by ken-soares on 01/04/2024.
//

#ifndef GAME_BACKGROUND_H
#define GAME_BACKGROUND_H

#include "raylib.h"
#include <string>

class Background {
public:
    [[maybe_unused]] explicit Background(std::string filepath);
    ~Background();
    void update();
    void draw();
private:
    float scrollback;
    Texture2D background{};
};


#endif//GAME_BACKGROUND_H
