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

void Entity::setLastUpdated(bool p_lastUpdated) {
    lastUpdated = p_lastUpdated;
}

int Entity::getId() const {
	return id;
}

Color Entity::getColor() const {
  switch (id) {
 		case 1:
			// grey
			return {200, 200, 200};
		case 2:
			// yellow
			return {245, 200, 70};
		case 3:
			// yellow
			return {0, 0, 255};
		default:
			return {0, 0, 0};
	}
}



