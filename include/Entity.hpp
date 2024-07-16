#pragma once
#include <SDL.h>

struct Color {
    Uint8 r;
    Uint8 g;
    Uint8 b;
};

enum class CellId {
	Air,
	Stone,
	Sand,
	Water
};

class Entity {
public:

	Entity(CellId p_id) 
		:id(p_id), lastUpdated(0) {
	}

	Entity() 
		:id(CellId::Air), lastUpdated(0) {
	}

	void setId(CellId p_id, int p_currWorldUpdate) {
		id = p_id;
		lastUpdated = p_currWorldUpdate;
	}

	void setLastUpdated(int p_lastUpdated) {
			lastUpdated = p_lastUpdated;
	}

	inline int getLastUpdated() const { return lastUpdated; }

	inline CellId getId() const { return id; }

	bool isEmpty() const {
		return (id == CellId::Air);
	}

	Color getColor() const {
		switch (id) {
			case CellId::Stone:
				// grey
				return {200, 200, 200};
			case CellId::Sand:
				// yellow
				return {245, 200, 70};
			case CellId::Water:
				// blue
				return {0, 0, 255};
			default:
				return {0, 0, 0};
		}
	}


private:
	CellId id;
	int lastUpdated;
};

