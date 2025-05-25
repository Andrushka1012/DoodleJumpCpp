#pragma once

#include "position.h"


namespace DoodleJumpGame {
    class Camera {
    public:
        Camera() = default;

        Camera(float startY)
                : position(0, startY) {}


        [[nodiscard]] bool isAbove(float value) const {
            return position.isAbove(value);
        }

        [[nodiscard]] bool isAbove(Position value) const {
            return position.isAbove(value);
        }

        [[nodiscard]] float getY() const {
            return position.y;
        }


    private:
        Position position;
    };


}