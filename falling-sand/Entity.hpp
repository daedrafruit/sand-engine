#pragma once
#include <SDL.h>


class Entity 
{
public:
	Entity(float p_x, float p_y, Uint8 p_r, Uint8 p_g, Uint8 p_b);

	float getX();
	float getY();
	Uint8 getR();
	Uint8 getG();
    Uint8 getB();

	void setY(float p_y);
	void setX(float p_x);

private:
	float x, y;
	Uint8 r, g, b;
};
