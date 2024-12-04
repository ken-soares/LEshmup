//
// Created by ken-soares on 01/04/2024.
//

#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#include "Globals.h"
#include "raylib.h"
#include <list>

class Player {
public:
    Player();
    ~Player();
    void draw() const;
    void update();

    void setHealth(int value);
    [[nodiscard]] int getHealth() const;

    void setBombs(int value);
    [[nodiscard]] int getBombs() const;

    [[nodiscard]] bool getShowHitBox() const;
    void setShowHitBox(bool value);
    void setPosition(Vector2 value);
    [[nodiscard]] Vector2 getPosition() const;

    [[nodiscard]] Vector2 getHitBoxVec() const;


    Rectangle frameRect{};
    std::list<Bullet> bullets;
    float gunReloadTimer;
    float gunReloadDelay;

    float immuneTimer;
    float immuneDelay;
    bool wasShot;

    int cauldron[3]{};
private:
    Vector2 playerPosition{};
    float playerSpeed;
    Texture2D playerTexture{};
    int numFrames;
    int frameCount;
    float animTimer;
    float animDelay;
    int health;
    int bombs;
    bool showHitBox;
    int playerTextureSize;
};


#endif//GAME_PLAYER_H
