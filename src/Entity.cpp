#include <memory>
#include <algorithm>

#include "Entity.hpp"
#include "Utils.hpp"


Entity::Entity(CellId p_id) 
    : id(p_id), lastUpdated(0) {
}

Entity::Entity() 
    : id(CellId::Air), lastUpdated(0) {
}

void Entity::setId(CellId p_id, int p_currWorldUpdate) {
    id = p_id;
    lastUpdated = p_currWorldUpdate;
}

void Entity::setLastUpdated(int p_lastUpdated) {
    lastUpdated = p_lastUpdated;
}

void Entity::setRegister(char reg, int value) { 
    switch(reg) {
        case 'a':
            ra = value;
    }
}

int Entity::getRegister(char reg) const { 
    switch(reg) {
        case 'a':
            return ra;
    }
    return 0;
}

int Entity::getLastUpdated() const { return lastUpdated; }

CellId Entity::getId() const { return id; }

bool Entity::isEmpty() const {
    return (id == CellId::Air);
}

Color Entity::getColor() const {
    switch (id) {
        case CellId::Stone:
            // grey
            return {200, 200, 200};
        case CellId::Sand:
            // yellow
            return {245, 200, 70};
        case CellId::Water:
            // blue
            return {0, 0, 255};
        case CellId::Fire:
            // red
            return {255, 0, 0};
        case CellId::Smoke:
            // grey
            return {75, 75, 75};
        default:
            return {0, 0, 0};
    }
}

std::unique_ptr<SwapOperation> Entity::update(const Entity *const *const grid, int x, int y) {
		return nullptr;
}

// *********************************************************************
// Sand
// *********************************************************************

std::unique_ptr<SwapOperation> Sand::update(const Entity *const *const grid, int x, int y) {

	const SwapOperation below = {x, y, x, y+1};
	const SwapOperation downLeft = {x, y, x-1, y+1};
	const SwapOperation downRight = {x, y, x+1, y+1};

	SwapOperation checkCells[3] = {below, downLeft, downRight};
	std::shuffle(checkCells + 1, checkCells + 3, utils::getRandomEngine());

	for (const SwapOperation& swap : checkCells) {
		const Entity& cell1 = grid[swap.x1][swap.y1];
		const Entity& cell2 = grid[swap.x2][swap.y2];

		if (cell2.getId() == CellId::Air || cell2.getId() == CellId::Water) {

			return std::make_unique<SwapOperation>(swap);
		}
	}
	return nullptr;
}

