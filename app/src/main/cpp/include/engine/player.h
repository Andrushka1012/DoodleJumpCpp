#pragma once

#include "renderer.h"
#include "renderable.h"
#include "camera.h"

namespace DoodleJumpGame {
    class Player final : public Renderable {  // final = "sealed class"
    public:
        Player(float startX = 0, float startY = 0)
                : Renderable(startX, startY, 32.0f, 32.0f) {}

        void renderOn(Renderer *render, Camera camera) override;

        void update(float deltaTime) override;

        void jump();

    private:
        float velocityY = 0.0f;
        static constexpr float GRAVITY = -9.8f;
        static constexpr float JUMP_FORCE = 5.0f;
    };
}