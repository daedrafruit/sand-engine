#pragma once
#include <SDL.h>
#include <memory>
#include <vector>
struct SwapOperation;

struct Color {
    Uint8 r;
    Uint8 g;
    Uint8 b;
};

enum class CellId {
    Air,
    Stone,
    Sand,
    Water,
    Fire,
    Smoke
};

class Entity {
protected:
	int lastUpdated;
	int ra = 0;
public:

	Entity(int lastUpdated);
	virtual ~Entity() = default;

	void setLastUpdated(int p_lastUpdated);
	void setRegister(char reg, int value);

	int getRegister(char reg) const;
	int getLastUpdated() const;
	virtual CellId getId() const = 0;
	virtual Color getColor() const = 0;

	virtual std::unique_ptr<SwapOperation> update(const std::vector<std::vector<std::unique_ptr<Entity>>>& grid, int x, int y);

private:
};

// *********************************************************************
// Air
// *********************************************************************

class Air : public Entity {
public:
	using Entity::Entity;

	Color getColor() const override {
		return {0, 0, 0};
	}

	CellId getId() const override {
		return CellId::Air;
	}
};

// *********************************************************************
// Stone
// *********************************************************************

class Stone : public Entity {
public:
	using Entity::Entity;

	Color getColor() const override {
		return {200, 200, 200};
	}

	CellId getId() const override {
		return CellId::Stone;
	}
};

// *********************************************************************
// Sand
// *********************************************************************

class Sand : public Entity {
public:
	using Entity::Entity;

	std::unique_ptr<SwapOperation> update(const std::vector<std::vector<std::unique_ptr<Entity>>>& grid, int x, int y) override;

	Color getColor() const override {
		return {245, 200, 70};
	}

	CellId getId() const override {
		return CellId::Sand;
	}
};

// *********************************************************************
// Water
// *********************************************************************

class Water : public Entity {
public:
	using Entity::Entity;

	std::unique_ptr<SwapOperation> update(const std::vector<std::vector<std::unique_ptr<Entity>>>& grid, int x, int y) override;

	Color getColor() const override {
		return {0, 0, 255};
	}

	CellId getId() const override {
		return CellId::Water;
	}
};

// *********************************************************************
// Fire
// *********************************************************************

class Fire : public Entity {
public:
	using Entity::Entity;

	std::unique_ptr<SwapOperation> update(const std::vector<std::vector<std::unique_ptr<Entity>>>& grid, int x, int y) override;

	Color getColor() const override;

	CellId getId() const override {
		return CellId::Fire;
	}
};

// *********************************************************************
// Smoke
// *********************************************************************

class Smoke : public Entity {
public:
	using Entity::Entity;

	std::unique_ptr<SwapOperation> update(const std::vector<std::vector<std::unique_ptr<Entity>>>& grid, int x, int y) override;

	Color getColor() const override {
			return {70, 70, 70};
	}

	CellId getId() const override {
		return CellId::Smoke;
	}
};

// *********************************************************************
// Swap Logic
// *********************************************************************

/*

	 Swaps are usually very simple, just the coordinates of two cells to be swapped, however sometimes a cell needs to be replaced, such as with air if the cell is to be removed
	 or fire -> smoke, in this case, we set x2 to -1, which signifies a replace operation, then, the second number determines which type of entity the cell is to be replaced with.

	 This logic is all abstracted in the swaps namespace, which allows the Entity::update methods to get the correct swap operation based on the entity type required, then it can pass
	 that same swap operation to the getReplaceSwap function (along with the currWorldUpdate to get a new, fresh, *unique*, entity pointer!

*/

struct SwapOperation {
  int x1, y1;
	int x2, y2;
	std::unique_ptr<Entity> newEntity;

	SwapOperation(int x1, int y1, int x2, int y2)
			: x1(x1), y1(y1), x2(x2), y2(y2), newEntity(nullptr) {}

	SwapOperation(int x1, int y1, int x2, int y2, std::unique_ptr<Entity> newEntity)
			: x1(x1), y1(y1), x2(x2), y2(y2), newEntity(std::move(newEntity)) {}

};

namespace swaps {

	inline const SwapOperation upLeft(int x, int y) { return {x, y, x-1, y-1}; }
	inline const SwapOperation above(int x, int y) { return {x, y, x, y-1}; }
	inline const SwapOperation upRight(int x, int y) { return {x, y, x+1, y-1}; }
	inline const SwapOperation left(int x, int y) { return {x, y, x-1, y}; }
	inline const SwapOperation right(int x, int y) { return {x, y, x+1, y}; }
	inline const SwapOperation downLeft(int x, int y) { return {x, y, x-1, y+1}; }
	inline const SwapOperation below(int x, int y) { return {x, y, x, y+1}; }
	inline const SwapOperation downRight(int x, int y) { return {x, y, x+1, y+1}; }
	inline const SwapOperation self(int x, int y) { return {x, y, x, y}; }
}
