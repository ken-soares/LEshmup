//
// Created by ken-soares on 01/07/2024.
//

#ifndef SCENE_H
#define SCENE_H

#include "Globals.h"
#include "raylib.h"
#include "Player.h"
#include "Background.h"
#include "Enemy.h"
#include <memory>

#include "BaseScene.h"

class Scene final : public BaseScene {
public:
    explicit Scene([[maybe_unused]] const std::string& filepath, const std::list<enemyDef>& lp);

    ~Scene() override;

    int update(int nextSceneCount) override;

    void draw() override;

    void drawDebugInfo() const;

    void drawHUD() const;

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

    std::list<Pickup> listPickups;


    bool isDebugInfoVisible;

    double sceneScore;
    double highScore;

private:
    void StartScreenShake();
    void UpdateScreenShake();
    Texture2D enemySprites[3]{};
    Background background;
    Shader bloom{};
    Shader outer_glow{};
    Shader shake{};
    float shakeIntensity = 0.0f;
    const float maxShakeIntensity = 0.02f;
    bool shaking = false;
    RenderTexture2D target{};

    float shakeMagnitude;
    float shakeDuration;
    float shakeTimeLeft;
    Vector2 shakeOffset{};
};



#endif //SCENE_H
