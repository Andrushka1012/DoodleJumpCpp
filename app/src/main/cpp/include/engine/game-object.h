#pragma once

#include "renderer.h"
#include "position.h"
#include "screen-controller.h"
#include "render-object.h"

namespace DoodleJumpGame {
    class GameObject {
    public:
        virtual ~GameObject() = default;

        virtual RenderObject getRenderObject() const = 0;

        virtual void update(float deltaTime) = 0;

        float width = 32.0f;
        float height = 32.0f;
        bool isVisible = true;

        [[nodiscard]] float getX() const {
            return position.x;
        }

        [[nodiscard]] float getY() const {
            return position.y;
        }

        [[nodiscard]] Position getPosition() const {
            return position;
        }

        [[nodiscard]] bool isColliding(const GameObject &other) const;

    protected:
        GameObject(float x, float y, float width, float height)
                : position(x, y), width(width), height(height) {}

        void setXWithScreenWrap(float newX);

        void setXWithBoundaryClamp(float newX);

        void setY(float newY) {
            position.y = newY;
        };


    private:
        Position position;
    };

}
