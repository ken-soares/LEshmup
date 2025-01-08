//
// Created by ken-soares on 06/07/2024.
//

#ifndef MAINMENU_H
#define MAINMENU_H

#include "BaseScene.h"
#include "raylib.h"

class MainMenu final : public BaseScene {

public:

    MainMenu();
    ~MainMenu() override;

    int update(int _count) override;
    void draw() override;

    // implémentation de la flemme surpuissante

    bool canPlay = false;
    int select;


    // animation
    Texture2D femiTex{};
    Texture2D bgTex{};
    float femiAnimRate;
    int animDir;

private:

    int textSize;
    float xCenterText;
};

#endif //MAINMENU_H
