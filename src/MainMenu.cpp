//
// Created by ken-soares on 06/07/2024.
//

#include "MainMenu.h"

#include "Globals.h"

#define OFFSET 30.0f

enum Option {
    PLAY = 0,
    FULLSCREEN,
    OPT,
    QUIT,
};

MainMenu::MainMenu() : BaseScene(){
    select = 0;
    femiTex = LoadTexture("../res/redesign/naf0.png");
    animDir = 1;
    femiAnimRate = 0.0f;

    textSize = static_cast<int>(MeasureTextEx(gameFont, "Game Name", gameNameSize, gameFontSpacing).x);
    xCenterText = static_cast<float>(screenWidth/2.0f - (static_cast<float>(textSize)/2.0f));


    playCenter = static_cast<int>(screenWidth/2.0f - (MeasureTextEx(gameFont, "Play", gameNameSize, gameFontSpacing).x)/2.0f);
    fullCenter = static_cast<int>(screenWidth/2.0f - (MeasureTextEx(gameFont, "Fullscreen", gameNameSize, gameFontSpacing).x)/2.0f);
    optCenter = static_cast<int>(screenWidth/2.0f - (MeasureTextEx(gameFont, "Options", gameNameSize, gameFontSpacing).x)/2.0f);
    quitCenter = static_cast<int>(screenWidth/2.0f - (MeasureTextEx(gameFont, "Quit", gameNameSize, gameFontSpacing).x)/2.0f);
}

MainMenu::~MainMenu() = default;

int MainMenu::update(int _count) {

    femiAnimRate += GetFrameTime();

    if(femiAnimRate >= 0.5f) {
        femiAnimRate = 0.0f;
        animDir *= (-1);
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
        return Ecrans::OPTIONS;
    }

    if(IsKeyPressed(KEY_ENTER) && select == PLAY) {

        // vérification premier lancement du jeu
        if(!exists("../res/save/high.save")) {
            writeFile("../res/save/high.save", "0");
        }

        writeFile("../res/temp/score.temp", "0");
        writeFile("../res/temp/lives.temp", "3");
        writeFile("../res/temp/bombs.temp", "3");

        return Ecrans::LV1;
    } else {
        return Ecrans::MENU;
    }
}

void MainMenu::draw() {

    ClearBackground(BLACK);

    DrawTextEx(gameFont, "Game Name", {xCenterText, 100}, gameNameSize, gameFontSpacing, (Color)PURPLE);
    DrawTextEx(gameFont, "A game by Torrent", {screenWidth - 380.0f, screenHeight-50.0f}, 24, gameFontSpacing, (Color)WHITE);

    DrawTextureEx(femiTex, {screenWidth - 150.0f, screenHeight - 130.0f + static_cast<float>(animDir) * femiAnimRate * 6}, 0, 4, WHITE);

    if(select == PLAY) {
        DrawTextEx(gameFont, "> Play <", {static_cast<float>(playCenter), 300}, menuItemSize, gameFontSpacing, (Color){153, 230, 0, 255});
    } else {
        DrawTextEx(gameFont, "Play", {static_cast<float>(playCenter) + OFFSET, 300}, menuItemSize, gameFontSpacing, WHITE);
    }

    if(select == FULLSCREEN) {
        DrawTextEx(gameFont, "> Fullscreen <", {static_cast<float>(fullCenter) + 15.0f, 350}, menuItemSize, gameFontSpacing, (Color){153, 230, 0, 255});
    } else {
        DrawTextEx(gameFont, "Fullscreen", {static_cast<float>(fullCenter) + OFFSET + 15.0f, 350}, menuItemSize, gameFontSpacing, WHITE);
    }

    if(select == OPT) {
        DrawTextEx(gameFont, "> Options <", {static_cast<float>(optCenter), 400}, menuItemSize, gameFontSpacing, (Color){153, 230, 0, 255});
    } else {
        DrawTextEx(gameFont, "Options", {static_cast<float>(optCenter) + OFFSET, 400}, menuItemSize, gameFontSpacing, WHITE);
    }

    if(select == QUIT) {
        DrawTextEx(gameFont, "> Quit <", {static_cast<float>(quitCenter), 450}, menuItemSize, gameFontSpacing, (Color){153, 230, 0, 255});
    } else {
        DrawTextEx(gameFont, "Quit", {static_cast<float>(quitCenter) + OFFSET, 450}, menuItemSize, gameFontSpacing, WHITE);
    }
}
