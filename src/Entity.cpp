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

namespace Sand {
	std::vector<SwapOp> update(const std::vector<std::vector<Entity>>& grid, int x, int y) {

		SwapOp checkCells[3] = {
				swaps::below(x, y),
				swaps::downLeft(x, y),
				swaps::downRight(x, y)
		};
		std::shuffle(checkCells + 1, checkCells + 3, utils::getRandomEngine());

		std::vector<SwapOp> outSwaps;

		for (SwapOp& swap : checkCells) {
			const Entity& cell1 = grid[swap.x1][swap.y1];
			const Entity& cell2 = grid[swap.x2][swap.y2];

			if (cell2.getId() == CellId::Air || cell2.getId() == CellId::Water) {
				outSwaps.emplace_back(std::move(swap));
				break;
			}
		}
		return outSwaps;
	}
}
namespace Water {
	std::vector<SwapOp> update(const std::vector<std::vector<Entity>>& grid, int x, int y) {

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
			const Entity& cell1 = grid[swap.x1][swap.y1];
			const Entity& cell2 = grid[swap.x2][swap.y2];

			if (cell2.getId() == CellId::Air) {
				outSwaps.emplace_back(std::move(swap));
				break;
			}
		}
		return outSwaps;
	}
}

namespace Fire {
	std::vector<SwapOp> update(std::vector<std::vector<Entity>>& grid, int x, int y) {

		Entity& cell1 = grid[x][y];

		std::vector<SwapOp> outSwaps;
		//timer to self destruct
		if ((cell1.ra >= 100)) {
			cell1.ra = 0;
			outSwaps.emplace_back(x, y, x, y, CellId::Smoke);
			return outSwaps;
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
				Entity air = Entity(CellId::Air);
				outSwaps.emplace_back(x, y, x, y, CellId::Air);
				break;
			} 
		}
		//return self swap so that partition stays enabled and timer can continue
		return outSwaps;
	}
}
namespace Smoke {
	std::vector<SwapOp> update(std::vector<std::vector<Entity>>& grid, int x, int y) {

		Entity& cell1 = grid[x][y];

		std::vector<SwapOp> outSwaps;
		//timer to self destruct
		if ((cell1.ra >= 75)) {
			cell1.ra = 0;
			outSwaps.emplace_back(x, y, x, y, CellId::Air);
			return outSwaps;
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
		//return self swap so that partition stays enabled and timer can continue
		return outSwaps;
	}
}

	std::vector<SwapOp> Entity::update(std::vector<std::vector<Entity>>& grid, int x, int y) {
		const Entity& target = grid[x][y];
		std::vector<SwapOp> outSwaps;

		switch (target.getId()) {
			case CellId::Air:
				return outSwaps;
			case CellId::Stone:
				return outSwaps;
			case CellId::Sand:
				return Sand::update(grid, x, y);
			case CellId::Water:
				return Water::update(grid, x, y);
			case CellId::Fire:
				return Fire::update(grid, x, y);
			case CellId::Smoke:
				return Smoke::update(grid, x, y);
			default:
				return outSwaps;
			}
	}

