//
// Created by ken-soares on 06/07/2024.
//

#include "MainMenu.h"

#include "Globals.h"

#define GAME_NAME "One ingredient short of youth!"
#define MENU_X_ALIGN 40.0f
enum Option {
    PLAY = 0,
    FULLSCREEN,
    OPT,
    QUIT,
};

MainMenu::MainMenu() {
    select = 0;
    femiTex = LoadTexture("../res/redesign/leaf_menu.png");
    bgTex = LoadTexture("../res/redesign/bg_menu.png");
    animDir = 1;
    femiAnimRate = 0.0f;

    textSize = static_cast<int>(MeasureTextEx(gameFont, GAME_NAME, gameNameSize, gameFontSpacing).x);
    xCenterText = (screenWidth/2.0f - static_cast<float>(textSize)/2.0f) - 40.0f;
}

MainMenu::~MainMenu() {
    UnloadTexture(femiTex);
    UnloadTexture(bgTex);
}

int MainMenu::update(int _count) {

    femiAnimRate += GetFrameTime();

    if(femiAnimRate >= 0.5f) {
        femiAnimRate = 0.0f;
        animDir *= -1;
    }

    if(IsKeyPressed(KEY_DOWN)) {
        select++;
        select = select % (QUIT + 1); // wrapping around
    }

    if(IsKeyPressed(KEY_UP)) {
        if(select > 0) {
            select--;
        } else {
            select = QUIT;
        }
    }

    if(IsKeyPressed(KEY_ENTER) && select == FULLSCREEN) {
        ToggleFullscreen();
    }

    if(IsKeyPressed(KEY_ENTER) && select == QUIT) {
        return -1;
    }

    if(IsKeyPressed(KEY_ENTER) && select == OPT) {
        return OPTIONS;
    }

    if(IsKeyPressed(KEY_ENTER) && select == PLAY) {

        // vérification premier lancement du jeu
        if(!exists("../res/save/high.save")) {
            writeFile("../res/save/high.save", "0");
        }

        writeFile("../res/temp/score.temp", "0");
        writeFile("../res/temp/lives.temp", "3");
        writeFile("../res/temp/bombs.temp", "3");

        return DLG_1;
    }
    return MENU;
}

void MainMenu::draw() {

    ClearBackground(BLACK);

    DrawTextureEx(bgTex, {0.0f,0.0f}, 0, 1, WHITE);

    DrawTextEx(gameFont, GAME_NAME, {xCenterText+2, 100+2}, gameNameSize, gameFontSpacing, BLACK);
    DrawTextEx(gameFont, GAME_NAME, {xCenterText, 100}, gameNameSize, gameFontSpacing, WHITE);
    DrawTextEx(gameFont, "A game by Torrent", {60.0f, screenHeight-50.0f}, 34, gameFontSpacing, WHITE);

    DrawTextureEx(femiTex, {0.0f, screenHeight - 70.0f + static_cast<float>(animDir) * femiAnimRate * 6}, 0, 0.6, WHITE);

    if(select == PLAY) {
        DrawTextEx(gameFont, "Play o", {MENU_X_ALIGN, 300}, menuItemSize, gameFontSpacing, P_YELLOW);
    } else {
        DrawTextEx(gameFont, "Play", {MENU_X_ALIGN, 300}, menuItemSize, gameFontSpacing, WHITE);
    }

    if(select == FULLSCREEN) {
        DrawTextEx(gameFont, "Fullscreen o", {MENU_X_ALIGN, 350}, menuItemSize, gameFontSpacing, P_YELLOW);
    } else {
        DrawTextEx(gameFont, "Fullscreen", {MENU_X_ALIGN, 350}, menuItemSize, gameFontSpacing, WHITE);
    }

    if(select == OPT) {
        DrawTextEx(gameFont, "Options o", {MENU_X_ALIGN, 400}, menuItemSize, gameFontSpacing, P_YELLOW);
    } else {
        DrawTextEx(gameFont, "Options", {MENU_X_ALIGN, 400}, menuItemSize, gameFontSpacing, WHITE);
    }

    if(select == QUIT) {
        DrawTextEx(gameFont, "Quit o", {MENU_X_ALIGN, 450}, menuItemSize, gameFontSpacing, P_YELLOW);
    } else {
        DrawTextEx(gameFont, "Quit", {MENU_X_ALIGN, 450}, menuItemSize, gameFontSpacing, WHITE);
    }
}
