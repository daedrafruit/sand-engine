#include <cstddef>
#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include "SDL3/SDL_pixels.h"
#include "SDL_error.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_stdinc.h"

#include "SandWorld.hpp"
#include "Entity.hpp"
#include "RenderWindow.hpp"
#include "Utils.hpp"

RenderWindow::RenderWindow(const char* title, int p_width, int p_height)
	:window(NULL), renderer(NULL), width(p_width), height(p_height) {

	window = SDL_CreateWindow(title, p_width, p_height, 0);
	if (!window)
		std::cout << "Window failed to init" << SDL_GetError() << std::endl; 

	renderer = SDL_CreateRenderer(window, NULL);
	if (!renderer)
		std::cout << "Renderer failed to init" << SDL_GetError() << std::endl; 

	texture = SDL_CreateTexture(renderer, 
			SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 
			width, height
	);
	if (!texture)
		std::cout << "Texture failed to init" << SDL_GetError() << std::endl; 

	showDebug = false;

} 

void RenderWindow::cleanUp() {
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

void RenderWindow::clear() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	if (!SDL_RenderClear(renderer))
			std::cout << "Failed to clear renderer: " << SDL_GetError() << std::endl;
}


void RenderWindow::display() {
	if (!SDL_RenderPresent(renderer))
		std::cout << "Failed to present renderer: " << SDL_GetError() << std::endl;
}

void RenderWindow::handleEvent(const bool* currKeyStates, int p_x, int p_y) {
	if (currKeyStates[SDL_SCANCODE_GRAVE])
		showDebug = !showDebug;
}

void RenderWindow::renderPartitionBorders(const SandWorld& world, int x, int y, Uint32* pixels, int pixelPitch) {
	int pSize = world.getPartitionSizeInCells();
	int pX = x / pSize;
	int pY = y / pSize;

	int pModX = x % pSize;
	int pModY = y % pSize;
	int finalCell = pSize - 1;

	bool atSide = pModX == 0 || pModX == finalCell;
	bool atTop = pModY == 0 || pModY == finalCell;

	bool pActive = world.partitionActive(pX, pY);

	Uint32 green = 0x00ff00ffu;

	int index = y * pixelPitch + x;

	if (pActive && (atSide || atTop)) {
		pixels[index] = green;
	}
}

void RenderWindow::renderWorld(const SandWorld& world) {
	Uint32* pixels;
	int pitch;

	if (!SDL_LockTexture(texture, nullptr, (void **)&pixels, &pitch)) {
		std::cout << "Failed to lock texture: " << SDL_GetError() << std::endl;
	}

	int props = SDL_GetTextureProperties(texture);

	SDL_PixelFormatDetails pixelFormat;
	SDL_PixelFormat extractedFormat = static_cast<SDL_PixelFormat>(atoi(SDL_GetStringProperty(props, SDL_PROP_TEXTURE_FORMAT_NUMBER, 0))); 

	if (!extractedFormat) { std::cout << "Failed to extract pixel format: " << SDL_GetError() << std::endl; return; }
	pixelFormat.format = extractedFormat;

	int pixelPitch = pitch/sizeof(unsigned int);

	for (int y = 0; y < world.getGridHeight(); ++y) {
		for (int x = 0; x < world.getGridWidth(); ++x) {

			const Entity& cell = world.getCellAt(x, y);
			Color cellColor = cell.getColor();

			Uint32 color = utils::mapRGBA(cellColor.r, cellColor.g, cellColor.b, 0xFF);
			int pixelIndex = y * pixelPitch + x;

			pixels[pixelIndex] = color;

			if (showDebug)
				renderPartitionBorders(world, x, y, pixels, pixelPitch);
		}
	}

	SDL_UnlockTexture(texture);
	SDL_Rect rect = {0, 0, 
		width * world.getCellSize(),
		height * world.getCellSize()
	};
	SDL_FRect frect;
	SDL_RectToFRect(&rect, &frect);
	SDL_RenderRect(renderer, &frect);
	if (!SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST)) {
		std::cout << "Failed to set scale mode: " << SDL_GetError() << std::endl;
	}

	SDL_RenderTexture(renderer, texture, NULL, &frect);
}
