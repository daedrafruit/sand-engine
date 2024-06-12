#pragma once
#include <vector>
#include <SDL.h>

struct Color {
    Uint8 r;
    Uint8 g;
    Uint8 b;
};

class Entity {
public:
	Entity(int p_id);


	void setId(int p_id);
	bool isEmpty();
	bool getLastUpdated() const;
	void setLastUpdated();

	int getId() const;

  virtual Color getColor() const;

	void update(std::vector<std::vector<Entity>>& p_grid, Entity& p_cell, int p_x, int p_y);

	

private:
	int id;
	bool lastUpdated;
};

class Sand : Entity {
	
};
