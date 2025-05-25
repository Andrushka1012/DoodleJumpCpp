#include "engine/platform.h"

namespace DoodleJumpGame {

    RenderObject Platform::getRenderObject() const {
        return {getX(), getY(), width, height, getColor()};
    }

    void Platform::update(float deltaTime) {
        switch (type) {
            case PlatformType::Moving:
                if (getX() < -0.8 || getX() > 0.8) {
                    velocity *= -1;
                }
                setXWithBoundaryClamp(getX() + GameConstants::PLATFORM_MOVING_SPEED * velocity * deltaTime);
                break;
            case PlatformType::Falling:
                setY(getY() + velocity * GameConstants::PLATFORM_FALLING_SPEED * deltaTime);
                break;
            case PlatformType::Normal:
            case PlatformType::Crumbling:
                break;
        }
    }


    void Platform::touch() {
        if (type == PlatformType::Crumbling) {
            isVisible = false;
        }
        if (type == PlatformType::Falling) {
            velocity = -1.0f;  // Start falling
        }
    }

    RenderColor Platform::getColor() const {
        switch (type) {
            case PlatformType::Moving:
                return {0.0f, 0.0f, 1.0f, 1.0f};
            case PlatformType::Normal:
                return {1.0f, 1.0f, 1.0f, 1.0f};
            case PlatformType::Crumbling:
                return {1.0f, 0.0f, 0.0f, 1.0f};
            case PlatformType::Falling:
                return {1.0f, 0.0f, 1.0f, 1.0f};
                break;
        }
    }

}