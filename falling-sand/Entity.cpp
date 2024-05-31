#include <SDL.h>
#include "Entity.hpp"

Entity::Entity(int p_id) 
	:id(p_id) {
}

void Entity::setId(int p_id)
{
	id = p_id;
}

bool Entity::isEmpty()
{
    return (id == 0);
}

int Entity::getId()
{
	return id;
}

Uint8 Entity::getR() 
{
	if (id > 0) {
		return 200;
	}
	else {
		return 0;
	}
}

Uint8 Entity::getG() 
{

	if (id > 0) {
		return 200;
	}
	else {
		return 0;
	}
}

Uint8 Entity::getB() 
{
	if (id > 0) {
		return 200;
	}
	else {
		return 0;
	}
}

