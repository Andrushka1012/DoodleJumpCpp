//
// Created by Andrii Makarenko on 27/05/2025.
//

#include "engine/item.h"

DoodleJumpGame::RenderObject DoodleJumpGame::Item::getRenderObject() const {
    return {getX(), getY(), width, height, getColor()};
}

void DoodleJumpGame::Item::interact(DoodleJumpGame::Player& player) {
    switch (type) {
        case ItemType::Rocket:
            isVisible = false;
            player.takeRocket();
            break;
        case ItemType::BlackHole:
            player.death();
            break;
    }
}

DoodleJumpGame::RenderColor DoodleJumpGame::Item::getColor() const {
    switch (type) {
        case ItemType::Rocket:
            return {1.0f, 1.0f, 0.0f, 1.0f};
        case ItemType::BlackHole:
            return {1.0f, 0.0f, 0.0f, 1.0f};
    }
}
