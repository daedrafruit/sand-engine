#include <SDL.h>
#include <ios>

#include "UserInterface.hpp"
#include "SandWorld.hpp"

UserInterface::UserInterface(SandWorld& p_world)
	:world(p_world) {
}

void UserInterface::mouseEvent() {
	Uint32 mouseState = SDL_GetMouseState(nullptr, nullptr);
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	bool leftClick = (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT));
	bool rightClick = (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT));
	
	if (leftClick) {
		world.handleEvent(Event::leftMouse, mouseX, mouseY);
	} else if (rightClick) {
		world.handleEvent(Event::rightMouse, mouseX, mouseY);
	}
}
