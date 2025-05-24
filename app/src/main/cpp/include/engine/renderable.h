#pragma once

#include "renderer.h"

class Renderable {
public:
    virtual ~Renderable() = default;

    virtual void renderOn(DoodleJumpGame::Renderer *renderer) = 0;

    virtual void update(float deltaTime) = 0;

    float width = 32.0f;
    float height = 32.0f;
    bool visible = true;

    float getX() const {
        return x;
    }

    float getY() const {
        return y;
    }

protected:
    Renderable(float x, float y, float width, float height)
            : x(x), y(y), width(width), height(height) {}

    void setXWithScreenWrap(float newX) {
        if (newX < -1.0f) {
            x = 1.0f;
        } else if (newX > 1.0f) {
            x = -1.0f;
        } else {
            x = newX;
        }
    }

    void setXWithBoundaryClamp(float newX) {
        if (newX < -1.0f) {
            x = -1.0f;
        } else if (newX > 1.0f) {
            x = 1.0f;
        } else {
            x = newX;
        }
    }

    void setY(float newY) {
        y = newY;
    };


private:
    float x = 0.0f;
    float y = 0.0f;
};
