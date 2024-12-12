//
// Created by ken-soares on 11/12/2024.
//

#ifndef DIALOGUESYSTEM_H
#define DIALOGUESYSTEM_H

#include "raylib.h"
#include <string>
#include <vector>

struct Dialogue {
    std::string fullText;  // Texte complet à afficher
    std::string visibleText; // Texte actuellement visible
    float timer;          // Timer pour l'apparition progressive
    float charDelay;      // Délai entre chaque caractère
    bool complete;        // Si le texte est complètement affiché
};

class DialogueSystem {
public:
    std::vector<std::string> dialogueLines;
    size_t currentLine;
    Dialogue dialogue;
    int maxWidth;

    DialogueSystem(const std::vector<std::string>& lines, int width, float delay);
    void startLine(size_t lineIndex);
    void update();
    bool nextLine();
    void draw(int x, int y, int fontSize, Color color) const;
private:
    [[nodiscard]] std::string wrapText(const std::string& text) const;
};



#endif //DIALOGUESYSTEM_H
