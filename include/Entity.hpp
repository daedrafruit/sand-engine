#pragma once
#include <SDL3/SDL.h>
#include <memory>
#include <vector>

struct SwapOp;

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
	int density;
	const CellId id;
	const Color color;
	Entity(int p_lastUpdated, CellId p_id, Color p_color)
			: lastUpdated(p_lastUpdated), id(p_id), color(p_color) {}

public:

	virtual ~Entity() = default;

	void setLastUpdated(int p_lastUpdated) { lastUpdated = p_lastUpdated; }
	int getLastUpdated() const { return lastUpdated; }

	void setRegister(char reg, int value);
	int getRegister(char reg) const;
	int getDensity() const { return density; }
	CellId getId() const { return id; }

	virtual Color getColor() const { return color; }
	//returns unique ptr so that no op can be returned, consider using std::optional
	virtual std::vector<SwapOp> update(const std::vector<std::vector<std::unique_ptr<Entity>>>& grid, int x, int y);

};

class Air : public Entity {
public:
	Air(int p_lastUpdated)
		:Entity(p_lastUpdated, CellId::Air, {0,0,0}) {
	}
};

class Stone : public Entity {
public:
	Stone(int p_lastUpdated)
		:Entity(p_lastUpdated, CellId::Stone, {200,200,200}) {
	}
};

class Sand : public Entity {
public:
	Sand(int p_lastUpdated)
		:Entity(p_lastUpdated, CellId::Sand, {245,200,70}) {
	}

	std::vector<SwapOp> update(const std::vector<std::vector<std::unique_ptr<Entity>>>& grid, int x, int y) override;
};

class Water : public Entity {
public:
	Water(int p_lastUpdated)
		:Entity(p_lastUpdated, CellId::Water, {0,0,255}) {
	}
	
	std::vector<SwapOp> update(const std::vector<std::vector<std::unique_ptr<Entity>>>& grid, int x, int y) override;
};

class Fire : public Entity {
public:

	Fire(int p_lastUpdated)
		//fire color is determined by function
		:Entity(p_lastUpdated, CellId::Fire, {255,255,0}) {
	}

	Color getColor() const override;

	std::vector<SwapOp> update(const std::vector<std::vector<std::unique_ptr<Entity>>>& grid, int x, int y) override;
};

class Smoke : public Entity {
public:
	Smoke(int p_lastUpdated)
		:Entity(p_lastUpdated, CellId::Smoke, {70,70,70}) {
	}

	std::vector<SwapOp> update(const std::vector<std::vector<std::unique_ptr<Entity>>>& grid, int x, int y) override;

};

// *********************************************************************
// Swap Logic
// *********************************************************************

/*
		(x1, y1, x2, y2, optional replace entity)

	 Swaps are usually very simple, just the coordinates of two cells to be swapped, however sometimes a cell needs to be replaced, such as with air if the cell is to be removed or fire -> smoke, in this case, we use the newEntity variable, when this entity is not null, sandworld will know it is a replace operation, in a replace operation x2 and y2 are not used 
*/

struct SwapOp {
  int x1, y1;
	int x2, y2;
	std::unique_ptr<Entity> newEntity;

	SwapOp(int x1, int y1, int x2, int y2)
			: x1(x1), y1(y1), x2(x2), y2(y2), newEntity(nullptr) {}

	SwapOp(int x1, int y1, int x2, int y2, std::unique_ptr<Entity> newEntity)
			: x1(x1), y1(y1), x2(x2), y2(y2), newEntity(std::move(newEntity)) {}
};

//just functions to make update methods a bit more consistent and readable
//takes x/y, returns the correct swap operation
namespace swaps {
	inline SwapOp upLeft(int x, int y) { return {x, y, x-1, y-1}; }
	inline SwapOp above(int x, int y) { return {x, y, x, y-1}; }
	inline SwapOp upRight(int x, int y) { return {x, y, x+1, y-1}; }
	inline SwapOp left(int x, int y) { return {x, y, x-1, y}; }
	inline SwapOp right(int x, int y) { return {x, y, x+1, y}; }
	inline SwapOp downLeft(int x, int y) { return {x, y, x-1, y+1}; }
	inline SwapOp below(int x, int y) { return {x, y, x, y+1}; }
	inline SwapOp downRight(int x, int y) { return {x, y, x+1, y+1}; }
	inline SwapOp self(int x, int y) { return {x, y, x, y}; }
}
