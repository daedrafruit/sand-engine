#pragma once
#include <random>
#include <chrono>

#include <SDL3/SDL.h>
#include <SDL3/SDL_timer.h>

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


	inline Uint32 mapRGBA(int r, int g, int b, int a) {
		return ((r << 24) | (g << 16) | (b << 8) | a);
	}


/* at beginning place:
		{ float time = 0; { utils::timer timer(&time);

	 at end place:
	 	} std::cout << "\033[2J\033[1;1H" << time << std::endl;}
*/
	class timer {
		private:
			int begin, end;
			float* time;

		public:
			timer(float* t) {
				begin = SDL_GetTicks();
				time = t;
			}

			~timer() {
				std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
				float totalTime = (float)(SDL_GetTicks() - begin) / 1000;
				//std::cout << totalTime << std::endl;
				*time = totalTime;
			}
	};
}

