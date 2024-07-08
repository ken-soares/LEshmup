//
// Created by ken-soares on 06/07/2024.
//

#include "BaseScene.h"

BaseScene::BaseScene() {

    // CREDIT Hewett Tsoi
    gameFont = LoadFont("../res/alagard.ttf");
}

BaseScene::~BaseScene() = default;

void BaseScene::draw() {}

int BaseScene::update(const int count) {
    return count;
}
