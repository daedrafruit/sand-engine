#pragma once
#include <SDL.h>

#include <vector>
#include "Entity.hpp"

namespace utils {	
	inline float hireTimeInSeconds() {
		float t = SDL_GetTicks();
		t *= 0.001f;

		return t;
	}

	void copy2DVector(const std::vector<std::vector<Entity>>& source, std::vector<std::vector<Entity>>& destination) {

		for (size_t i = 0; i < source.size(); ++i) {
			for (size_t j = 0; j < source[i].size(); ++j) {
				destination[i][j] = source[i][j];
			}
		}
	}
	}

