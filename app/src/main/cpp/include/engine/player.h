#pragma once

#include "renderer.h"
#include "renderable.h"
#include "camera.h"

namespace DoodleJumpGame {
    class Player final : public Renderable {  // final = "sealed class"
    public:
        Player(float startX = 0, float startY = 0)
                : Renderable(startX, startY, 10.0f, 10.0f) {}

        void renderOn(Renderer *render, Camera camera) override;

        void update(float deltaTime) override;

        void jump();

    private:
        float velocityY = 0.0f;
        static constexpr float GRAVITY = -50.0f;    // Игровые единицы/сек²
        static constexpr float JUMP_FORCE = 25.0f;  // Прыжок на ~12.5 единиц вверх
    };
}