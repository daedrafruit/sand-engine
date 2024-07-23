#pragma once
#include <SDL.h>
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

	template<typename T>
	void swap(T*& a, T*& b) {
			T* temp = a;
			a = b;
			b = temp;
	}

	template<typename T>
	T** createDynamicArray(int rows, int cols) {
    T** grid = new T*[cols];
    for (int i = 0; i < cols; ++i) {
        grid[i] = new T[rows];
    }
    return grid;
	}

	template<typename T>
	void deleteDynamicArray(T** grid, int cols) {
    for (int i = 0; i < cols; ++i) {
        delete[] grid[i];
    }
    delete[] grid;
	}

	template<typename T>
	void initializeDynamicArray(T** grid, int rows, int cols) {
		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < cols; ++j) {
				grid[i][j] = T();
			}
		}
	}

}

