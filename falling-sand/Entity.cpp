#include <SDL.h>
#include "Entity.hpp"

Entity::Entity(float p_x, float p_y, Uint8 p_r, Uint8 p_g, Uint8 p_b) 
	:x(p_x), y(p_y), r(p_r), g(p_g), b(p_b) {
}

float Entity::getX() {
	return x;
}

float Entity::getY() {
	return y;
}

Uint8 Entity::getR() {
	return r;
}

Uint8 Entity::getG() {
	return g;
}

Uint8 Entity::getB() {
	return b;
}

void Entity::setY(float p_y) {
	y = p_y;
}

void Entity::setX(float p_x) {
	x = p_x;
}
