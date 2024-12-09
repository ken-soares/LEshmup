//
// Created by ken-soares on 01/04/2024.
//

#include "Player.h"

#include <iostream>

Player::Player() {
    emptyCauldron();

    playerPosition = {40, static_cast<float>(screenHeight) / 2};
    playerSpeed = 10.0f;
    playerTexture = LoadTexture("../res/redesign/perso_spritesheet_r.png");
    numFrames = 2;
    frameCount = 0;
    playerTextureSize = (playerTexture.width + TEXTURE_OFFSET_X) / numFrames;
    frameRect = {0.0f, 0.0f, static_cast<float>(playerTextureSize), static_cast<float>(playerTexture.height)};
    health = 3;
    bombs = 3;
    showHitBox = false;
    gunReloadTimer = 0;
    gunReloadDelay = 0.2f;
    animTimer = 0.0f;
    animDelay = 0.5f;


    shotLevel = 1;

    normalImmuneDelay = 1.5f;
    immuneDelay = normalImmuneDelay;
    shieldDelay = 15.0f;
    immuneTimer = 0.0f;
    shieldOn = false;

    wasShot = false;
}

Player::~Player() {
    UnloadTexture(playerTexture);
}

void Player::draw() const {
    if (!wasShot && !shieldOn) {
        DrawTextureRec(playerTexture, frameRect, playerPosition, WHITE);
    } else if (wasShot) {
        DrawTextureRec(playerTexture, frameRect, playerPosition, RED);
    } else {
        DrawTextureRec(playerTexture, frameRect, playerPosition, BLUE);
    }

    if (showHitBox) {
        DrawCircle(static_cast<int>(playerPosition.x + static_cast<float>(playerTextureSize) / 2),
                   static_cast<int>(playerPosition.y + HITBOX_OFFSET_Y + static_cast<float>(playerTextureSize) / 2),
                   8.0f, RED);
        DrawCircle(static_cast<int>(playerPosition.x + static_cast<float>(playerTextureSize) / 2),
                   static_cast<int>(playerPosition.y + HITBOX_OFFSET_Y + static_cast<float>(playerTextureSize) / 2),
                   5.0f, ORANGE);
    }
}

void Player::activateC() {
    if (IsKeyPressed(KEY_C) && canActivateC() != -1) {
        switch (canActivateC()) {
            case ONE_UP:
                if (health < 3) setHealth(health + 1);
                break;
            case BOMB:
                if (bombs < 3) setBombs(bombs + 1);
                break;
            case UPGRADE:
                if (shotLevel < 3) setShotLevel(shotLevel + 1);
                break;
            case SHIELD:
                shieldOn = true;
                break;
            default:
                std::cout << "problem in c activation" << std::endl;
        }
        emptyCauldron();
    }
}

void Player::move() {
    if (IsKeyDown(KEY_LEFT) && playerPosition.x > 0) {
        playerPosition.x -= playerSpeed;
    }
    if (IsKeyDown(KEY_RIGHT) && static_cast<int>(playerPosition.x) < screenWidth - playerTextureSize / 1.5) {
        playerPosition.x += playerSpeed;
    }
    if (IsKeyDown(KEY_UP) && playerPosition.y > 0) {
        playerPosition.y -= playerSpeed;
    }
    if (IsKeyDown(KEY_DOWN) && static_cast<int>(playerPosition.y) < screenHeight - playerTexture.height) {
        playerPosition.y += playerSpeed;
    }
    if (IsKeyDown(KEY_LEFT_SHIFT)) {
        playerSpeed = 4.0f;
        setShowHitBox(true);
    } else {
        playerSpeed = 10.0f;
        setShowHitBox(false);
    }
}

void Player::fire() {
    // mécanique de tir
    bool canFire = false;
    gunReloadTimer += GetFrameTime();

    if (gunReloadTimer >= gunReloadDelay) {
        canFire = true;
        gunReloadTimer = 0.0f;
    }
    if (IsKeyDown(KEY_Z) || IsKeyDown(KEY_W)) {
        if (canFire) {
            bullets.push_back(
                {
                    false,
                    {playerPosition.x + 80.0f, playerPosition.y + 100.0f},
                    {20.0f, 0.0f},
                }
            );
            if (shotLevel >= 2) {
                bullets.push_back(
                    {
                        false,
                        {playerPosition.x + 80.0f, playerPosition.y + 100.0f},
                        {20.0f, 5.0f},
                    }
                );
            }
            if (shotLevel >= 3) {
                bullets.push_back(
                    {
                        false,
                        {playerPosition.x + 80.0f, playerPosition.y + 100.0f},
                        {20.0f, -5.0f},
                    }
                );
            }
        }
    }
}

void Player::animate() {
    // animation du joueur
    animTimer += GetFrameTime();
    // std::cout << frameCount << "|" << animTimer << std::endl;
    if (animTimer >= animDelay) {
        frameCount++;
        frameCount = frameCount % (numFrames); // wrapping around
        frameRect.x = static_cast<float>(frameCount) * 128.0f;
        animTimer = 0.0f;
    }
}

void Player::immune() {
    if (shieldOn) {
        immuneDelay = shieldDelay;
    } else {
        immuneDelay = normalImmuneDelay;
    }

    if (wasShot || shieldOn) {
        immuneTimer += GetFrameTime();
        //std::cout << "Immune timer: "  << immuneTimer << std::endl;
    }

    if (immuneTimer >= immuneDelay) {
        immuneTimer = 0.0f;
        wasShot = false;
        shieldOn = false;
    }
}

void Player::godMode() {
    if (IsKeyPressed(KEY_SPACE)) {
        std::cout << "entered god mode" << std::endl;
        setCauldron(UPGRADE);
        setCauldron(UPGRADE);
        setCauldron(UPGRADE);
    }
}

void Player::update() {
    godMode();
    activateC();
    move();
    animate();
    fire();
    immune();
}

int Player::getHealth() const {
    return health;
}

void Player::setHealth(const int value) {
    health = value;
}

int Player::getBombs() const {
    return bombs;
}

void Player::setBombs(const int value) {
    bombs = value;
}

bool Player::getShowHitBox() const {
    return showHitBox;
}

void Player::setShowHitBox(const bool value) {
    showHitBox = value;
}

Vector2 Player::getPosition() const {
    return playerPosition;
}

void Player::setPosition(const Vector2 value) {
    playerPosition = value;
}

Vector2 Player::getHitBoxVec() const {
    return Vector2{
        playerPosition.x + static_cast<float>(playerTextureSize) / 2,
        playerPosition.y + HITBOX_OFFSET_Y + static_cast<float>(playerTextureSize) / 2
    };
}

int Player::getCauldron(const int pos) const {
    return cauldron[pos];
}

void Player::emptyCauldron() {
    cauldron[0] = NONE;
    cauldron[1] = NONE;
    cauldron[2] = NONE;
}

void Player::setShotLevel(const int level) {
    std::cout << level << std::endl;
    shotLevel = level;
}

void Player::setCauldron(const int color) {
    for (auto &item: cauldron) {
        if (item != color) {
            if (item == NONE) {
                item = color;
                return;
            }
            emptyCauldron();
            return;
        }
    }
}

void Player::printCauldron() {
    for (const auto &item: cauldron) {
        std::cout << item << std::endl;
    }
}

int Player::canActivateC() {
    for (const auto &item: cauldron) {
        if (item == NONE) {
            return -1;
        }
    }

    return getCauldron(0);
}
