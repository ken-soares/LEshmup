//
// Created by ken-soares on 11/12/2024.
//

#include "DialogueSystem.h"
#include <iostream>

#include "Globals.h"

DialogueSystem::DialogueSystem(const std::vector<std::string> &lines, const int width,
                               const float delay): dialogueLines(lines), currentLine(0), maxWidth(width) {
    dialogue.charDelay = delay;
    startLine(currentLine);
    gameFont = LoadFontEx("../res/fg1.ttf", 200, nullptr, 255);
}

void DialogueSystem::startLine(const size_t lineIndex) {
    if (lineIndex < dialogueLines.size()) {
        dialogue.fullText = wrapText(dialogueLines[lineIndex]);
        dialogue.visibleText = "";
        dialogue.timer = 0.0f;
        dialogue.complete = false;
    }
}

bool DialogueSystem::update() {
    if (!dialogue.complete) {
        dialogue.timer += GetFrameTime();
        auto charsToShow = static_cast<size_t>(dialogue.timer / dialogue.charDelay);

        if (charsToShow >= dialogue.fullText.size() || IsKeyPressed(KEY_C)) {
            dialogue.visibleText = dialogue.fullText;
            dialogue.complete = true;
        } else {
            dialogue.visibleText = dialogue.fullText.substr(0, charsToShow);
        }

        if (std::isspace(dialogue.visibleText.back())) {
            return true;
        } else {
            return false;
        }
    }
    return false;
}

std::string DialogueSystem::getSpriteName() const {
    std::string word = dialogueLines.at(currentLine);
    word = word.substr(0, word.find(':'));
    //std::cout << word.substr(0, word.find(':')) << std::endl;
    return "../res/faces/" +  word + ".png";
}


std::string DialogueSystem::getSpriteAudio() const {
    std::string word = dialogueLines.at(currentLine);
    word = word.substr(0, word.find(':'));
    //std::cout << word.substr(0, word.find(':')) << std::endl;
    return "../res/faces/" +  word + ".mp3";
}

bool DialogueSystem::nextLine() {
    if (dialogue.complete && currentLine + 1 < dialogueLines.size()) {
        currentLine++;
        startLine(currentLine);
        return true;
    }
    return false;
}

void DialogueSystem::draw(const int x, const int y, const int fontSize, const Color color) const {
    Vector2 pos;
    pos.x = static_cast<float>(x);
    pos.y = static_cast<float>(y);
    DrawTextEx(gameFont,dialogue.visibleText.c_str(), pos, static_cast<float>(fontSize),gameFontSpacing, color);
}


std::string DialogueSystem::wrapText(const std::string &text) const {
    std::string wrappedText;
    int lineWidth = 0;
    const int spaceWidth = MeasureText(" ", 20);

    for (const char c: text) {
        if (const int charWidth = MeasureText(std::string(1, c).c_str(), 20);
            c == ' ' && lineWidth + charWidth > maxWidth) {
            wrappedText += '\n';
            wrappedText += '\n';
            lineWidth = 0;
        } else {
            wrappedText += c;
            lineWidth += c == ' ' ? spaceWidth : charWidth;
        }
    }
    return wrappedText;
}
