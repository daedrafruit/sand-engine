#include <SDL.h>
#include "Entity.hpp"

Entity::Entity(CellId p_id) 
	:id(p_id), lastUpdated(0) {
}

void Entity::setId(CellId p_id, int p_currWorldUpdate) {
	id = p_id;
	lastUpdated = p_currWorldUpdate;
}

bool Entity::isEmpty() {
  return (id == CellId::Empty);
}

int Entity::getLastUpdated() const {
  return lastUpdated;
}

void Entity::setLastUpdated(int p_lastUpdated) {
    lastUpdated = p_lastUpdated;
}

CellId Entity::getId() const {
	return id;
}

Color Entity::getColor() const {
  switch (id) {
		case CellId::Stone:
			// grey
			return {200, 200, 200};
		case CellId::Sand:
			// yellow
			return {245, 200, 70};
		case CellId::Water:
			// yellow
			return {0, 0, 255};
		default:
			return {0, 0, 0};
	}
}



