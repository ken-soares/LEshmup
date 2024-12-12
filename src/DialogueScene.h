//
// Created by ken-soares on 11/12/2024.
//

#ifndef DIALOGUESCENE_H
#define DIALOGUESCENE_H
#include "BaseScene.h"
#include "DialogueSystem.h"


class DialogueScene final : public BaseScene {
public:
    DialogueScene(const std::string &Filepath);
    ~DialogueScene() override;

    int update(int _count) override;
    void draw() override;

private:
    DialogueSystem dialogueSystem;

};



#endif //DIALOGUESCENE_H
