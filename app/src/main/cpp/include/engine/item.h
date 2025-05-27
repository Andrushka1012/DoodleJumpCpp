#pragma once

#include "renderer.h"
#include "game-object.h"
#include "screen-controller.h"
#include "constants.h"
#include "player.h"

namespace DoodleJumpGame {
    enum class ItemType {
        Rocket,
        BlackHole,
    };

    inline float getItemSize(ItemType type) {
        switch (type) {
            case ItemType::Rocket:
                return GameConstants::ROCKET;
            case ItemType::BlackHole:
                return GameConstants::BACK_HOLE;
        }
    }

    class Item final : public GameObject {
    public:
        explicit Item(float startX = 0, float startY = 0, ItemType type = ItemType::Rocket)
                : GameObject(startX, startY, getItemSize(type), getItemSize(type)),
                  type(type) {}

        RenderObject getRenderObject() const override;

        void update(float deltaTime) {};

        void interact(DoodleJumpGame::Player& player);

        RenderColor getColor() const;

    private:
        ItemType type;
    };
}