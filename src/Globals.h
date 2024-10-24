//
// Created by ken-soares on 01/04/2024.
//

#ifndef GAME_GLOBALS_H
#define GAME_GLOBALS_H

#include "raylib.h"

#define KILL_SCORE 150
#define HITBOX_OFFSET_Y 20

constexpr int screenWidth = 1280;
constexpr int screenHeight = 720;
constexpr float sceneSpeed = 4.0f;

constexpr int gameFontSpacing = 2;
constexpr int gameNameSize = 50;
constexpr int menuItemSize = 35;


enum Ecrans {
    MENU = 0,
    LV1,
    LV2,
    LV3,
    LV4,
    LV5,
    SCOREBOARD,
    OPTIONS,
};

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
