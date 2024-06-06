#include <SDL.h>
#include "Entity.hpp"

Entity::Entity(int p_x, int p_y, int p_id) 
	:x(p_x), y(p_y), id(p_id), lastUpdated(false) {
}

bool Entity::isEmpty() {
    return (id == 0);
}

int Entity::getX() {
    return x;
}

int Entity::getY() {
    return y;
}

int Entity::getId() {
	return id;
}

bool Entity::getLastUpdated() {
    return lastUpdated;
}

Color Entity::getColor() const {
    if (id > 0) {
        //yellow
        return {245, 200, 70};
    } else {
        return {0, 0, 0};
    }
}

void Entity::setX(int p_x) {
    x = p_x;
}

void Entity::setY(int p_y) {
    y = p_y;
}

void Entity::setId(int p_id) {
	id = p_id;
}

void Entity::setLastUpdated(bool p_lastUpdated) {
    lastUpdated = p_lastUpdated;
}
