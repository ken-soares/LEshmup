//
// Created by ken-soares on 29/03/2024.
//

#include "Globals.h"
#include "raylib.h"
#include "patterns.h"
#include "Scene.h"

// TODO: Réimplémenter le fullscreen quand le système de scenes est bon


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
            break;
    }
}


int main() {
    int ret_value = EXIT_SUCCESS;

    // initialisation de la fenêtre
    InitWindow(screenWidth, screenHeight, "scrolling Background");

    // données de la scène
    try {
        int currSceneCount = 1;
        int nextSceneCount = 1;
        std::shared_ptr<Scene> currScene = std::make_shared<Scene>("../res/background.png", listSpawn1);
        SetTargetFPS(60);

        while (!WindowShouldClose()) {
            nextSceneCount = currScene->update(nextSceneCount);

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
        ret_value = EXIT_FAILURE;
    }

    return ret_value;
}