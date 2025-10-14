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

enum class CellId : uint8_t {
		Null,
    Air,
    Stone,
    Sand,
    Water,
    Fire,
    Smoke,

};

struct Entity {
	int lastUpdated;
	int ra = 0;
	int density;
	CellId id;
	Entity(int worldUpdate, CellId p_id)
			: lastUpdated(worldUpdate), id(p_id) {}


	~Entity() = default;

	void setLastUpdated(int p_lastUpdated) { lastUpdated = p_lastUpdated; }
	int getLastUpdated() const { return lastUpdated; }

	void setRegister(char reg, int value);
	int getRegister(char reg) const;
	int getDensity() const { return density; }
	void setId(CellId p_id, int worldUpdate); 
	CellId getId() const { return id; }

	Color getColor() const;
	//returns unique ptr so that no op can be returned, consider using std::optional
	std::vector<SwapOp> update(const std::vector<std::vector<std::unique_ptr<Entity>>>& grid, int x, int y);

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
	CellId id;

	SwapOp(int x1, int y1, int x2, int y2)
			: x1(x1), y1(y1), x2(x2), y2(y2), id(CellId::Null) {}

	SwapOp(int x1, int y1, int x2, int y2, CellId id)
			: x1(x1), y1(y1), x2(x2), y2(y2), id(id) {}
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
