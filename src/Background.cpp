//
// Created by ken-soares on 01/04/2024.
//

#include "Background.h"

[[maybe_unused]]
Background::Background(const std::string& filepath) {
    scrollback = 0.0f;
    background = LoadTexture(filepath.c_str());
    grayscale = LoadShader(nullptr, "../res/shaders/dream_vision.frag");
}

Background::~Background() = default;

void Background::update() {
    scrollback -= 1.0f;

    // bien faire attention à ce que l'image de fond aie la meme largeur que la résolution d'écran
    if (-scrollback >= (float) background.width) { scrollback = 0.0f; }
}

void Background::draw() {

    BeginShaderMode(grayscale);
        DrawTextureEx(background, (Vector2){scrollback, 0}, 0.0f, 1.0f, WHITE);
        DrawTextureEx(background, (Vector2){(float) background.width + scrollback, 0}, 0.0f,
                      1.0f, WHITE);
    EndShaderMode();
}
