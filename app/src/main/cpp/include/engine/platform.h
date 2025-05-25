#pragma once

#include "renderer.h"
#include "game-object.h"
#include "screen-controller.h"
#include "constants.h"

namespace DoodleJumpGame {
    class Platform final : public GameObject {  // final = "sealed class"
    public:
        explicit Platform(float startX = 0, float startY = 0)
                : GameObject(startX, startY, GameConstants::PLATFORM_WIDTH, GameConstants::PLATFORM_HEIGHT) {}

        [[nodiscard]] RenderObject getRenderObject() const override;

        void update(float deltaTime) override;

    private:

    };
}