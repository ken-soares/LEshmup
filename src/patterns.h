//
// Created by Admin on 01/07/2024.
//

#ifndef PATTERNS_H
#define PATTERNS_H

#include "Enemy.h"
#include <cmath>
#include <random>

inline std::random_device rd;
inline std::mt19937 rng(rd());
inline std::uniform_int_distribution<std::mt19937::result_type> dist100(1,100);

inline auto move_none = [](Enemy &e) {
    e.setPos({e.getPos().x - sceneSpeed, e.getPos().y});
};

inline auto move_third = [](Enemy &e) {
    if(e.getPos().x >= screenWidth - static_cast<float>(screenWidth)/3) {
        e.setPos({e.getPos().x - sceneSpeed, e.getPos().y});
    }
};

inline auto set_third = [](Enemy &e) {
      e.setPos({static_cast<float>(2)*screenWidth/3, e.getPos().y});
};


inline auto move_basic = [](Enemy &e) {
    e.setPos({e.getPos().x - 1.25f * sceneSpeed, e.getPos().y});
};

inline auto move_fast = [](Enemy &e) {
    e.setPos({e.getPos().x - 3.0f * sceneSpeed, e.getPos().y});
};

inline auto move_sin_narrow = [](Enemy &e) {
    e.setPos({e.getPos().x - sceneSpeed, e.getPos().y});
    e.dataMove[0] += GetFrameTime();
    e.setPos({e.getPos().x, e.getPos().y + 2 * cosf(e.dataMove[0])});
};

// routes de tir
inline auto fire_none = [](Enemy &e, std::list<Bullet> &bullets) {
};

inline auto fire_Straight2 = [](Enemy &e, std::list<Bullet> &bullets) {
    constexpr float fDelay = 0.2f;
    e.dataFire[0] += GetFrameTime();

    if (e.dataFire[0] >= fDelay) {
        e.dataFire[0] = 0.0f;
        Bullet b{};
        b.pos = e.getPos();
        b.vel = {-15.0f, 0.0f};
        b.type = BULLET_LONG;
        bullets.push_back(b);
    }
};


inline auto fire_CirclePulse12 = [](Enemy &e, std::list<Bullet> &bullets) {
    constexpr float fDelay = 1.2f;
    constexpr int nBullets = 25;
    constexpr float fTheta = 2 * PI / nBullets;
    e.dataFire[0] += GetFrameTime();

    if (e.dataFire[0] >= fDelay) {
        e.dataFire[0] = 0.0f;
        for (int i = 0; i < nBullets; i++) {
            Bullet b{};
            b.pos = e.getPos();

            b.vel.x = 8.0f * cos(fTheta * static_cast<float>(i));
            b.vel.y = 8.0f * sinf(fTheta * static_cast<float>(i));

            b.type = BULLET_ROUND;
            bullets.push_back(b);
        }
    }
};

inline auto fire_SpiralReverse02 = [](Enemy &e, std::list<Bullet> &bullets) {
  constexpr float fDelay = 0.013f;
  e.dataFire[0] += GetFrameTime();
  const float offset_amount = 0.2f;

  if (e.dataFire[0] >= fDelay) {

      e.dataFire[3] += 0.5f;
      if(static_cast<int>(e.dataFire[3]) >= 50) {
          e.dataFire[2] = (e.dataFire[2] == 1) ? 0 : 1;
          e.dataFire[3] = 0.0f;
      }

      if(e.dataFire[2] == 1) {
          e.dataFire[1] -= offset_amount;
      } else {
         e.dataFire[1] += offset_amount;
      }

      e.dataFire[0] = 0.0f;
      Bullet b{};
      b.pos = e.getPos();
      b.vel.x = 3.0f * cos(e.dataFire[1]);
      b.vel.y = 3.0f * sinf(e.dataFire[1]);
      b.type = BULLET_ROUND;
      bullets.push_back(b);
  }
};

inline auto fire_CircleConstUpdate04 = [](Enemy &e, std::list<Bullet> &bullets) {
  e.dataFire[1] += 0.1;
  constexpr float fDelay = 0.4f;
  constexpr int nBullets = 25;
  constexpr float fTheta = (2 * PI / nBullets);
  e.dataFire[0] += GetFrameTime();

  if (e.dataFire[0] >= fDelay) {
      e.dataFire[0] = 0.0f;
      for (int i = 0; i < nBullets; i++) {
          Bullet b{};
          b.pos = e.getPos();
          b.vel.x = 8.0f * cos(fTheta * static_cast<float>(i) + e.dataFire[1]);
          b.vel.y = 8.0f * sinf(fTheta * static_cast<float>(i) + e.dataFire[1]);
          b.type = BULLET_ROUND;
          bullets.push_back(b);
      }
  }
};

inline auto fire_CirclePulseRand05 = [](Enemy &e, std::list<Bullet> &bullets) {
  constexpr float fDelay = 0.5f;
  constexpr int nBullets = 60;
  constexpr float fTheta = 2 * PI / nBullets;
  e.dataFire[0] += GetFrameTime();

  if (e.dataFire[0] >= fDelay) {
      e.dataFire[0] = 0.0f;
      for (int i = 0; i < nBullets; i++) {
          Bullet b{};
          b.pos = e.getPos();
          b.vel.x = 8.0f * cos(fTheta * static_cast<float>(i)) + static_cast<float>(dist100(rng)) * 0.1f;
          b.vel.y = 8.0f * sinf(fTheta * static_cast<float>(i)) + static_cast<float>(dist100(rng)) * 0.05f;
          b.type = BULLET_ROUND;
          bullets.push_back(b);
      }
  }
};
#endif //PATTERNS_H
