#include <SDL.h>
#include <vector>
#include <iostream>

#include "Entity.hpp"

Entity::Entity(int p_x, int p_y, int p_id, const int p_cellSize) 
	:x(p_x), y(p_y), id(p_id), cellSize(p_cellSize), lastUpdated(false) {
}

void Entity::updateCell(std::vector<Entity>& p_entities) {

    if (id < 1) {
        return;
    }
    
    for (Entity& e : p_entities) {

        if (!(e.getX() == x)) {
            continue;
        }

        if (e.getY() == y + cellSize) {
            return;
        }
    }
    y += cellSize;
    
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
        return {200, 200, 200};
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
