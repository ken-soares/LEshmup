//
// Created by ken-soares on 11/09/2024.
//

#include "OptionMenu.h"
#include "Globals.h"
#include "raylib.h"

enum Option {
    PLAYER_SPEED = 0,
    BOMB_NB,
    SHOT_TYPE,
};


OptionMenu::OptionMenu() : BaseScene() {
    select = 0;

    textSize = static_cast<int>(MeasureTextEx(gameFont, "Options", gameNameSize, gameFontSpacing).x);
    xCenterText = static_cast<float>(screenWidth / 2.0f - (static_cast<float>(textSize) / 2.0f));
}

OptionMenu::~OptionMenu() = default;

int OptionMenu::update(int _count) {
    if (IsKeyPressed(KEY_SPACE)) {
        return Ecrans::MENU;
    }
    return Ecrans::OPTIONS;
}

void OptionMenu::draw() {
    ClearBackground(BLACK);
    DrawTextEx(gameFont, "Options", {xCenterText, 100}, gameNameSize, gameFontSpacing, (Color) PURPLE);
}
