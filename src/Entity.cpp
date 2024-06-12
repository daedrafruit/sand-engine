#include <SDL.h>
#include "Entity.hpp"

Entity::Entity(int p_id) 
	:id(p_id), lastUpdated(false) {
}

void Entity::setId(int p_id) {
	id = p_id;
}

bool Entity::isEmpty() {
  return (id == 0);
}

bool Entity::getLastUpdated() const {
  return lastUpdated;
}

void Entity::setLastUpdated() {
  lastUpdated = !lastUpdated;
}

int Entity::getId() const {
	return id;
}

Color Entity::getColor() const {
  if (id == 1) {
    //grey
    return { 200, 200, 200 };
  }
  if (id == 2) {
    //yellow
    return {245, 200, 70};
  } else {
    return {0, 0, 0};
	}
}

void Entity::update(std::vector<std::vector<Entity>>& p_grid, Entity& p_cell, int p_x, int p_y) {

	p_cell.setLastUpdated();

	int x = p_x;
	int y = p_y;

	if (p_cell.isEmpty() || p_cell.getId() == 1) {
		return;
	}

	if (p_grid[x][y + 1].isEmpty()) {
		p_cell.setId(0);
		p_grid[x][y + 1].setId(2);
		p_grid[x][y + 1].setLastUpdated();
	}

	else if (p_grid[x - 1][y + 1].isEmpty()) {
		p_cell.setId(0);
		p_grid[x - 1][y + 1].setId(2);
		p_grid[x - 1][y + 1].setLastUpdated();
	} 

	else if (p_grid[x + 1][y + 1].isEmpty()) {
		p_cell.setId(0);
		p_grid[x + 1][y + 1].setId(2);
		p_grid[x + 1][y + 1].setLastUpdated();
	}
}


