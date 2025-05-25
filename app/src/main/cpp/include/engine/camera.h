#pragma once

#include "position.h"


namespace DoodleJumpGame {
    class Camera {
    public:
        Camera() = default;
        Camera(float startX, float startY)
                : position(startX, startY) {}

        [[nodiscard]] bool canSee(Position pos) const {
            return pos.x >= position.x - 1.0f && pos.x <= position.x + 1.0f &&
                   pos.y >= position.y - 1.0f && pos.y <= position.y + 1.0f;
        }


        [[nodiscard]] bool isAbove(float value) const {
            return position.y > value;
        }


        [[nodiscard]] Position getPosition() const {
            return position;
        }


    private:
        Position position;
    };


}