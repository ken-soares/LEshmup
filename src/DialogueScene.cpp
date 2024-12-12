//
// Created by ken-soares on 11/12/2024.
//

#include "DialogueScene.h"

#include <iostream>

#include "DialogueSystem.h"
#include "Globals.h"

DialogueScene::DialogueScene(const std::string &Filepath) :
dialogueSystem(splitStringByNewline(readFile(Filepath)), 800, 0.02f){
}

DialogueScene::~DialogueScene() = default;

int DialogueScene::update(const int _count) {
    int textLeft = true;
    dialogueSystem.update();
    if (IsKeyReleased(KEY_X)) {
        textLeft = dialogueSystem.nextLine();
    }

    if (IsKeyReleased(KEY_LEFT_SHIFT)) {
        textLeft = false;
    }

    if (!textLeft) {
        return _count + 1;
    }

    return _count;
}

void DialogueScene::draw() {
    ClearBackground(BLACK);
    dialogueSystem.draw(50,screenHeight-200,20,WHITE);
    DrawText("next: X   display all: C   skip: Left Shift", screenWidth/2.5f,screenHeight-30,15,GRAY);
}
