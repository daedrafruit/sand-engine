#pragma once
#include <SDL.h>

struct Color {
    Uint8 r;
    Uint8 g;
    Uint8 b;
};

enum class CellId {
	Empty,
	Stone,
	Sand,
	Water
};

class Entity {
public:
	Entity(CellId p_id);

	void setId(CellId p_id, int p_currWorldUpdate);
	bool isEmpty();
	int getLastUpdated() const;
	void setLastUpdated(int p_currWorldUpdate);

	CellId getId() const;

  virtual Color getColor() const;

private:
	CellId id;
	int lastUpdated;
};

