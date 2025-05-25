#pragma once

#include "engine/game-object.h"

namespace DoodleJumpGame {

    bool GameObject::isColliding(const GameObject &other) const {
        float halfWidth = width / 2.0f;
        float thirdHeight = height / 3.0f;
        float otherHalfWidth = other.width / 2.0f;
        float otherThirdHeight = other.height / 3.0f;

        // Calculate the edges of both objects
        float thisLeft = getX() - halfWidth;
        float thisRight = getX() + halfWidth;
        float thisBottom = getY() - thirdHeight;
        float thisTop = getY() + thirdHeight;

        float otherLeft = other.getX() - otherHalfWidth;
        float otherRight = other.getX() + otherHalfWidth;
        float otherBottom = other.getY() - otherThirdHeight;
        float otherTop = other.getY() + otherThirdHeight;

        // Check for collision
        return (thisLeft < otherRight &&
                thisRight > otherLeft &&
                thisBottom < otherTop &&
                thisTop > otherBottom);
    }


    void GameObject::setXWithScreenWrap(float newX) {

        if (newX < -1.0f) {
            position.x = 1.0f;
        } else if (newX > 1.0f) {
            position.x = -1.0f;
        } else {
            position.x = newX;
        }
    }

    void GameObject::setXWithBoundaryClamp(float newX) {
        if (newX < -1.0f) {
            position.x = -1.0f;
        } else if (newX > 1.0f) {
            position.x = 1.0f;
        } else {
            position.x = newX;
        }
    }
}