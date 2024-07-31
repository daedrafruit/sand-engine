#include <memory>
#include <algorithm>
#include <vector>

#include "Entity.hpp"
#include "Utils.hpp"

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

std::vector<SwapOperation> Entity::update(const std::vector<std::vector<std::unique_ptr<Entity>>>& grid, int x, int y) {
	std::vector<SwapOperation> outSwaps;
	return outSwaps;
}

// *********************************************************************
// Sand
// *********************************************************************

std::vector<SwapOperation> Sand::update(const std::vector<std::vector<std::unique_ptr<Entity>>>& grid, int x, int y) {

	SwapOperation checkCells[3] = {
			swaps::below(x, y),
			swaps::downLeft(x, y),
			swaps::downRight(x, y)
	};
	std::shuffle(checkCells + 1, checkCells + 3, utils::getRandomEngine());

	std::vector<SwapOperation> outSwaps;

	for (SwapOperation& swap : checkCells) {
		const std::unique_ptr<Entity>& cell1 = grid[swap.x1][swap.y1];
		const std::unique_ptr<Entity>& cell2 = grid[swap.x2][swap.y2];

		if (cell2->getId() == CellId::Air || cell2->getId() == CellId::Water) {
			outSwaps.emplace_back(std::move(swap));
			break;
		}
	}
	return outSwaps;
}

// *********************************************************************
// Water
// *********************************************************************

std::vector<SwapOperation> Water::update(const std::vector<std::vector<std::unique_ptr<Entity>>>& grid, int x, int y) {

	SwapOperation checkCells[5] = {
			swaps::below(x, y),
			swaps::downLeft(x, y),
			swaps::downRight(x, y),
			swaps::left(x, y),
			swaps::right(x, y)
	};
	std::shuffle(checkCells + 1, checkCells + 5, utils::getRandomEngine());

	std::vector<SwapOperation> outSwaps;
	for (SwapOperation& swap : checkCells) {
		const std::unique_ptr<Entity>& cell1 = grid[swap.x1][swap.y1];
		const std::unique_ptr<Entity>& cell2 = grid[swap.x2][swap.y2];

		if (cell2->getId() == CellId::Air) {
			outSwaps.emplace_back(std::move(swap));
			break;
		}
	}
	return outSwaps;
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
		return this->color; 
	}
}

std::vector<SwapOperation> Fire::update(const std::vector<std::vector<std::unique_ptr<Entity>>>& grid, int x, int y) {

	const std::unique_ptr<Entity>& cell1 = grid[x][y];

	std::vector<SwapOperation> outSwaps;
	//timer to self destruct
	if ((cell1->getRegister('a') >= 100)) {
		cell1->setRegister('a', 0);
		std::unique_ptr<Entity> smoke = std::make_unique<Smoke>(0);
		outSwaps.emplace_back(x, y, x, y, std::move(smoke));
		return outSwaps;
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
	for (SwapOperation& swap : checkCells) {
		const std::unique_ptr<Entity>& cell2 = grid[swap.x2][swap.y2];

		if (cell2->getId() == CellId::Air) {
			outSwaps.emplace_back(std::move(swap));
			break;
		} 
	}
	//return self swap so that partition stays enabled and timer can continue
	return outSwaps;
}

// *********************************************************************
// Smoke
// *********************************************************************

std::vector<SwapOperation> Smoke::update(const std::vector<std::vector<std::unique_ptr<Entity>>>& grid, int x, int y) {

	const std::unique_ptr<Entity>& cell1 = grid[x][y];

	std::vector<SwapOperation> outSwaps;
	//timer to self destruct
	if ((cell1->getRegister('a') >= 75)) {
		cell1->setRegister('a', 0);
		std::unique_ptr<Entity> air = std::make_unique<Air>(0);
		outSwaps.emplace_back(x, y, x, y, std::move(air));
		return outSwaps;
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
	for (SwapOperation& swap : checkCells) {
		const std::unique_ptr<Entity>& cell2 = grid[swap.x2][swap.y2];

		if (cell2->getId() == CellId::Air) {
			outSwaps.emplace_back(std::move(swap));
			break;
		} 
	}
	//return self swap so that partition stays enabled and timer can continue
	return outSwaps;
}
