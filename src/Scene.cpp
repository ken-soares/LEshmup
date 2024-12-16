//
// Created by ken-soares on 01/07/2024.
//

#include "Scene.h"

#include "patterns.h"
#include "raymath.h"
#include <iostream>
#include <cmath>
#include "rlgl.h"

#include <string>

void Scene::drawHUD() const {
    DrawRectangle(0, 0, screenWidth, 50, {0, 0, 0, 200});
    drawHealthHUD();
    drawScoreHUD();
    drawBombsHUD();
    drawCauldronHUD();
}

void Scene::drawBombsHUD() const {
    DrawTextEx(gameFont, "B", {630, 2}, 25, gameFontSpacing, WHITE);
    for (int i = 0; i < player.getBombs(); i++) {
        DrawRectangle(660 + i * 40, 8, 20, 10,RED);
    }
}

void Scene::drawScoreHUD() const {
    DrawTextEx(gameFont, "Score", {screenWidth - 500, 10}, 30, gameFontSpacing, WHITE);
    DrawTextEx(gameFont, std::to_string(static_cast<int>(sceneScore)).c_str(), {screenWidth - 430, 10}, 30,
               gameFontSpacing, WHITE);

    if (highScore < sceneScore) {
        DrawTextEx(gameFont, "Hi-Score", {screenWidth - 350, 10}, 30, gameFontSpacing, YELLOW);
        DrawTextEx(gameFont, std::to_string(static_cast<int>(sceneScore)).c_str(), {screenWidth - 240, 10}, 30,
                   gameFontSpacing, YELLOW);
    } else {
        DrawTextEx(gameFont, "Hi-Score", {screenWidth - 350, 10}, 30, gameFontSpacing, WHITE);
        DrawTextEx(gameFont, std::to_string(static_cast<int>(highScore)).c_str(), {screenWidth - 240, 10}, 30,
                   gameFontSpacing, WHITE);
    }
}

void Scene::drawCauldronHUD() const {
    DrawTextEx(gameFont, "C", {630, 22}, 25, gameFontSpacing, WHITE);
    auto cauldron_color = GRAY;
    cauldron_color.a -= 150;
    for (int i = 0; i < 3; i++) {
        switch (player.getCauldron(i)) {
            case NONE:
                DrawRectangle(660 + i * 40, 30, 20, 10, cauldron_color);
                break;
            case ONE_UP:
                DrawRectangle(660 + i * 40, 30, 20, 10,GREEN);
                break;
            case BOMB:
                DrawRectangle(660 + i * 40, 30, 20, 10,ORANGE);
                break;
            case UPGRADE:
                DrawRectangle(660 + i * 40, 30, 20, 10,WHITE);
                break;
            case SHIELD:
                DrawRectangle(660 + i * 40, 30, 20, 10,BLUE);
                break;
            default:
                std::cout << "error in the buffs" << std::endl;
                break;
        }
    }
}

void Scene::drawHealthHUD() const {
    switch (player.getHealth()) {
        case 1:
            DrawRectangleGradientEx(Rectangle{100, 10, 500.0f * static_cast<float>(player.getHealth()) / 3, 15}, ORANGE,
                                    ORANGE,RED, RED);
            break;
        case 2:
            DrawRectangleGradientEx(Rectangle{100, 10, 500.0f * static_cast<float>(player.getHealth()) / 3, 15}, GREEN,
                                    GREEN,ORANGE, ORANGE);
            break;

        case 3:
            DrawRectangleGradientEx(Rectangle{100, 10, 500.0f * static_cast<float>(player.getHealth()) / 3, 15}, GREEN,
                                    GREEN,DARKGREEN, DARKGREEN);
            break;
        default:
            DrawRectangleGradientEx(Rectangle{100, 10, 500.0f * static_cast<float>(player.getHealth()) / 3, 15}, ORANGE,
                                    ORANGE,RED, RED);
            break;
    }


    DrawTextEx(gameFont, "Health", {10, 10}, 20, gameFontSpacing, WHITE);

}

void Scene::drawDebugInfo() const {
    DrawRectangle(screenWidth - 400, screenHeight - 50, 400, 50, {0, 0, 0, 150});
    DrawText("Current Scene Position: ", screenWidth - 350, screenHeight - 30, 15, WHITE);
    DrawText(std::to_string(static_cast<int>(scenePosition)).c_str(), screenWidth - 160, screenHeight - 30, 18, GRAY);
    DrawFPS(screenWidth - 100, screenHeight - 30);
}

Scene::Scene([[maybe_unused]] const std::string &filepath, const std::list<enemyDef> &lp) : background(filepath) {
    //TODO: chargement de la premiere musique
    //backgroundMusic = LoadMusicStream("../res/flow_state.mp3");

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

    pickUpScore = 0;

    isDebugInfoVisible = false;

    sceneScore = std::stod(readFile("../res/temp/score.temp"));
    highScore = std::stod(readFile("../res/save/high.save"));

    std::cout << readFile("../res/temp/score.temp") << std::endl;

    bloom = LoadShader(nullptr, "../res/shaders/sobel.frag");
    outer_glow = LoadShader(nullptr, "../res/shaders/outer_glow.frag");
    target = LoadRenderTexture(screenWidth, screenHeight);


    //TODO: réactiver quand prêt
    //PlayMusicStream(backgroundMusic);
}

Scene::~Scene() {
    for (const auto &enemySprite: enemySprites) {
        UnloadTexture(enemySprite);
    }

    UnloadShader(bloom);
    UnloadShader(outer_glow);

    UnloadRenderTexture(target);
    //UnloadMusicStream(backgroundMusic);
}


void Scene::UpdateScreenShake() {
    if (shakeTimeLeft > 0.0f) {
        shakeTimeLeft -= GetFrameTime();

        // Randomize the shake offset
        shakeOffset.x = (static_cast<float>(dist100(rng)) / 100 * 2 - 1) * shakeMagnitude;
        shakeOffset.y = (static_cast<float>(dist100(rng)) / 100 * 2 - 1) * shakeMagnitude;

        // Gradually reduce the magnitude over time (ease out)
        shakeMagnitude *= shakeTimeLeft / shakeDuration;
    } else {
        // Reset the shake offset when the effect ends
        shakeOffset = {0.0f, 0.0f};
    }
}

void Scene::StartScreenShake() {
    shakeMagnitude = 13.0;
    shakeDuration = 0.5;
    shakeTimeLeft = 0.5;
}

void Scene::useBomb() {
    if (IsKeyPressed(KEY_X) && player.getBombs() > 0) {
        sceneScore += 500;
        StartScreenShake();
        listBullets.clear();
        player.setBombs(player.getBombs() - 1);
        for (const auto &e: listEnemies) {
            if (e->def.isBoss) {
                scenePosition += e->def.timer;
            }
            e->def.health = 0;

            if (e->def.health <= 0) {
                for (int i = 0; i < 200; i++) {
                    // coordonnées polaires du vecteur de chaque particule
                    const float fAngle = static_cast<float>(dist100(rng)) / 100 * 2 * PI;
                    const float fVel = static_cast<float>(dist100(rng)) / 100 * 2.0f + 5.0f;

                    listParticles.push_back(Particle{
                        false, Vector2Add(e->getPos(), Vector2{24.0f, 24.0f}),
                        {fVel * cosf(fAngle), fVel * sinf(fAngle)}, 0, (Color){253, 249, 0, 255}
                    });
                }
            }
        }
    }
}

void Scene::updatePlayerShot() {
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
                pickUpScore += KILL_SCORE;

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
}

void Scene::listRemove() {
    // retrait de chaque ennemi
    listEnemies.remove_if([&](const std::shared_ptr<Enemy> &e) {
        return e->getPos().x + static_cast<float>(enemySprites[e->def.spriteID].width) <= 0 || e->def.health <= 0 || e
               ->def.timer <= 0;
    });

    // retrait des tirs
    listBullets.remove_if([&](const Bullet &b) {
        return b.pos.x < 0 || b.pos.x > screenWidth || b.pos.y < 0 || b.pos.y > screenHeight || b.remove;
    });
    player.bullets.remove_if([&](const Bullet &b) {
        return b.pos.x < 0 || b.pos.x > screenWidth || b.pos.y < 0 || b.pos.y > screenHeight || b.remove;
    });
    // retrait des particules d'explosion
    listParticles.remove_if([&](const Particle &p) {
        return p.timer >= 50 || p.pos.x < 0 || p.pos.x > screenWidth || p.pos.y < 0 || p.pos.y > screenHeight || p.
               remove;
    });
    // retrait des bonus
    listPickups.remove_if([&](const Pickup &p) { return p.pos.x < 0 || p.timer <= 0 || p.remove == true; });
}

void Scene::spawnPickup() {
    if (pickUpScore > PICKUP_LIMIT && listPickups.size() < MAX_ONSCREEN_PICKUP) {
        pickUpScore = 0;
        listPickups.push_back(Pickup{
            false,
            {screenWidth / 3.0f, 50},
            {static_cast<float>(dist100(rng)) / 54.0f + 2, static_cast<float>(dist100(rng)) / 50.0f},
            static_cast<PickupType>(dist4(rng)),
            PICKUP_TIMER,
            false,
        });
    }
}

void Scene::updatePickupsPlayerCol() {
    // mise à jour des pickup
    for (auto &p: listPickups) {
        p.pos = Vector2Add(p.pos, p.vel);
        if (Vector2Length(Vector2Subtract(p.pos, player.getHitBoxVec())) < 8 * 8) {
            p.vel.x = static_cast<float>(150 * ((player.getHitBoxVec().x - p.pos.x) / pow(
                                                    Vector2Length(Vector2Subtract(p.pos, player.getHitBoxVec())), 2)));
            p.vel.y = static_cast<float>(150 * ((player.getHitBoxVec().y - p.pos.y) / pow(
                                                    Vector2Length(Vector2Subtract(p.pos, player.getHitBoxVec())), 2)));
        }

        if (Vector2Length(Vector2Subtract(p.pos, player.getHitBoxVec())) < 3 * 3) {
            std::cout << "Player got a buff" << std::endl;
            p.remove = true;
            player.setCauldron(p.type);
        }
    }
}

void Scene::viewDebugInfo() {
    if (IsKeyPressed(KEY_F3)) {
        isDebugInfoVisible = !isDebugInfoVisible;
    }
}


void Scene::spawnEnemies() {
    // apparition des ennemis aux moments adaptés
    while (!listSpawn.empty() && scenePosition >= listSpawn.front().triggerTime) {
        std::shared_ptr<Enemy> e(nullptr);
        e = std::make_shared<Enemy>(listSpawn.front());
        listSpawn.pop_front();
        listEnemies.push_back(e);
    }
}

void Scene::updateEnemyShots() {
    // mise à jour des tirs
    for (auto &b: listBullets) {
        b.pos = Vector2Add(b.pos, b.vel);
        if (!player.shieldOn && !player.wasShot && Vector2Length(Vector2Subtract(b.pos, player.getHitBoxVec())) < 3 *
            3) {
            std::cout << "Player was shot" << std::endl;
            b.remove = true;
            player.setHealth(player.getHealth() - 1);
            player.wasShot = true;
        }
    }
}

void Scene::updatePlayerEnemyCol() {
    // mise à jour des collisions joueur/ennemi
    for (const auto &e: listEnemies) {
        // condition aussi grosse que ta daronne
        if (!player.shieldOn && !player.wasShot && (
                player.getHitBoxVec().x >= e->getPos().x && player.getHitBoxVec().x <= e->getPos().x + static_cast<
                    float>(enemySprites[e->def.spriteID].
                    width) && (player.getHitBoxVec().y >= e->getPos().y && player.getHitBoxVec().y <= e->getPos().y +
                               static_cast<float>(enemySprites[e->def.spriteID].
                                   height)))) {

            std::cout << "Player hit an enemy" << std::endl;
            player.setHealth(player.getHealth() - 1);
            player.wasShot = true;
        }
    }
}

void Scene::updatePickups() {
    // mise a jour des pickups
    for (auto &p: listPickups) {
        p.pos = Vector2Add(p.pos, p.vel);
        if (p.pos.y <= 50 || p.pos.y >= screenHeight) {
            p.vel.y *= -1;
        }
        if (p.pos.x >= screenWidth) {
            p.vel.x *= -1;
        }

        p.timer -= 3 * GetFrameTime();
    }
}

void Scene::updateParticles() {
    // mise à jour des particules
    for (auto &p: listParticles) {
        p.pos = Vector2Add(p.pos, p.vel);
    }
}

void Scene::updateEnemies() {
    // mise à jour de chaque ennemi
    for (const auto &e: listEnemies) {
        e->update(listBullets);
    }
}

int Scene::update(const int nextSceneCount) {
    spawnPickup();
    UpdateScreenShake();
    UpdateMusicStream(backgroundMusic);
    viewDebugInfo();
    useBomb();
    player.update();
    background.update();
    scenePosition += sceneSpeed;
    spawnEnemies();
    updatePickups();
    updatePlayerShot();
    updateParticles();
    updateEnemies();
    updateEnemyShots();
    updatePickupsPlayerCol();
    updatePlayerEnemyCol();
    listRemove();

    if (player.getHealth() > 0 && listSpawn.empty() && listEnemies.empty()) {
        std::cout << "Debug: nextSceneCount = " << nextSceneCount << std::endl;
        std::cout << "Debug: player health = " << player.getHealth() << std::endl;
        std::cout << "Debug: listSpawn.size() = " << listSpawn.size() << std::endl;
        std::cout << "Debug: listEnemies.size() = " << listEnemies.size() << std::endl;

       writeFile("../res/temp/score.temp", std::to_string(sceneScore));
        writeFile("../res/temp/lives.temp", std::to_string(player.getHealth()));
        writeFile("../res/temp/bombs.temp", std::to_string(player.getBombs()));
        return nextSceneCount + 1;
    }
    return nextSceneCount;
}


void Scene::drawPickups() {
    for (auto &p: listPickups) {
        switch (p.type) {
            case ONE_UP:
                DrawEllipse(static_cast<int>(p.pos.x), static_cast<int>(p.pos.y), 12.0f, 12.0f, RAYWHITE);
                DrawEllipse(static_cast<int>(p.pos.x), static_cast<int>(p.pos.y), 10.0f, 10.0f, GREEN);
                break;
            case BOMB:
                DrawEllipse(static_cast<int>(p.pos.x), static_cast<int>(p.pos.y), 12.0f, 12.0f, RAYWHITE);
                DrawEllipse(static_cast<int>(p.pos.x), static_cast<int>(p.pos.y), 10.0f, 10.0f, ORANGE);
                break;
            case SHIELD:
                DrawEllipse(static_cast<int>(p.pos.x), static_cast<int>(p.pos.y), 12.0f, 12.0f, RAYWHITE);
                DrawEllipse(static_cast<int>(p.pos.x), static_cast<int>(p.pos.y), 10.0f, 10.0f, BLUE);
                break;
            case UPGRADE:
                DrawEllipse(static_cast<int>(p.pos.x), static_cast<int>(p.pos.y), 12.0f, 12.0f, BLACK);
                DrawEllipse(static_cast<int>(p.pos.x), static_cast<int>(p.pos.y), 10.0f, 10.0f, WHITE);
                break;
            case NONE:
                p.remove = true;
                break;
            default:
                break;
        }
    }
}

void Scene::draw() {
    ClearBackground(BLACK);
    rlPushMatrix();
    rlTranslatef(shakeOffset.x, shakeOffset.y, 0); // Apply shake offset
    background.draw();
    if (isDebugInfoVisible) {
        drawDebugInfo();
    }
    player.draw();
    rlPopMatrix();
    drawPickups();

    // affichage de tous les tirs
    // ceux des ennemis
    BeginTextureMode(target);
    ClearBackground(BLANK);

    drawBullets();

    // tirs du joueur
    for (const auto &b: player.bullets) {
        DrawEllipse(static_cast<int>(b.pos.x), static_cast<int>(b.pos.y), 17.0f, 7.0f, RAYWHITE);
        DrawEllipse(static_cast<int>(b.pos.x), static_cast<int>(b.pos.y), 15.0f, 5.0f, SKYBLUE);
    }

    // affichage des particules d'explosion
    drawParticles();
    EndTextureMode();

    BeginShaderMode(bloom);
    DrawTextureRec(target.texture, (Rectangle){
                       0, 0, static_cast<float>(target.texture.width), static_cast<float>(-target.texture.height)
                   }, (Vector2){0, 0}, WHITE);
    EndShaderMode();
    // affichage de la texture de chaque ennemi
    for (const auto &e: listEnemies) {
        DrawTexture(enemySprites[e->def.spriteID], static_cast<int>(e->getPos().x), static_cast<int>(e->getPos().y),
                    WHITE);
    }
    drawGameOver();
    drawHUD();
}


void Scene::drawBullets() const {
    for (const auto &b: listBullets) {
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
}
void Scene::drawParticles() {
    for (auto &p: listParticles) {
        DrawEllipse(static_cast<int>(p.pos.x), static_cast<int>(p.pos.y), 3.0f, 3.0f, p.color);
        DrawEllipse(static_cast<int>(p.pos.x) + 2, static_cast<int>(p.pos.y) - 2, 3.0f, 3.0f, {255, 161, 0, p.color.a});
        p.timer++;
        p.vel.x /= 1.05f;
        p.vel.y /= 1.05f;
        p.color.a -= 5;
    }
}

void Scene::drawGameOver() {
    if (!player.getHealth()) {
        listEnemies.clear();
        listBullets.clear();
        DrawRectangle(0, 0, screenWidth, screenHeight, {255, 0, 0, 40});

        DrawTextEx(gameFont, "GAME OVER", {static_cast<int>(screenWidth / 2.48f), screenHeight / 1.98f}, 45,
                   gameFontSpacing, BLACK);
        DrawTextEx(gameFont, "GAME OVER", {static_cast<int>(screenWidth / 2.5f), screenHeight / 2.0f}, 45,
                   gameFontSpacing, WHITE);

        if (sceneScore > highScore) {
            std::cout << "wrote high score " << sceneScore << std::endl;
            writeFile("../res/save/high.save", std::to_string(static_cast<int>(sceneScore)));
        }
    }
}