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

void Entity::setId(CellId p_id, int worldUpdate) {
	id = p_id;
	lastUpdated = worldUpdate;

}

Color Entity::getColor() const {
	switch (id) {
		case CellId::Air: return {0,0,0};
		case CellId::Stone: return {200,200,200};
		case CellId::Sand: return {245,200,70};
		case CellId::Water: return {0,0,255};

		case CellId::Fire:
			if (this->ra <= 25)
				return {255, 75, 0}; 
			else if (this->ra < 50)
				return {255, 165, 0}; 
			else
				return {255,255,0}; 

		case CellId::Smoke: return {70,70,70};
		default:
			return {0,255,0}; 
	}
}

std::vector<SwapOp> Entity::update(const std::vector<std::vector<std::unique_ptr<Entity>>>& grid, int x, int y) {
	std::vector<SwapOp> outSwaps;
	return outSwaps;
}

/*
std::vector<SwapOp> Sand::update(const std::vector<std::vector<std::unique_ptr<Entity>>>& grid, int x, int y) {

	SwapOp checkCells[3] = {
			swaps::below(x, y),
			swaps::downLeft(x, y),
			swaps::downRight(x, y)
	};
	std::shuffle(checkCells + 1, checkCells + 3, utils::getRandomEngine());

	std::vector<SwapOp> outSwaps;

	for (SwapOp& swap : checkCells) {
		const std::unique_ptr<Entity>& cell1 = grid[swap.x1][swap.y1];
		const std::unique_ptr<Entity>& cell2 = grid[swap.x2][swap.y2];

		if (cell2->getId() == CellId::Air || cell2->getId() == CellId::Water) {
			outSwaps.emplace_back(std::move(swap));
			break;
		}
	}
	return outSwaps;
}

std::vector<SwapOp> Water::update(const std::vector<std::vector<std::unique_ptr<Entity>>>& grid, int x, int y) {

	SwapOp checkCells[5] = {
			swaps::below(x, y),
			swaps::downLeft(x, y),
			swaps::downRight(x, y),
			swaps::left(x, y),
			swaps::right(x, y)
	};
	std::shuffle(checkCells + 1, checkCells + 5, utils::getRandomEngine());

	std::vector<SwapOp> outSwaps;
	for (SwapOp& swap : checkCells) {
		const std::unique_ptr<Entity>& cell1 = grid[swap.x1][swap.y1];
		const std::unique_ptr<Entity>& cell2 = grid[swap.x2][swap.y2];

		if (cell2->getId() == CellId::Air) {
			outSwaps.emplace_back(std::move(swap));
			break;
		}
	}
	return outSwaps;
}

std::vector<SwapOp> Fire::update(const std::vector<std::vector<std::unique_ptr<Entity>>>& grid, int x, int y) {

	const std::unique_ptr<Entity>& cell1 = grid[x][y];

	std::vector<SwapOp> outSwaps;
	//timer to self destruct
	if ((cell1->getRegister('a') >= 100)) {
		cell1->setRegister('a', 0);
		std::unique_ptr<Entity> smoke = std::make_unique<Smoke>(0);
		outSwaps.emplace_back(x, y, x, y, std::move(smoke));
		return outSwaps;
	} else {
		cell1->setRegister('a', cell1->getRegister('a') + 1);
	}

	SwapOp checkCells[5] = {
			swaps::above(x, y),
			swaps::upLeft(x, y),
			swaps::upRight(x, y),
			swaps::left(x, y),
			swaps::right(x, y)
	};

	std::shuffle(checkCells, checkCells + 5, utils::getRandomEngine());
	for (SwapOp& swap : checkCells) {
		const std::unique_ptr<Entity>& cell2 = grid[swap.x2][swap.y2];

		if (cell2->getId() == CellId::Air) {
			outSwaps.emplace_back(std::move(swap));
			break;
		} 
		if (cell2->getId() == CellId::Water) {
			std::unique_ptr<Entity> air = std::make_unique<Air>(0);
			outSwaps.emplace_back(x, y, x, y, std::move(air));
			break;
		} 
	}
	//return self swap so that partition stays enabled and timer can continue
	return outSwaps;
}

std::vector<SwapOp> Smoke::update(const std::vector<std::vector<std::unique_ptr<Entity>>>& grid, int x, int y) {

	const std::unique_ptr<Entity>& cell1 = grid[x][y];

	std::vector<SwapOp> outSwaps;
	//timer to self destruct
	if ((cell1->getRegister('a') >= 75)) {
		cell1->setRegister('a', 0);
		std::unique_ptr<Entity> air = std::make_unique<Air>(0);
		outSwaps.emplace_back(x, y, x, y, std::move(air));
		return outSwaps;
	} else {
		cell1->setRegister('a', cell1->getRegister('a') + 1);
	}

	SwapOp checkCells[5] = {
			swaps::above(x, y),
			swaps::upLeft(x, y),
			swaps::upRight(x, y),
			swaps::left(x, y),
			swaps::right(x, y)
	};

	std::shuffle(checkCells, checkCells + 5, utils::getRandomEngine());
	for (SwapOp& swap : checkCells) {
		const std::unique_ptr<Entity>& cell2 = grid[swap.x2][swap.y2];

		if (cell2->getId() == CellId::Air) {
			outSwaps.emplace_back(std::move(swap));
			break;
		} 
	}
	//return self swap so that partition stays enabled and timer can continue
	return outSwaps;
}
*/
