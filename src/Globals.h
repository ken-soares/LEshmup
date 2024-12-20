//
// Created by ken-soares on 01/04/2024.
//

#ifndef GAME_GLOBALS_H
#define GAME_GLOBALS_H

#include "raylib.h"

#define KILL_SCORE 150
#define PICKUP_LIMIT 1000
#define MAX_ONSCREEN_PICKUP 3
#define PICKUP_TIMER 45
#define HITBOX_OFFSET_Y 10
#define TEXTURE_OFFSET_X (-20)

constexpr int screenWidth = 1280;
constexpr int screenHeight = 720;
constexpr float sceneSpeed = 4.0f;

constexpr int gameFontSpacing = 2;
constexpr int gameNameSize = 100;
constexpr int menuItemSize = 50;


enum Ecrans {
    MENU = 0,
    DLG_1,
    LV1,
    DLG_2,
    LV2,
    DLG_3,
    LV3,
    DLG_4,
    LV4,
    SCOREBOARD,
    OPTIONS,
};

enum PickupType {
    NONE,
    ONE_UP,
    BOMB,
    UPGRADE,
    SHIELD,
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

struct Pickup {
    bool remove;
    Vector2 pos;
    Vector2 vel;
    PickupType type;
    float timer;
    bool magnet;
};

struct Particle {
    bool remove;
    Vector2 pos;
    Vector2 vel;
    int timer;
    Color color;
};



#endif//GAME_GLOBALS_H
