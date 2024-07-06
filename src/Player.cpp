//
// Created by ken-soares on 01/04/2024.
//

#include "Player.h"

Player::Player() {
    playerPosition = {40, static_cast<float>(screenHeight) / 2};
    playerSpeed = 10.0f;
    playerTexture = LoadTexture("../res/character.png");
    numFrames = 4;
    playerTextureSize = playerTexture.width / numFrames;
    frameRect = {0.0f, 0.0f, static_cast<float>(playerTextureSize), static_cast<float>(playerTexture.height)};
    health = 3;
    showHitBox = false;
    gunReloadTimer = 0;
    gunReloadDelay = 0.2f;

    immuneDelay = 1.5f;
    immuneTimer = 0.0f;
    wasShot = false;
}

Player::~Player() = default;

void Player::draw() const {
    if(!wasShot) {
        DrawTextureRec(playerTexture, frameRect, playerPosition, WHITE);
    } else {
        DrawTextureRec(playerTexture, frameRect, playerPosition, RED);
    }
    if(showHitBox){
        DrawCircle(static_cast<int>(playerPosition.x + static_cast<float>(playerTextureSize) / 2), static_cast<int>(playerPosition.y + 105), 8.0f, RED);
        DrawCircle(static_cast<int>(playerPosition.x + static_cast<float>(playerTextureSize) / 2), static_cast<int>(playerPosition.y + 105), 5.0f, ORANGE);
    }

}

void Player::update() {

    if (IsKeyDown(KEY_LEFT) && playerPosition.x > 0) {
        playerPosition.x -= playerSpeed;
    }
    if (IsKeyDown(KEY_RIGHT) && static_cast<int>(playerPosition.x) < screenWidth - playerTextureSize/1.5) {
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


    // mécanique de tir
    bool canFire = false;
    gunReloadTimer += GetFrameTime();

    if(gunReloadTimer >= gunReloadDelay) {
        canFire = true;
        gunReloadTimer = 0.0f;
    }

    if(wasShot) {
        immuneTimer += GetFrameTime();
        //std::cout << "Immune timer: "  << immuneTimer << std::endl;
    }

    if(immuneTimer >= immuneDelay) {
        immuneTimer = 0.0f;
        wasShot = false;
    }

    if(IsKeyDown(KEY_Z) || IsKeyDown(KEY_W)) {

        if(canFire) {
            bullets.push_back(
            {
                false,
                {playerPosition.x + 80.0f, playerPosition.y + 100.0f},
                {20.0f, 0.0f},
            });

            bullets.push_back(
                    {
                            false,
                            {playerPosition.x + 80.0f, playerPosition.y + 100.0f},
                            {20.0f, 5.0f},
                    });

            bullets.push_back(
                    {
                            false,
                            {playerPosition.x + 80.0f, playerPosition.y + 100.0f},
                            {20.0f, -5.0f},
                    });
        }
    }
}

int Player::getHealth() const {
    return health;
}

void Player::setHealth(int value) {
    health = value;
}

bool Player::getShowHitBox() const {
    return showHitBox;
}

void Player::setShowHitBox(bool value) {
    showHitBox = value;
}

Vector2 Player::getPosition() const {
    return playerPosition;
}

void Player::setPosition(Vector2 value) {
    playerPosition = value;
}

Vector2 Player::getHitBoxVec() const {
    return Vector2 {playerPosition.x + static_cast<float>(playerTextureSize)/2, playerPosition.y+105};
}