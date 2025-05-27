#pragma once

#include "renderer.h"
#include "game-object.h"
#include "screen-controller.h"
#include "constants.h"

namespace DoodleJumpGame {
    class Player final : public GameObject {  // final = "sealed class"
    public:
        Player(float startX = 0, float startY = 0)
                : GameObject(startX, startY, GameConstants::PLAYER_WIDTH, GameConstants::PLAYER_HEIGHT) {}

        RenderObject getRenderObject() const override;

        void update(float deltaTime) override;

        void jump();

        void setHorizontalMovement(float value) {
            if (isAlive) {
                horizontalMovement = value;
            }
        }

        bool isFalling() const {
            return velocityY <= 0.0f;
        }

        void death();

        void takeRocket();

    private:
        float velocityY = 0.0f;
        float horizontalMovement = 0.0f;
        bool isAlive = true;
        float rocketTime = 0.0f;
    };
}