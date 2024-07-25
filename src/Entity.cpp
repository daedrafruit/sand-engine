#include <memory>
#include <algorithm>

#include "Entity.hpp"
#include "Utils.hpp"

Entity::Entity(int p_lastUpdated) 
    :lastUpdated(p_lastUpdated) {
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

std::unique_ptr<SwapOperation> Entity::update(const std::vector<std::vector<std::unique_ptr<Entity>>>& grid, int x, int y) {
		return nullptr;
}

// *********************************************************************
// Sand
// *********************************************************************

std::unique_ptr<SwapOperation> Sand::update(const std::vector<std::vector<std::unique_ptr<Entity>>>& grid, int x, int y) {

	const SwapOperation below = {x, y, x, y+1};
	const SwapOperation downLeft = {x, y, x-1, y+1};
	const SwapOperation downRight = {x, y, x+1, y+1};

	SwapOperation checkCells[3] = {below, downLeft, downRight};
	std::shuffle(checkCells + 1, checkCells + 3, utils::getRandomEngine());

	for (const SwapOperation& swap : checkCells) {
		const std::unique_ptr<Entity>& cell1 = grid[swap.x1][swap.y1];
		const std::unique_ptr<Entity>& cell2 = grid[swap.x2][swap.y2];

		if (cell2->getId() == CellId::Air || cell2->getId() == CellId::Water) {

			return std::make_unique<SwapOperation>(swap);
		}
	}
	return nullptr;
}

// *********************************************************************
// Water
// *********************************************************************

std::unique_ptr<SwapOperation> Water::update(const std::vector<std::vector<std::unique_ptr<Entity>>>& grid, int x, int y) {

	const SwapOperation below = {x, y, x, y+1};
	const SwapOperation downLeft = {x, y, x-1, y+1};
	const SwapOperation downRight = {x, y, x+1, y+1};
	const SwapOperation left = {x, y, x-1, y};
	const SwapOperation right = {x, y, x+1, y};

	SwapOperation checkCells[5] = {below, downLeft, downRight, left, right};
	std::shuffle(checkCells + 1, checkCells + 5, utils::getRandomEngine());
	for (const SwapOperation& swap : checkCells) {
		const std::unique_ptr<Entity>& cell1 = grid[swap.x1][swap.y1];
		const std::unique_ptr<Entity>& cell2 = grid[swap.x2][swap.y2];

		if (cell2->getId() == CellId::Air) {

			return std::make_unique<SwapOperation>(swap);
		}
	}
	return nullptr;
}

// *********************************************************************
// Fire
// *********************************************************************

std::unique_ptr<SwapOperation> Fire::update(const std::vector<std::vector<std::unique_ptr<Entity>>>& grid, int x, int y) {

	const SwapOperation above = {x, y, x, y-1};
	const SwapOperation upLeft = {x, y, x-1, y-1};
	const SwapOperation upRight = {x, y, x+1, y-1};
	const SwapOperation left = {x, y, x-1, y};
	const SwapOperation right = {x, y, x+1, y};

	const SwapOperation remove = {x, y, -1, -1};
	const SwapOperation self = {x, y, x, y};

	SwapOperation checkCells[5] = {above, upLeft, upRight, left, right};
	std::shuffle(checkCells, checkCells + 5, utils::getRandomEngine());
	for (const SwapOperation& swap : checkCells) {
		const std::unique_ptr<Entity>& cell1 = grid[swap.x1][swap.y1];
		const std::unique_ptr<Entity>& cell2 = grid[swap.x2][swap.y2];

		//timer to self destruct
		if ((cell1->getRegister('a') >= 100)) {
			cell1->setRegister('a', 0);
			return std::make_unique<SwapOperation>(swaps::smoke(x, y));
		} else {
			cell1->setRegister('a', cell1->getRegister('a') + 1);
		}

		if (cell2->getId() == CellId::Air) {
			return std::make_unique<SwapOperation>(swap);
		} 
	}
	//return self swap so that partition stays enabled and timer can continue
	return std::make_unique<SwapOperation>(self);
}

// *********************************************************************
// Smoke
// *********************************************************************

std::unique_ptr<SwapOperation> Smoke::update(const std::vector<std::vector<std::unique_ptr<Entity>>>& grid, int x, int y) {

	SwapOperation checkCells[5] = {
			swaps::above(x, y),
			swaps::upLeft(x, y),
			swaps::upRight(x, y),
			swaps::left(x, y),
			swaps::right(x, y)
	};

	const SwapOperation remove = {x, y, -1, -1};
	const SwapOperation self = {x, y, x, y};

	std::shuffle(checkCells, checkCells + 5, utils::getRandomEngine());
	for (const SwapOperation& swap : checkCells) {
		const std::unique_ptr<Entity>& cell1 = grid[swap.x1][swap.y1];
		const std::unique_ptr<Entity>& cell2 = grid[swap.x2][swap.y2];

		//timer to self destruct
		if ((cell1->getRegister('a') >= 200)) {
			cell1->setRegister('a', 0);
			return std::make_unique<SwapOperation>(swaps::air(x, y));
		} else {
			cell1->setRegister('a', cell1->getRegister('a') + 1);
		}

		if (cell2->getId() == CellId::Air) {
			return std::make_unique<SwapOperation>(swap);
		} 
	}
	//return self swap so that partition stays enabled and timer can continue
	return std::make_unique<SwapOperation>(self);
}
