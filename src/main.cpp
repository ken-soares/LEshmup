//
// Created by ken-soares on 29/03/2024.
//

#include "Globals.h"
#include "MainMenu.h"
#include "OptionMenu.h"
#include "raylib.h"
#include "patterns.h"
#include "Scene.h"
#include <iostream>

#include "DialogueScene.h"

// DONE : Faire en sorte que les vies du joueur ne se reset pas d'un stage à l'autre
// DONE : VOIR COMMENT FONCTIONNENT LES SHADERS ET EN APPLIQUER AUX BOMBES POUR FAIRE UN EFFET DE TREMBLEMENT
// DONE : SYSTÈME DE MUSIQUE
// DONE : AJOUTER MÉCANIQUE CHAUDRON (RÉGLER LES VÉLOCITÉS, LE RAMASSAGE, ETC.)
// DONE :  SYSTÈME DE DIALOGUES

// TODO: RAJOUTER DES PATTERNS
// TODO: CHECK UNLOADING OF TEXTURES ON THE GPU
// TODO: DESIGN LES 5 NIVEAUX
// TODO: AJOUTER LE SUPPORT POUR PLUSIEURS FONT DANS LE SYSTÈME DE DIALOGUE
// TODO: REFAIRE L'ART (non urgent) (nique ta mère, en fait si, c'est urgent)
// TODO: IMPLÉMENTER LE CHOIX DES OPTIONS
// TODO: ÌMPLÉMENTER UNE SCENE DE SCOREBOARD

std::list<enemyDef> listSpawn1 = {
    {60.0, 2, 3.0f, 0.5f, move_fast, fire_none,9999.f, false},
    {180.0, 2, 3.0f, 0.75f, move_sin_narrow, fire_Straight2, 9999.f, false},
    {180.0, 2, 3.0f, 0.25f, move_sin_narrow, fire_Straight2, 9999.f, false},
    {800.0, 0, 3.0f, 0.25f, move_basic, fire_none, 9999.f, false},
    {800.0, 0, 3.0f, 0.75f, move_basic, fire_none, 9999.f, false},
    {860.0, 1, 3.0f, 0.2f, move_basic, fire_none, 9999.f, false},
    {860.0, 1, 3.0f, 0.8f, move_basic, fire_none, 9999.f, false},
    {1500.0, 0, 5.0f, 0.5f, move_none, fire_CirclePulse12, 9999.f, false},
    {3000.0, 0, 999.0f, 0.5f, move_third, fire_CircleConstUpdate04, 3500.0, true},
    {6500.0, 0, 999.0f, 0.5f, set_third, fire_CirclePulseRand05, 3000.0, true},
    {9500.0, 0, 100.0f, 0.5f, set_third, fire_Triskel8, 5000.0, true},
    //{9500.0, 0, 100.0f, 0.5f, set_third, fire_Rings, 5000.0, true}, TODO: améliorer parce que ca pue le kk
    //{9500.0, 0, 100.0f, 0.5f, set_third, fire_SpiralReverse02, 5000.0, true},
    //{9500.0, 0, 100.0f, 0.5f, set_third, fire_DoubleSpiral, 5000.0, true},
};

std::shared_ptr<BaseScene> SwitchScenes(const int count) {
    switch (count) {
        case MENU:
            std::cout << "changed scene count:" << count << std::endl;
            return std::make_shared<MainMenu>();

        case DLG_1:
            std::cout << "ON VA BLABLA" << count << std::endl;
            return std::make_shared<DialogueScene>("../res/blabla/1.txt");
        case LV1:
            std::cout << "changed scene count:" << count << std::endl;
            return std::make_shared<Scene>("../res/redesign/lv1.png", listSpawn1);

        case OPTIONS:
            std::cout << "changed scene count:" << count << std::endl;
            return std::make_shared<OptionMenu>();

        default:
            std::cout << "changed scene count:" << count << std::endl;
            return std::make_shared<Scene>("../res/redesign/lv1.png", listSpawn1);
    }
}

void Setup() {
    InitWindow(screenWidth, screenHeight, "scrolling Background"); // initialisation de la fenêtre
    InitAudioDevice();
    SetTargetFPS(60);
    SetConfigFlags(FLAG_MSAA_4X_HINT);      // Enable Multi Sampling Anti Aliasing 4x (if available)

    if (!IsCursorHidden()) {
        HideCursor();
    }
}

void Cleanup() {
    CloseAudioDevice();
    CloseWindow();
}

int main() {

    Setup();

    try {
        int currSceneCount = 0;
        std::shared_ptr<BaseScene> currScene = std::make_shared<MainMenu>();

        while (!WindowShouldClose()) {

            const int nextSceneCount = currScene->update(currSceneCount);

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

    Cleanup();

    return EXIT_SUCCESS;
}