#pragma once
#include <SDL.h>
#include <vector>

struct Color {
    Uint8 r;
    Uint8 g;
    Uint8 b;
};

class Entity {
public:
	Entity(int p_x, int p_y, int p_id, const int p_cellSize);

	void updateCell(std::vector<Entity>& p_entities);
	
	bool isEmpty();

	int getX();
	int getY();
	int getId();
	bool getLastUpdated();
    virtual Color getColor() const;

	void setX(int p_x);
	void setY(int p_y);
	void setId(int p_id);
	void setLastUpdated(bool p_lastUpdated);


private:
	int x, y, id;
	const int cellSize;
	bool lastUpdated;
};

