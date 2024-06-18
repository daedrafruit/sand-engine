#pragma once
#include <SDL.h>

#include "SandWorld.hpp"

class UserInterface {
public:
	UserInterface(SandWorld& p_world);
	void mouseEvent();

private:
	SandWorld& world;
};

