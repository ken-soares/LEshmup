//
// Created by ken-soares on 29/03/2024.
//

#include "Globals.h"
#include "MainMenu.h"
#include "raylib.h"
#include "patterns.h"
#include "Scene.h"
#include <iostream>

// DONE: Faire en sorte que les vies du joueurs ne se reset pas d'un stage à l'autre
// DONE: IMPLÉMENTER UN SYSTEME DE BOMBES

// TODO: SYSTEME DE SON
// TODO: REFAIRE L'ART
// TODO: VOIR COMMENT FONCTIONNENT LES SHADERS ET EN APPLIQUER AUX BOMBES POUR FAIRE UN EFFET DE TREMBLEMENT

std::list<enemyDef> listSpawn1 = {
    {60.0, 2, 3.0f, 0.5f, move_fast, fire_none},
    {180.0, 2, 3.0f, 0.75f, move_sin_narrow, fire_Straight2},
    {180.0, 2, 3.0f, 0.25f, move_sin_narrow, fire_Straight2},
    {800.0, 0, 3.0f, 0.25f, move_basic, fire_none},
    {800.0, 0, 3.0f, 0.75f, move_basic, fire_none},
    {860.0, 1, 3.0f, 0.2f, move_basic, fire_none},
    {860.0, 1, 3.0f, 0.8f, move_basic, fire_none},
    {1500.0, 0, 5.0f, 0.5f, move_none, fire_CirclePulse12},
    {2800.0, 0, 999.0f, 0.5f, move_third, fire_CircleConstUpdate04, 3500.0},
    {6300.0, 0, 999.0f, 0.5f, set_third, fire_CirclePulseRand05, 3000.0},
    {9300.0, 0, 100.0f, 0.5f, set_third, fire_SpiralReverse02, 5000.0},
};

std::shared_ptr<Scene> SwitchScenes(const int count) {
    switch (count) {
        default:
            std::cout << "changed scene count:" << count << std::endl;
            return std::make_shared<Scene>("../res/background.png", listSpawn1);
    }
}


int main() {

    // initialisation de la fenêtre
    InitWindow(screenWidth, screenHeight, "scrolling Background");

    // données de la scène
    try {
        int currSceneCount = 0;
        int nextSceneCount;
        std::shared_ptr<BaseScene> currScene = std::make_shared<MainMenu>();
        SetTargetFPS(60);

        if (!IsCursorHidden()) {
            HideCursor();
        }

        while (!WindowShouldClose()) {

            nextSceneCount = currScene->update(currSceneCount);

            if(nextSceneCount == -1)
                break;

            if (nextSceneCount != currSceneCount) {
                currScene = SwitchScenes(nextSceneCount);
                currSceneCount = nextSceneCount;
            }

            BeginDrawing();
            currScene->draw();
            EndDrawing();
        }
    } catch (std::runtime_error &e) {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    CloseWindow();
    return EXIT_SUCCESS;
}