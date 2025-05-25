#pragma once

#include "renderer.h"
#include "renderable.h"
#include "camera.h"
#include "constants.h"

namespace DoodleJumpGame {
    class Player final : public Renderable {  // final = "sealed class"
    public:
        Player(float startX = 0, float startY = 0)
                : Renderable(startX, startY, GameConstants::PLAYER_SIZE, GameConstants::PLAYER_SIZE) {}

        void renderOn(Renderer *render, Camera camera) override;

        void update(float deltaTime) override;

        void jump();

    private:
        float velocityY = 0.0f;
    };
}