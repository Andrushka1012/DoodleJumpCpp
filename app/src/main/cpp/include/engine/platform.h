#pragma once

#include "renderer.h"
#include "renderable.h"
#include "camera.h"
#include "constants.h"

namespace DoodleJumpGame {
    class Platform final : public Renderable {  // final = "sealed class"
    public:
        explicit Platform(float startX = 0, float startY = 0)
                : Renderable(startX, startY, GameConstants::PLATFORM_WIDTH, GameConstants::PLATFORM_HEIGHT) {}

        [[nodiscard]] RenderObject getRenderObject() const override;

        void update(float deltaTime) override;

    private:

    };
}