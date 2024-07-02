//
// Created by ken-soares on 01/04/2024.
//

#ifndef GAME_GLOBALS_H
#define GAME_GLOBALS_H

#include "raylib.h"

const int screenWidth = 1280;
const int screenHeight = 720;
const float sceneSpeed = 4.0f;


typedef enum Ecrans {
    MENU = 0,
    LV1,
    LV2,
    LV3,
    LV4,
    LV5,
    SCOREBOARD,
} Ecrans;

enum BulletType {
    BULLET_LONG,
    BULLET_ROUND,
    BULLET_PLAYER,
};

struct Bullet {
    bool remove;
    Vector2 pos;
    Vector2 vel;
    BulletType type;
};

struct Particle {
    bool remove;
    Vector2 pos;
    Vector2 vel;
    int timer;
    Color color;
};
#endif//GAME_GLOBALS_H
