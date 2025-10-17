//#include <memory>
#include <algorithm>
#include <vector>

#include "Entity.hpp"
#include "Utils.hpp"

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

bool Entity::update(std::vector<std::vector<Entity>>& grid, int x, int y, std::vector<SwapOp>& outSwaps) {
	const Entity& target = grid[x][y];

	switch (target.getId()) {
		case CellId::Null: 
		case CellId::Air: 
		case CellId::Stone: 
			return false;

		case CellId::Sand:
			return sand::update(grid, x, y, outSwaps);
		case CellId::Water:
			return water::update(grid, x, y, outSwaps);
		case CellId::Fire:
			return fire::update(grid, x, y, outSwaps);
		case CellId::Smoke:
			return smoke::update(grid, x, y, outSwaps);
		default:
				return false;
	}
}

bool sand::update(const std::vector<std::vector<Entity>>& grid, int x, int y, std::vector<SwapOp>& outSwaps) {

	bool updated = false;
	SwapOp checkCells[3] = {
			swaps::below(x, y),
			swaps::downLeft(x, y),
			swaps::downRight(x, y)
	};
	std::shuffle(checkCells + 1, checkCells + 3, utils::getRandomEngine());

	for (SwapOp& swap : checkCells) {
		const Entity& cell1 = grid[swap.x1][swap.y1];
		const Entity& cell2 = grid[swap.x2][swap.y2];

		if (cell2.getId() == CellId::Air || cell2.getId() == CellId::Water) {
			outSwaps.emplace_back(std::move(swap));
			updated = true;
			break;
		}
	}
	return updated;
}

bool water::update(const std::vector<std::vector<Entity>>& grid, int x, int y, std::vector<SwapOp>& outSwaps) {
	bool updated = false;
	SwapOp checkCells[5] = {
			swaps::below(x, y),
			swaps::downLeft(x, y),
			swaps::downRight(x, y),
			swaps::left(x, y),
			swaps::right(x, y)
	};
	std::shuffle(checkCells + 1, checkCells + 5, utils::getRandomEngine());

	for (SwapOp& swap : checkCells) {
		const Entity& cell1 = grid[swap.x1][swap.y1];
		const Entity& cell2 = grid[swap.x2][swap.y2];

		if (cell2.getId() == CellId::Air) {
			outSwaps.emplace_back(std::move(swap));
			updated = true;
			break;
		}
	}
	return updated;
}

bool fire::update(std::vector<std::vector<Entity>>& grid, int x, int y, std::vector<SwapOp>& outSwaps) {

	Entity& cell1 = grid[x][y];

	//timer to self destruct
	if ((cell1.ra >= 100)) {
		cell1.ra = 0;
		outSwaps.emplace_back(x, y, x, y, CellId::Smoke);
		return true;
	} else {
		cell1.ra++;
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
		const Entity& cell2 = grid[swap.x2][swap.y2];

		if (cell2.getId() == CellId::Air) {
			outSwaps.emplace_back(std::move(swap));
			break;
		} 
		if (cell2.getId() == CellId::Water) {
			outSwaps.emplace_back(x, y, x, y, CellId::Air);
			break;
		} 
	}
	// always return true to keep partition (and counter) active
	return true;
}

bool smoke::update(std::vector<std::vector<Entity>>& grid, int x, int y, std::vector<SwapOp>& outSwaps) {
	Entity& cell1 = grid[x][y];

	//timer to self destruct
	if ((cell1.ra >= 75)) {
		cell1.ra = 0;
		outSwaps.emplace_back(x, y, x, y, CellId::Air);
	} else {
		cell1.ra++;
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
		const Entity& cell2 = grid[swap.x2][swap.y2];

		if (cell2.getId() == CellId::Air) {
			outSwaps.emplace_back(std::move(swap));
			break;
		} 
	}
	// always return true to keep partition (and counter) active
	return true;
}


