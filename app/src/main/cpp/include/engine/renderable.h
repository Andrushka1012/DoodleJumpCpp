#pragma once

#include "renderer.h"

class Renderable {
public:
    virtual ~Renderable() = default;

    virtual void renderOn(Engine::Renderer* renderer) = 0;
    virtual void update(float deltaTime) = 0;

    float x = 0.0f;
    float y = 0.0f;
    float width = 32.0f;
    float height = 32.0f;
    bool visible = true;

protected:
    Renderable(float x, float y, float width, float height)
            : x(x), y(y), width(width), height(height) {}
};