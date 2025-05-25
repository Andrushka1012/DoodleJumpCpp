#pragma once

#include "renderer.h"
#include "renderable.h"
#include "camera.h"
#include "constants.h"

namespace DoodleJumpGame {
    class Player final : public Renderable {  // final = "sealed class"
    public:
        Player(float startX = 0, float startY = 0)
                : Renderable(startX, startY, GameConstants::PLAYER_WIDTH, GameConstants::PLAYER_HEIGHT) {}

        void renderOn(Renderer *render, Camera camera) const override;

        void update(float deltaTime) override;

        void jump();

        void setHorizontalMovement(float value) {
            horizontalMovement = value;
        }

    private:
        float velocityY = 0.0f;
        float horizontalMovement = 0.0f;
    };
}