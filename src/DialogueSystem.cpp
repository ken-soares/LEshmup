//
// Created by ken-soares on 11/12/2024.
//

#include "DialogueSystem.h"
#include <iostream>

DialogueSystem::DialogueSystem(const std::vector<std::string> &lines, const int width,
                               const float delay): dialogueLines(lines), currentLine(0), maxWidth(width) {
    dialogue.charDelay = delay;
    startLine(currentLine);
}

void DialogueSystem::startLine(const size_t lineIndex) {
    if (lineIndex < dialogueLines.size()) {
        dialogue.fullText = wrapText(dialogueLines[lineIndex]);
        dialogue.visibleText = "";
        dialogue.timer = 0.0f;
        dialogue.complete = false;
    }
}

void DialogueSystem::update() {
    if (!dialogue.complete) {
        dialogue.timer += GetFrameTime();
        auto charsToShow = static_cast<size_t>(dialogue.timer / dialogue.charDelay);

        // Débogage des valeurs importantes
        std::cout << "Timer: " << dialogue.timer
                  << ", CharsToShow: " << charsToShow
                  << ", FullTextSize: " << dialogue.fullText.size()
                  << ", FullText: " << dialogue.fullText << std::endl;

        if (charsToShow >= dialogue.fullText.size() || IsKeyPressed(KEY_C)) {
            dialogue.visibleText = dialogue.fullText;
            dialogue.complete = true;
        } else {
            dialogue.visibleText = dialogue.fullText.substr(0, charsToShow);
        }
    }
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
    DrawText(dialogue.visibleText.c_str(), x, y, fontSize, color);
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
