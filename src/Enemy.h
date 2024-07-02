//
// Created by ken-soares on 02/04/2024.
//

#ifndef GAME_ENEMY_H
#define GAME_ENEMY_H

#include "Globals.h"
#include "raylib.h"
#include <cstdint>
#include <functional>
#include <iostream>
#include <list>
#include <memory>

class Enemy;

struct enemyDef {
    double triggerTime = 0.0;
    uint32_t spriteID = 0;
    float health = 0.0f;
    float offset = 0.0f;
    std::function<void(Enemy &)> funcMove;
    std::function<void(Enemy &, std::list<Bullet>&)> funcFire;
    float timer = 18000.0f;
};

class Enemy {
public:
    explicit Enemy(const enemyDef& d);
    ~Enemy();
    void update(std::list<Bullet>& listBullets);
    Vector2 getPos();
    void setPos(Vector2 p);
    enemyDef def;

    // array pour informations complémentaires liées aux déplacements et aux tirs.
    std::array<float, 4> dataMove{0};
    std::array<float, 4> dataFire{0};
private:
    Vector2 pos{};
};

#endif//GAME_ENEMY_H
