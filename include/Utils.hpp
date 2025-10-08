#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_timer.h>
#include <random>

namespace utils {
	inline float hireTimeInSeconds() {
		float t = SDL_GetTicks();
		t *= 0.001f;

		return t;
	}

	inline std::mt19937& getRandomEngine() {
		static std::mt19937 engine(SDL_GetTicks());
		return engine;
	}


	inline void logicalOr2D(std::vector<std::vector<bool>>& array1, const std::vector<std::vector<bool>>& array2) {
		int rows = array1.size();
		int cols = array1[0].size(); // Assuming all rows have the same number of columns

		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < cols; ++j) {
				array1[i][j] = array1[i][j] || array2[i][j];
			}
		}
	}

	inline Uint32 mapRGBA(int r, int g, int b, int a) {
		return ((r << 24) | (g << 16) | (b << 8) | a);
	}

	inline auto timeFuncInvocation = 
    [](auto&& func, auto&&... params) {
        auto start = SDL_GetTicks();
        std::forward<decltype(func)>(func)(std::forward<decltype(params)>(params)...);
        auto stop = SDL_GetTicks();
        return stop - start;
     };
}

