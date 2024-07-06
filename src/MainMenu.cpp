//
// Created by ken-soares on 06/07/2024.
//

#include "MainMenu.h"

#include "Globals.h"

#define OFFSET 30.0f

enum Option {
    PLAY = 0,
    FULLSCREEN,
    QUIT,
};

MainMenu::MainMenu() : BaseScene(){
    select = 0;
    // par la suite on pourra bouger tout ca dans BaseScene
    // CREDIT Hewett Tsoi


    gameFont = LoadFont("../res/alagard.ttf");
    textSize = static_cast<int>(MeasureTextEx(gameFont, "Game Name", gameNameSize, gameFontSpacing).x);
    xCenterText = static_cast<float>(screenWidth/2.0f - (static_cast<float>(textSize)/2.0f));


    playCenter = static_cast<int>(screenWidth/2.0f - (MeasureTextEx(gameFont, "Play", gameNameSize, gameFontSpacing).x)/2.0f);
    fullCenter = static_cast<int>(screenWidth/2.0f - (MeasureTextEx(gameFont, "Fullscreen", gameNameSize, gameFontSpacing).x)/2.0f);
    quitCenter = static_cast<int>(screenWidth/2.0f - (MeasureTextEx(gameFont, "Quit", gameNameSize, gameFontSpacing).x)/2.0f);
}

MainMenu::~MainMenu() = default;

int MainMenu::update(int _count) {

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

    if(IsKeyPressed(KEY_ENTER) && select == PLAY) {
        return 1;
    } else {
        return 0;
    }
}

void MainMenu::draw() {

    ClearBackground(BLACK);

    DrawTextEx(gameFont, "Game Name", {xCenterText, 100}, gameNameSize, gameFontSpacing, (Color)PURPLE);
    DrawTextEx(gameFont, "A game by Torrent", {screenWidth - 250.0f, screenHeight-50.0f}, 20, gameFontSpacing, (Color)WHITE);

    if(select == PLAY) {
        DrawTextEx(gameFont, "> Play <", {static_cast<float>(playCenter), 300}, menuItemSize, gameFontSpacing, (Color){153, 230, 0, 255});
    } else {
        DrawTextEx(gameFont, "Play", {static_cast<float>(playCenter) + OFFSET, 300}, menuItemSize, gameFontSpacing, WHITE);
    }

    if(select == FULLSCREEN) {
        DrawTextEx(gameFont, "> Fullscreen <", {static_cast<float>(fullCenter) + 15.0f, 400}, menuItemSize, gameFontSpacing, (Color){153, 230, 0, 255});
    } else {
        DrawTextEx(gameFont, "Fullscreen", {static_cast<float>(fullCenter) + OFFSET + 15.0f, 400}, menuItemSize, gameFontSpacing, WHITE);
    }

    if(select == QUIT) {
        DrawTextEx(gameFont, "> Quit <", {static_cast<float>(quitCenter), 500}, menuItemSize, gameFontSpacing, (Color){153, 230, 0, 255});
    } else {
        DrawTextEx(gameFont, "Quit", {static_cast<float>(quitCenter) + OFFSET, 500}, menuItemSize, gameFontSpacing, WHITE);
    }
}
