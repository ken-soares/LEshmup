//
// Created by ken-soares on 02/04/2024.
//

#include "Enemy.h"


Enemy::Enemy(const enemyDef& d) {
    def = d;
    pos = {
            screenWidth,
            d.offset * static_cast<float>(screenHeight),
    };

}
Enemy::~Enemy() = default;

void Enemy::update(std::list<Bullet> &listBullets) {
    def.funcMove(*this);
    def.funcFire(*this, listBullets);
    def.timer -= sceneSpeed;
    if(def.timer <= 0) {
        listBullets.clear();
    }
}

Vector2 Enemy::getPos() const {
    return pos;
}

void Enemy::setPos(Vector2 p) {
    pos = p;
}
