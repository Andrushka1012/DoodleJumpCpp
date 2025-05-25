#pragma once

#include "position.h"
#include "constants.h"
#include "render-object.h"

namespace DoodleJumpGame {
    class Camera {
    public:
        explicit Camera(float startY = 0.0f, float worldHeight = 100.0f)
                : cameraPosition(0, startY), worldViewHeight(worldHeight) {}

        void setAspectRatio(const float ratio) { aspectRatio = ratio; }

        [[nodiscard]] bool isCameraAbove(const float value) const {
            return cameraPosition.isAbove(value);
        }

        [[nodiscard]] bool isCameraAbove(const Position& value) const {
            return cameraPosition.isAbove(value);
        }

        void adjustCameraPosition(const Position& position);

        [[nodiscard]] float getY() const {
            return cameraPosition.y;
        }

        [[nodiscard]] RenderObject adjustToScreen(const RenderObject &renderObject) const {
            Position onScreenPosition = transformToOnScreenPosition(renderObject.getPosition());
            return {
                    onScreenPosition.x,
                    onScreenPosition.y,
                    transformToOnScreenWidth(renderObject.width),
                    transformToOnScreenHeight(renderObject.height),
                    renderObject.color
            };
        }

    private:
        Position cameraPosition;
        float maxCameraHeight = 0.0f;
        float aspectRatio = 1.0f;
        float worldViewHeight = 100.0f;


        [[nodiscard]] Position transformToOnScreenPosition(const Position &worldPos) const {
            float relativeY = worldPos.y - cameraPosition.y;

            float screenY = (relativeY / (worldViewHeight * 0.5f)) + GameConstants::CAMERA_OFFSET_Y;
            float screenX = worldPos.x;

            return {screenX, screenY};
        }

        [[nodiscard]] float transformToOnScreenWidth(float worldSize) const {
            return worldSize / (worldViewHeight * 0.5f);
        }

        [[nodiscard]] float transformToOnScreenHeight(float height) const {
            return height / (worldViewHeight * 0.5f) * aspectRatio;
        }
    };
}