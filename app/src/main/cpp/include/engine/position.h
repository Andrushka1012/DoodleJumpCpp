#pragma once

namespace DoodleJumpGame {
    class Position {
    public:
        Position(float startX = 0.0f, float startY = 0.0f) : x(startX), y(startY) {}

        Position operator+(const Position& other) const {
            return Position(x + other.x, y + other.y);
        }
        
        Position operator-(const Position& other) const {
            return Position(x - other.x, y - other.y);
        }
        
        Position& operator+=(const Position& other) {
            x += other.x;
            y += other.y;
            return *this;
        }
        
        float x = 0.0f;
        float y = 0.0f;
    };
}