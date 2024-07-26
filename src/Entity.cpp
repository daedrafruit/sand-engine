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

	SwapOperation checkCells[3] = {
			swaps::below(x, y),
			swaps::left(x, y),
			swaps::right(x, y)
	};

	std::shuffle(checkCells + 1, checkCells + 3, utils::getRandomEngine());

	for (const SwapOperation& swap : checkCells) {
		const std::unique_ptr<Entity>& cell1 = grid[swap.x1][swap.y1];
		const std::unique_ptr<Entity>& cell2 = grid[swap.x2][swap.y2];

		if (cell2->getId() == CellId::Air || cell2->getId() == CellId::Water) {

			return std::make_unique<SwapOperation>(swap.x1, swap.y1, swap.x2, swap.y2);
		}
	}
	return nullptr;
}

// *********************************************************************
// Water
// *********************************************************************

std::unique_ptr<SwapOperation> Water::update(const std::vector<std::vector<std::unique_ptr<Entity>>>& grid, int x, int y) {

	SwapOperation checkCells[5] = {
			swaps::below(x, y),
			swaps::downLeft(x, y),
			swaps::downRight(x, y),
			swaps::left(x, y),
			swaps::right(x, y)
	};

	std::shuffle(checkCells + 1, checkCells + 5, utils::getRandomEngine());
	for (const SwapOperation& swap : checkCells) {
		const std::unique_ptr<Entity>& cell1 = grid[swap.x1][swap.y1];
		const std::unique_ptr<Entity>& cell2 = grid[swap.x2][swap.y2];

		if (cell2->getId() == CellId::Air) {

			return std::make_unique<SwapOperation>(swap.x1, swap.y1, swap.x2, swap.y2);
		}
	}
	return nullptr;
}

// *********************************************************************
// Fire
// *********************************************************************

Color Fire::getColor() const {
	if (this->ra <= 25) {
		return {255, 75, 0}; 
	}
	else if (this->ra < 50) {
		return {255, 165, 0}; 
	}
	else {
		return {255, 255, 0}; 
	}
}

std::unique_ptr<SwapOperation> Fire::update(const std::vector<std::vector<std::unique_ptr<Entity>>>& grid, int x, int y) {

	const std::unique_ptr<Entity>& cell1 = grid[x][y];

	//timer to self destruct
	if ((cell1->getRegister('a') >= 100)) {
		cell1->setRegister('a', 0);
		return std::make_unique<SwapOperation>(x, y, x, y, std::make_unique<Smoke>(0));
	} else {
		cell1->setRegister('a', cell1->getRegister('a') + 1);
	}

	SwapOperation checkCells[5] = {
			swaps::above(x, y),
			swaps::upLeft(x, y),
			swaps::upRight(x, y),
			swaps::left(x, y),
			swaps::right(x, y)
	};

	std::shuffle(checkCells, checkCells + 5, utils::getRandomEngine());
	for (const SwapOperation& swap : checkCells) {
		const std::unique_ptr<Entity>& cell2 = grid[swap.x2][swap.y2];

		if (cell2->getId() == CellId::Air) {
			return std::make_unique<SwapOperation>(swap.x1, swap.y1, swap.x2, swap.y2);
		} 
	}
	//return self swap so that partition stays enabled and timer can continue
	return std::make_unique<SwapOperation>(x, y, x, y);
}

// *********************************************************************
// Smoke
// *********************************************************************

std::unique_ptr<SwapOperation> Smoke::update(const std::vector<std::vector<std::unique_ptr<Entity>>>& grid, int x, int y) {

	const std::unique_ptr<Entity>& cell1 = grid[x][y];

	//timer to self destruct
	if ((cell1->getRegister('a') >= 75)) {
		cell1->setRegister('a', 0);
		return std::make_unique<SwapOperation>(x, y, x, y, std::make_unique<Air>(0));
	} else {
		cell1->setRegister('a', cell1->getRegister('a') + 1);
	}

	SwapOperation checkCells[5] = {
			swaps::above(x, y),
			swaps::upLeft(x, y),
			swaps::upRight(x, y),
			swaps::left(x, y),
			swaps::right(x, y)
	};

	std::shuffle(checkCells, checkCells + 5, utils::getRandomEngine());
	for (const SwapOperation& swap : checkCells) {
		const std::unique_ptr<Entity>& cell2 = grid[swap.x2][swap.y2];

		if (cell2->getId() == CellId::Air) {
			return std::make_unique<SwapOperation>(swap.x1, swap.y1, swap.x2, swap.y2);
		} 
	}
	//return self swap so that partition stays enabled and timer can continue
	return std::make_unique<SwapOperation>(x, y, x, y);
}
