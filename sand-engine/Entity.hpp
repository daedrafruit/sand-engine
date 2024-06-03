#pragma once
#include <SDL.h>

class Entity {
public:
	Entity(int p_id);
	void setId(int p_id);
	bool isEmpty();

	int getId();

	Uint8 getR();
	Uint8 getG();
    Uint8 getB();

private:
	int id;
};
