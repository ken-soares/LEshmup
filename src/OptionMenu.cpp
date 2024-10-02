//
// Created by ken-soares on 11/09/2024.
//

#include "OptionMenu.h"
#include "Globals.h"
#include "raylib.h"

enum Option {
    PLAYER_SPEED = 0,
   // BOMB_NB,
    SHOT_TYPE,
    BACK,
};


OptionMenu::OptionMenu() : BaseScene() {
    select = 0;

    textSize = static_cast<int>(MeasureTextEx(gameFont, "Options", gameNameSize, gameFontSpacing).x);
    xCenterText = static_cast<float>(screenWidth / 2.0f - (static_cast<float>(textSize) / 2.0f));
}

OptionMenu::~OptionMenu() = default;

int OptionMenu::update(int _count) {

    if(IsKeyPressed(KEY_DOWN)) {
        select++;
        select = select % (BACK + 1); // wrapping around
    }

    if(IsKeyPressed(KEY_UP)) {
        if(select > 0) {
            select--;
        } else {
            select = BACK;
        }
    }

    if(select == BACK && IsKeyPressed(KEY_ENTER)) {
        return Ecrans::MENU;
    }

    return Ecrans::OPTIONS;
}

void OptionMenu::draw() {
    ClearBackground(BLACK);
    DrawTextEx(gameFont, "Options", {xCenterText, 100}, gameNameSize, gameFontSpacing, (Color) PURPLE);

    if(select == PLAYER_SPEED) {
        DrawTextEx(gameFont, "Player Speed: ", {40, 300}, menuItemSize, gameFontSpacing, (Color){153, 230, 0, 255});
    } else {
        DrawTextEx(gameFont, "Player Speed: ", {40, 300}, menuItemSize, gameFontSpacing, WHITE);
    }

    DrawTextEx(gameFont, "Bomb number: ", {40, 400}, menuItemSize, gameFontSpacing, GRAY);

    if(select == SHOT_TYPE) {
        DrawTextEx(gameFont, "Shot Type: ", {40, 500}, menuItemSize, gameFontSpacing, (Color){153, 230, 0, 255});
    } else {
        DrawTextEx(gameFont, "Shot Type: ", {40, 500}, menuItemSize, gameFontSpacing, WHITE);
    }


    if(select == BACK) {
        DrawTextEx(gameFont, "Back", {40, 600}, menuItemSize, gameFontSpacing, (Color){153, 230, 0, 255});
    } else {
        DrawTextEx(gameFont, "Back", {40, 600}, menuItemSize, gameFontSpacing, WHITE);
    }
}
