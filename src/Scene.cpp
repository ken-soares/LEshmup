//
// Created by ken-soares on 01/07/2024.
//

#include "Scene.h"

#include "patterns.h"
#include "raymath.h"
#include <iostream>
#include "rlgl.h"

#include <string>


void Scene::drawHUD() const {
    DrawRectangle(0, 0, screenWidth, 50, {0, 0, 0, 200});

    switch(player.getHealth()) {
        case 1:
            DrawRectangleGradientEx(Rectangle{100, 10, 500.0f * static_cast<float>(player.getHealth()) / 3, 20}, ORANGE, ORANGE,RED, RED);
            break;
        case 2:
            DrawRectangleGradientEx(Rectangle{100, 10, 500.0f * static_cast<float>(player.getHealth()) / 3, 20}, GREEN, GREEN,ORANGE, ORANGE);
            break;

        case 3:
            DrawRectangleGradientEx(Rectangle{100, 10, 500.0f * static_cast<float>(player.getHealth()) / 3, 20}, GREEN, GREEN,DARKGREEN, DARKGREEN);
            break;
    }


    DrawTextEx(gameFont,"Health" , {10, 10}, 20, gameFontSpacing, WHITE);
    DrawTextEx(gameFont,"Score" , {screenWidth-500, 10}, 20, gameFontSpacing, WHITE);
    DrawTextEx(gameFont,std::to_string(static_cast<int>(sceneScore)).c_str() , {screenWidth-430, 10}, 20, gameFontSpacing, WHITE);

    if(highScore < sceneScore) {
        DrawTextEx(gameFont,"Hi-Score" , {screenWidth-350, 10}, 20, gameFontSpacing, YELLOW);
        DrawTextEx(gameFont,std::to_string(static_cast<int>(sceneScore)).c_str() , {screenWidth-240, 10}, 20, gameFontSpacing, YELLOW);
    } else {
        DrawTextEx(gameFont,"Hi-Score" , {screenWidth-350, 10}, 20, gameFontSpacing, WHITE);
        DrawTextEx(gameFont,std::to_string(static_cast<int>(highScore)).c_str() , {screenWidth-240, 10}, 20, gameFontSpacing, WHITE);
    }

    for(int i = 0; i < player.getBombs(); i++) {
        DrawRectangle(610 + (i * 40),15,20,10,RED);
    }
}


void Scene::drawDebugInfo() const {
    DrawRectangle(screenWidth - 400, screenHeight-50, 400, 50, {0, 0, 0, 150});
    DrawText("Current Scene Position: ", screenWidth - 350, screenHeight-30, 15, WHITE);
    DrawText(std::to_string(static_cast<int>(scenePosition)).c_str(), screenWidth-160, screenHeight-30, 18, GRAY);
    DrawFPS(screenWidth - 100, screenHeight-30);
}

Scene::Scene([[maybe_unused]] const std::string& filepath, const std::list<enemyDef> &lp) : BaseScene(), background(filepath){

    // chargement de la premiere musique

    // backgroundMusic = LoadMusicStream();

    // variables nécessaires au screen shake
    shakeMagnitude = 0.0f;
    shakeDuration = 0.0f;
    shakeTimeLeft = 0.0f;

    shakeOffset.x = 0.0f;
    shakeOffset.y = 0.0f;

    // chargement des textures des ennemis
    enemySprites[0] = LoadTexture("../res/one.png");
    enemySprites[1] = LoadTexture("../res/redesign/naf0.png");
    enemySprites[2] = LoadTexture("../res/three.png");

    player.setHealth(std::stoi(readFile("../res/temp/lives.temp")));
    player.setBombs(std::stoi(readFile("../res/temp/bombs.temp")));

    listSpawn = lp;

    isDebugInfoVisible = false;

    sceneScore = std::stod(readFile("../res/temp/score.temp"));
    highScore = std::stod(readFile("../res/save/high.save"));

    std::cout << readFile("../res/temp/score.temp") << std::endl;

    bloom = LoadShader(nullptr, "../res/shaders/sobel.frag");
    outer_glow = LoadShader(nullptr, "../res/shaders/outer_glow.frag");
    target = LoadRenderTexture(screenWidth, screenHeight);


    PlayMusicStream(backgroundMusic);
}

Scene::~Scene() = default;


void Scene::UpdateScreenShake() {
    if (shakeTimeLeft > 0.0f) {
        shakeTimeLeft -= GetFrameTime();

        // Randomize the shake offset
        shakeOffset.x = (static_cast<float>(dist100(rng))/100 * 2 - 1) * shakeMagnitude;
        shakeOffset.y = (static_cast<float>(dist100(rng))/100  * 2 - 1) * shakeMagnitude;

        // Gradually reduce the magnitude over time (ease out)
        shakeMagnitude *= (shakeTimeLeft / shakeDuration);
    } else {
        // Reset the shake offset when the effect ends
        shakeOffset = { 0.0f, 0.0f };
    }
}
void Scene::StartScreenShake() {
    shakeMagnitude = 13.0;
    shakeDuration = 0.5;
    shakeTimeLeft = 0.5;
}

int Scene::update(const int nextSceneCount) {

    UpdateScreenShake();
    UpdateMusicStream(backgroundMusic);

    if(IsKeyPressed(KEY_F3)) {
        isDebugInfoVisible = !isDebugInfoVisible;
    }

    if(IsKeyPressed(KEY_X) && player.getBombs() > 0) {
        StartScreenShake();
        listBullets.clear();
        player.setBombs(player.getBombs() - 1);
        for (auto &e: listEnemies) {
            if(e->def.isBoss) {
                scenePosition += e->def.timer;
            }
            e->def.health = 0;

            if(e->def.health <= 0) {
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

        if (!player.wasShot && Vector2Length(Vector2Subtract(b.pos, player.getHitBoxVec())) < 3 * 3) {
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
                sceneScore += KILL_SCORE;

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
            std::cout << "Player hit an enemy" << std::endl;
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
        writeFile("../res/temp/score.temp", std::to_string(sceneScore));
        writeFile("../res/temp/lives.temp", std::to_string(player.getHealth()));
        writeFile("../res/temp/bombs.temp", std::to_string(player.getBombs()));
        return nextSceneCount + 1;
    }
    return nextSceneCount;
}

void Scene::draw() {
    ClearBackground(BLACK);

    rlPushMatrix();
    rlTranslatef(shakeOffset.x, shakeOffset.y, 0); // Apply shake offset
    background.draw();

    if(isDebugInfoVisible) {
        drawDebugInfo();
    }

    player.draw();
    rlPopMatrix();

    // affichage des tirs
    // tirs des ennemis
    BeginTextureMode(target);
    ClearBackground(BLANK);
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

    // affichage des particules d'explosion
    for (auto &p: listParticles) {
        DrawEllipse(static_cast<int>(p.pos.x), static_cast<int>(p.pos.y), 3.0f, 3.0f, p.color);
        DrawEllipse(static_cast<int>(p.pos.x) + 2, static_cast<int>(p.pos.y) - 2, 3.0f, 3.0f, {255, 161, 0, p.color.a});
        p.timer++;
        p.vel.x /= 1.05f;
        p.vel.y /= 1.05f;
        p.color.a -= 5;
    }
    EndTextureMode();

    BeginShaderMode(bloom);
    DrawTextureRec(target.texture, (Rectangle){0,0,(float)target.texture.width, (float)-target.texture.height}, (Vector2){0,0}, WHITE);
    EndShaderMode();


    // affichage de la texture de chaque ennemi
    for (auto &e: listEnemies) {
        DrawTexture(enemySprites[e->def.spriteID], static_cast<int>(e->getPos().x), static_cast<int>(e->getPos().y),
                    WHITE);
    }


    if (!player.getHealth()) {
        listEnemies.clear();
        listBullets.clear();
        DrawRectangle(0, 0, screenWidth, screenHeight, {255, 0, 0, 40});

        DrawTextEx(gameFont,"GAME OVER" , {static_cast<int>(screenWidth / 2.48f), screenHeight / 1.98f}, 45, gameFontSpacing, BLACK);
        DrawTextEx(gameFont,"GAME OVER" , {static_cast<int>(screenWidth / 2.5f), screenHeight / 2.0f}, 45, gameFontSpacing, WHITE);

        if(sceneScore > highScore) {
            std::cout << "wrote high score " << sceneScore << std::endl;
            writeFile("../res/save/high.save", std::to_string(static_cast<int>(sceneScore)));
        }
    }

    drawHUD();
}