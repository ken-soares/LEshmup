//
// Created by Admin on 01/07/2024.
//

#include "Scene.h"

#include <utility>
#include "patterns.h"
#include "raymath.h"

void drawHUD(const Player &player) {
    DrawText("Health", 10, 10, 20, WHITE);
    DrawRectangleGradientEx(Rectangle{80, 10, 1000.0f * static_cast<float>(player.getHealth()) / 3, 20}, GREEN, GREEN,
                            DARKGREEN, DARKGREEN);

    if (!IsCursorHidden()) {
        HideCursor();
    }
}

Scene::Scene(std::string bg_path, const std::list<enemyDef> &lp) : background(std::move(bg_path)) {
    // chargement des textures des ennemis
    enemySprites[0] = LoadTexture("../res/one.png");
    enemySprites[1] = LoadTexture("../res/two.png");
    enemySprites[2] = LoadTexture("../res/three.png");

    listSpawn = lp;
}

Scene::~Scene() = default;


int Scene::update(int nextSceneCount) {
    player.update();
    background.update();

    scenePosition += sceneSpeed;

    // apparition des ennemis aux moments adaptés
    while (!listSpawn.empty() && scenePosition >= listSpawn.front().triggerTime) {
        std::shared_ptr<Enemy> e(nullptr);
        e = std::make_shared<Enemy>(listSpawn.front());
        listSpawn.pop_front();
        listEnemies.push_back(e);
    }

    // mise à jour des particules
    for (auto &p: listParticles) {
        p.pos = Vector2Add(p.pos, p.vel);
    }

    // mise à jour de chaque ennemi
    for (auto &e: listEnemies) {
        e->update(listBullets);
    }

    // mise à jour des tirs
    for (auto &b: listBullets) {
        b.pos = Vector2Add(b.pos, b.vel);

        if (!player.wasShot && Vector2Length(Vector2Subtract(b.pos, player.getHitBoxVec())) < 4 * 4) {
            std::cout << "Player was shot" << std::endl;
            b.remove = true;
            player.setHealth(player.getHealth() - 1);
            player.wasShot = true;
        }
    }

    // mise à jour des tirs du joueur
    for (auto &b: player.bullets) {
        b.pos = Vector2Add(b.pos, b.vel);

        for (auto &e: listEnemies) {
            if ((b.pos.x > e->getPos().x && b.pos.x < e->getPos().x + static_cast<float>(enemySprites[e->def.spriteID].
                     width)) &&
                (b.pos.y > e->getPos().y && b.pos.y < e->getPos().y + static_cast<float>(enemySprites[e->def.spriteID].
                     height))) {
                b.remove = true;

                e->def.health--;

                if (e->def.health <= 0) {
                    for (int i = 0; i < 200; i++) {
                        // coordonnées polaires du vecteur de chaque particule
                        float fAngle = (float) (static_cast<float>(dist100(rng)) / 100) * 2 * PI;
                        float fVel = (float) (static_cast<float>(dist100(rng)) / 100) * 2.0f + 5.0f;

                        listParticles.push_back(Particle{
                            false, Vector2Add(e->getPos(), Vector2{24.0f, 24.0f}),
                            {fVel * cosf(fAngle), fVel * sinf(fAngle)}, 0, (Color){253, 249, 0, 255}
                        });
                    }
                }
            }
        }
    }

    // mise à jour des collisions joueur/ennemi
    for (auto &e: listEnemies) {
        // condition aussi grosse que ta daronne
        if(!player.wasShot && ((player.getHitBoxVec().x >= e->getPos().x && player.getHitBoxVec().x <= e->getPos().x + static_cast<float>(enemySprites[e->def.spriteID].
                     width)) && (player.getHitBoxVec().y >= e->getPos().y && player.getHitBoxVec().y <= e->getPos().y + static_cast<float>(enemySprites[e->def.spriteID].
                     height)))) {
            std::cout << "Player hit an ennemy" << std::endl;
            player.setHealth(player.getHealth() - 1);
            player.wasShot = true;
        }

    }

    // retrait de chaque ennemi
    listEnemies.remove_if([&](const std::shared_ptr<Enemy> &e) {return (e->getPos().x + static_cast<float>(enemySprites[e->def.spriteID].width)) <= 0 || e->def.health <= 0 || e->def.timer <= 0;});

    // retrait des tirs
    listBullets.remove_if([&](const Bullet &b) {return b.pos.x < 0 || b.pos.x > screenWidth || b.pos.y < 0 || b.pos.y > screenHeight || b.remove;});
    player.bullets.remove_if([&](const Bullet &b) { return b.pos.x < 0 || b.pos.x > screenWidth || b.pos.y < 0 || b.pos.y > screenHeight || b.remove;});
    // retrait des particules d'explosion
    listParticles.remove_if([&](const Particle &p) {return p.timer >= 50 || p.pos.x < 0 || p.pos.x > screenWidth || p.pos.y < 0 || p.pos.y >screenHeight || p.remove;});


    if (player.getHealth() > 0 && listSpawn.empty() && listEnemies.empty()) {
        return nextSceneCount + 1;
    }
    return nextSceneCount;
}

void Scene::draw() {
    ClearBackground(BLACK);
    background.draw();
    drawHUD(player);
    DrawFPS(screenWidth - 100, 20);
    player.draw();

    // affichage des tirs

    // tirs des ennemis
    for (auto &b: listBullets) {
        switch (b.type) {
            case BULLET_LONG:
                DrawEllipse(static_cast<int>(b.pos.x), static_cast<int>(b.pos.y), 22.0f, 7.0f, MAROON);
                DrawEllipse(static_cast<int>(b.pos.x), static_cast<int>(b.pos.y), 20.0f, 5.0f, RED);
                break;
            case BULLET_ROUND:
                DrawEllipse(static_cast<int>(b.pos.x), static_cast<int>(b.pos.y), 6.0f, 6.0f, RED);
                DrawEllipse(static_cast<int>(b.pos.x), static_cast<int>(b.pos.y), 5.0f, 5.0f, MAROON);
                break;
            default:
                break;
        }
    }

    // tirs du joueur
    for (auto &b: player.bullets) {
        DrawEllipse(static_cast<int>(b.pos.x), static_cast<int>(b.pos.y), 17.0f, 7.0f, RAYWHITE);
        DrawEllipse(static_cast<int>(b.pos.x), static_cast<int>(b.pos.y), 15.0f, 5.0f, SKYBLUE);
    }

    // affichage de la texture de chaque ennemi
    for (auto &e: listEnemies) {
        DrawTexture(enemySprites[e->def.spriteID], static_cast<int>(e->getPos().x), static_cast<int>(e->getPos().y),
                    WHITE);
    }

    // affichage des particules d'explosion
    for (auto &p: listParticles) {
        DrawEllipse(static_cast<int>(p.pos.x), static_cast<int>(p.pos.y), 3.0f, 3.0f, p.color);
        DrawEllipse(static_cast<int>(p.pos.x) + 2, static_cast<int>(p.pos.y) - 2, 3.0f, 3.0f, {255, 161, 0, p.color.a});
        p.timer++;
        p.vel.x /= 1.05f;
        p.vel.y /= 1.05f;
        p.color.a -= 5;
    }

    if (!player.getHealth()) {
        listEnemies.clear();
        listBullets.clear();
        DrawRectangle(0, 0, screenWidth, screenHeight, {255, 0, 0, 40});
        DrawText("GAME OVER", screenWidth / 2.25f, screenHeight / 2, 35, WHITE);
    }
}
