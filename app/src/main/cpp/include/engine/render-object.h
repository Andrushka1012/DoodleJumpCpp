#pragma once

#include "position.h"

namespace DoodleJumpGame {
    struct RenderColor {
        float r = 1.0f;  // Red component (0.0 to 1.0)
        float g = 1.0f;  // Green component (0.0 to 1.0)
        float b = 1.0f;  // Blue component (0.0 to 1.0)
        float a = 1.0f;  // Alpha component (opacity, 0.0 to 1.0)

        RenderColor() = default;

        RenderColor(float red, float green, float blue, float alpha = 1.0f)
                : r(red), g(green), b(blue), a(alpha) {}
    };

    struct RenderObject {
        float x = 0.0f;  // X position
        float y = 0.0f;  // Y position
        float width = 1.0f;  // Width of the object
        float height = 1.0f; // Height of the object
        RenderColor color; // Color of the object

        RenderObject() = default;

        RenderObject(float x, float y, float w, float h, RenderColor c = RenderColor())
                : x(x), y(y), width(w), height(h), color(c) {}

        [[nodiscard]] Position getPosition() const {
            return {x, y};
        }
    };
}
