//
// Created by ken-soares on 06/07/2024.
//

#ifndef BASESCENE_H
#define BASESCENE_H

#include "raylib.h"
#include <string>

class BaseScene {
public:
    BaseScene();
    virtual ~BaseScene();
    virtual void draw();
    virtual int update(int count);
    static void writeFile(const std::string& filepath, const std::string& contents);
    static std::string readFile(const std::string& filepath);
    static bool exists(const std::string& name);
    Font gameFont{};
    Music backgroundMusic{};
};



#endif //BASESCENE_H
