//
// Created by Admin on 01/07/2024.
//

#ifndef SCENE_H
#define SCENE_H

#include "Globals.h"
#include "raylib.h"
#include "Player.h"
#include "Background.h"
#include "Enemy.h"
#include <memory>

class Scene {
public:
    Scene(std::string bg_path, const std::list<enemyDef>& lp);
    ~Scene();
    int update(int nextSceneCount);
    void draw();
    Player player;
    float scenePosition = 0.0f;

    // liste des specifications d'ennemis
    std::list<enemyDef> listSpawn;

    // liste des ennemis présents dans l'écran
    std::list<std::shared_ptr<Enemy>> listEnemies;

    // liste des particules d'explosion
    std::list<Particle> listParticles;

    // liste des tirs
    std::list<Bullet> listBullets;
private:
    Texture2D enemySprites[3]{};
    Background background;
};



#endif //SCENE_H
