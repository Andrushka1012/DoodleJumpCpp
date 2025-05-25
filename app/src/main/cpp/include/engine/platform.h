#pragma once

#include "renderer.h"
#include "game-object.h"
#include "screen-controller.h"
#include "constants.h"

namespace DoodleJumpGame {

    enum class PlatformType {
        Normal,
        Moving,
        Crumbling,
        Falling,
    };

    class Platform final : public GameObject {  // final = "sealed class"
    public:
        explicit Platform(float startX = 0, float startY = 0, PlatformType type = PlatformType::Normal, float velocity = 0.0f)
                : GameObject(startX, startY, GameConstants::PLATFORM_WIDTH, GameConstants::PLATFORM_HEIGHT),
                  type(type),
                  velocity(velocity) {}

        [[nodiscard]] RenderObject getRenderObject() const override;

        void update(float deltaTime) override;

        void touch();

        [[nodiscard]] RenderColor getColor() const;

        [[nodiscard]] PlatformType getType() const { return type; }

        void normalizeY(float offset);

    private:
        PlatformType type;
        float velocity = 0.0f;
    };

}
