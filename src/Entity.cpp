#include <SDL3/SDL_stdinc.h>
#include <memory>
#include <algorithm>
#include <vector>

#include "Entity.hpp"
#include "Utils.hpp"

namespace Entity {
	std::vector<SwapOp> update(const std::vector<std::vector<std::unique_ptr<Entity>>>& grid, int x, int y) {

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

			if (cell2->id == CellId::Air || cell2->id == CellId::Water) {
				outSwaps.emplace_back(std::move(swap));
				break;
			}
		}
		return outSwaps;
	}
	Entity newEntity(CellId id, int worldUpdate) {
		Color color;
		switch (id) {
			default:
				color = Color{0, 0, 0};
			
		}

		return Entity{worldUpdate, 0, id, color};
	}

	void Entity::updateEntity(Entity& cell, CellId id, int worldUpdate) {
		cell.id = id;

	}
}

/*
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
