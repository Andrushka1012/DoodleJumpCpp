#pragma once

#include "renderer.h"
#include "renderable.h"

namespace DoodleJumpGame {
    class Player final : public Renderable {  // final = "sealed class"
    public:
        Player(float startX, float startY)
                : Renderable(startX, startY, 32.0f, 32.0f) {}

        // Реализуем интерфейс
        void renderOn(Renderer *render) override;

        void update(float deltaTime) override;

        void jump();

    private:
        float velocityY = 0.0f;
        static constexpr float GRAVITY = -9.8f;
        static constexpr float JUMP_FORCE = 15.0f;
    };
}