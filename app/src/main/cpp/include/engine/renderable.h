#pragma once

#include "renderer.h"
#include "position.h"

namespace DoodleJumpGame {
    class Renderable {
    public:
        virtual ~Renderable() = default;

        virtual void renderOn(DoodleJumpGame::Renderer *renderer, float cameraOffsetY) = 0;

        virtual void update(float deltaTime) = 0;

        float width = 32.0f;
        float height = 32.0f;
        bool visible = true;

        [[nodiscard]] float getX() const {
            return position.x;
        }

        [[nodiscard]] float getY() const {
            return position.y;
        }

        [[nodiscard]] Position getPosition() const {
            return position;
        }

    protected:
        Renderable(float x, float y, float width, float height)
                : position(x, y), width(width), height(height) {}

        void setXWithScreenWrap(float newX) {

            if (newX < -1.0f) {
                position.x = 1.0f;
            } else if (newX > 1.0f) {
                position.x = -1.0f;
            } else {
                position.x = newX;
            }
        }

        void setXWithBoundaryClamp(float newX) {
            if (newX < -1.0f) {
                position.x = -1.0f;
            } else if (newX > 1.0f) {
                position.x = 1.0f;
            } else {
                position.x = newX;
            }
        }

        void setY(float newY) {
            position.y = newY;
        };


    private:
        Position position;
    };

}
