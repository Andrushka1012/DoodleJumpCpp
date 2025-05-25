#pragma once

#include "position.h"

namespace DoodleJumpGame {
    class Camera {
    public:
        Camera() = default;

        Camera(float startY)
                : cameraPosition(0, startY) {}

        void setAspectRatio(float ratio) { aspectRatio = ratio; } // TODO: check how it handles square screens

        [[nodiscard]] bool isAbove(float value) const {
            return cameraPosition.isAbove(value);
        }

        [[nodiscard]] bool isAbove(Position value) const {
            return cameraPosition.isAbove(value);
        }

        [[nodiscard]] float getY() const {
            return cameraPosition.y;
        }

        Position transformToOnScreenPosition(Position position);

        float transformToOnScreenWidth(float size) {
            return size / (200.0f * aspectRatio);
        }

        float transformToOnScreenHeight(float size) {
            return size / 200.0f;
        }

    private:
        Position cameraPosition;
        float aspectRatio = 1.0f;
    };
}