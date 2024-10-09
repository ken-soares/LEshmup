//
// Created by ken-soares on 11/09/2024.
//

#ifndef GAME_OPTIONMENU_H
#define GAME_OPTIONMENU_H

#include "BaseScene.h"

class OptionMenu final : public BaseScene {
public:
    OptionMenu();
    ~OptionMenu() override;
    int update(int _count) override;
    void draw() override;
    int select;

private:
    int optionCounter;

    int bombVals;
    int speedVals;
    int shotVals;

    int textSize;
    float xCenterText;
};

#endif//GAME_OPTIONMENU_H
