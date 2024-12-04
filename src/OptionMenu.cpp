//
// Created by ken-soares on 11/09/2024.
//

#include "OptionMenu.h"

#include <iostream>

#include "Globals.h"
#include "raylib.h"

enum Option {
    PLAYER_SPEED = 0,
    BOMB_NB,
    SHOT_TYPE,
    BACK,
};

OptionMenu::OptionMenu() : BaseScene() {
    select = 0;

    optionCounter = 0;
    bombVals = 3;
    speedVals = 2;
    shotVals = 1;

    textSize = static_cast<int>(MeasureTextEx(gameFont, "Options", gameNameSize, gameFontSpacing).x);
    xCenterText = static_cast<float>(screenWidth / 2.0f - (static_cast<float>(textSize) / 2.0f));
}

OptionMenu::~OptionMenu() = default;

int OptionMenu::update(int _count) {

    if (IsKeyPressed(KEY_DOWN)) {
        select++;
        select = select % (BACK + 1);// wrapping around
    }

    if (IsKeyPressed(KEY_UP)) {
        if (select > 0) {
            select--;
        } else {
            select = BACK;
        }
    }

    if (select == PLAYER_SPEED && IsKeyPressed(KEY_RIGHT)) {
        std::cout << "right pressed" << std::endl;
        if (speedVals < 3) {
            speedVals++;
        }
    }

    if (select == PLAYER_SPEED && IsKeyPressed(KEY_LEFT)) {
        std::cout << "left pressed" << std::endl;
        if (speedVals > 1) {
            speedVals--;
        }
    }

    if (select == SHOT_TYPE && IsKeyPressed(KEY_RIGHT)) {
        std::cout << "right pressed" << std::endl;
        if (shotVals < 3) {
            shotVals++;
        }
    }

    if (select == SHOT_TYPE && IsKeyPressed(KEY_LEFT)) {
        std::cout << "left pressed" << std::endl;
        if (shotVals > 1) {
            shotVals--;
        }
    }

    if (select == BOMB_NB && IsKeyPressed(KEY_RIGHT)) {
        std::cout << "right pressed" << std::endl;
        if (bombVals < 3) {
            bombVals++;
        }
    }

    if (select == BOMB_NB && IsKeyPressed(KEY_LEFT)) {
        std::cout << "left pressed" << std::endl;
        if (bombVals > 1) {
            bombVals--;
        }
    }
    if (select == BACK && IsKeyPressed(KEY_ENTER)) {
        return Ecrans::MENU;
    }

    return Ecrans::OPTIONS;
}

void OptionMenu::draw() {
    ClearBackground(BLACK);
    DrawTextEx(gameFont, "Options", {xCenterText, 100}, gameNameSize, gameFontSpacing, (Color) PURPLE);

    if (select == PLAYER_SPEED) {
        DrawTextEx(gameFont, "Player Speed: ", {40, 300}, menuItemSize, gameFontSpacing, (Color){153, 230, 0, 255});
        DrawTextEx(gameFont,std::to_string(speedVals).c_str() , {800, 300}, menuItemSize, gameFontSpacing, (Color){153, 230, 0, 255});
    } else {
        DrawTextEx(gameFont, "Player Speed: ", {40, 300}, menuItemSize, gameFontSpacing, WHITE);
        DrawTextEx(gameFont,std::to_string(speedVals).c_str() , {800, 300}, menuItemSize, gameFontSpacing, WHITE);
    }

    if (select == BOMB_NB) {
        DrawTextEx(gameFont, "Bomb number: ", {40, 400}, menuItemSize, gameFontSpacing, (Color){153, 230, 0, 255});
        DrawTextEx(gameFont,std::to_string(bombVals).c_str() , {800, 400}, menuItemSize, gameFontSpacing, (Color){153, 230, 0, 255});
    } else {
        DrawTextEx(gameFont, "Bomb number: ", {40, 400}, menuItemSize, gameFontSpacing, WHITE);
        DrawTextEx(gameFont,std::to_string(bombVals).c_str() , {800, 400}, menuItemSize, gameFontSpacing, WHITE);
    }

    if (select == SHOT_TYPE) {
        DrawTextEx(gameFont, "Shot Type: ", {40, 500}, menuItemSize, gameFontSpacing, (Color){153, 230, 0, 255});
        DrawTextEx(gameFont,std::to_string(shotVals).c_str() , {800, 500}, menuItemSize, gameFontSpacing, (Color){153, 230, 0, 255});
    } else {
        DrawTextEx(gameFont, "Shot Type: ", {40, 500}, menuItemSize, gameFontSpacing, WHITE);
        DrawTextEx(gameFont,std::to_string(shotVals).c_str() , {800, 500}, menuItemSize, gameFontSpacing, WHITE);
    }

    if (select == BACK) {
        DrawTextEx(gameFont, "Back", {40, 600}, menuItemSize, gameFontSpacing, (Color){153, 230, 0, 255});
    } else {
        DrawTextEx(gameFont, "Back", {40, 600}, menuItemSize, gameFontSpacing, WHITE);
    }
}
