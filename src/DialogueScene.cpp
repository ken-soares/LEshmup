//
// Created by ken-soares on 11/12/2024.
//

#include "DialogueScene.h"

#include <iostream>

#include "DialogueSystem.h"
#include "Globals.h"
#include <random>

DialogueScene::DialogueScene(const std::string &Filepath) :
dialogueSystem(splitStringByNewline(readFile(Filepath)), 800, 0.02f){
    hasLoadedCharFace = false;
    chatBoxTex = LoadTexture("../res/redesign/text_box.png");
    charFaceTexture = {};
    charSound = {};
}

DialogueScene::~DialogueScene() {
    UnloadTexture(charFaceTexture);
}

int DialogueScene::update(const int _count) {

    if (dialogueSystem.getSpriteName() != charFacePath) {
        hasLoadedCharFace = false;
    }

    if (!hasLoadedCharFace) {
        hasLoadedCharFace = true;
        charFacePath = dialogueSystem.getSpriteName();
        charSoundPath = dialogueSystem.getSpriteAudio();
    }

    charFaceTexture = LoadTexture(charFacePath.c_str());
    charSound = LoadSound(charSoundPath.c_str());

    int textLeft = true;
    bool playSound = dialogueSystem.update();

    std::random_device rd; // Source aléatoire non déterministe
    std::mt19937 generator(rd()); // Générateur Mersenne Twister
    std::uniform_real_distribution pitchDist(0.9f, 1.1f);

    if (playSound) {
        SetSoundPitch(charSound, pitchDist(generator));
        PlaySound(charSound);
    }
    if (IsKeyReleased(KEY_X)) {
        textLeft = dialogueSystem.nextLine();
    }

    if (IsKeyReleased(KEY_LEFT_SHIFT)) {
        textLeft = false;
        dialogueSystem.currentLine = dialogueSystem.dialogueLines.size() - 1;
    }

    if (!textLeft && dialogueSystem.currentLine + 1 == dialogueSystem.dialogueLines.size()) {
        return _count + 1;
    }

    return _count;
}

void DialogueScene::draw() {
    ClearBackground(BLACK);
    DrawRectangle(0, screenHeight - 220, screenWidth, 300,{89, 91, 125, 255});

    for (int i = 0; i < 8; i++)
        DrawTexture(chatBoxTex, i * 180, screenHeight-220, WHITE);

    if (hasLoadedCharFace) {
        DrawTextureEx(charFaceTexture, {0 + 2,screenHeight - 190 + 2}, 0.0f, 0.3f, BLACK);
        DrawTextureEx(charFaceTexture, {0,screenHeight - 190}, 0.0f, 0.3f, WHITE);
    }
    dialogueSystem.draw(200,screenHeight-200,30,WHITE);
    Vector2 pos;
    pos.x = screenWidth/2.75f;
    pos.y = screenHeight-30;
    DrawTextEx(gameFont,"next: X   display all: C   skip: Left Shift", pos,25,gameFontSpacing, {230, 225, 242, 255});
}
