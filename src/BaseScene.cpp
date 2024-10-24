//
// Created by ken-soares on 06/07/2024.
//

#include "BaseScene.h"
#include <fstream>
#include <iostream>

BaseScene::BaseScene() {

    // CREDIT Hewett Tsoi
    gameFont = LoadFont("../res/alagard.ttf");
}

BaseScene::~BaseScene() = default;

void BaseScene::draw() {}

int BaseScene::update(const int count) {
    return count;
}

void BaseScene::writeFile(const std::string& filepath, const std::string& contents) {
    std::ofstream writer(filepath);

    if(!writer) {
        std::cout << "Error opening file for output" << std::endl;
    }

    writer << contents << std::endl;

    writer.close();

}

std::string BaseScene::readFile(const std::string& filepath) {
    std::ifstream reader(filepath);
    std::string contents;
    std::string line;

    if(!reader) {
        std::cout << "Error opening file for input" << std::endl;
    }

    while(std::getline(reader,line)) {
        contents.append(line + "\n");
    }
    reader.close();

    return contents;
}

bool BaseScene::exists(const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}

