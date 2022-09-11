#pragma once

#include "Color.h"
#include "Datatypes/Rect.h"
#include "Datatypes/Vertex.h"
#include "helpers.h"

namespace af {
	// TODO: remove in favour of vec2i
	struct SizeI {
		int width; int height;
		inline SizeI(int w, int h) : width(w), height(h) {}
		inline SizeI() : width(0), height(0) {}
	};

	struct FpsTimer {
		time_point lastFrame;
		float frames = 0;
		float framerate = 0;

		// returns the current FPS based on how fast we're calling this method
		inline float tick(int frequencyMilliseconds) {
			frames++;

			time_point t = high_resolution_clock::now();
			if (t - lastFrame >= milliseconds(frequencyMilliseconds)) {
				lastFrame = high_resolution_clock::now();
				framerate = frames / ((float)frequencyMilliseconds / 1000.0f);
				frames = 0.0f;
			}

			return framerate;
		}
	};

	enum class CapType {
		None,
		Circle
	};

	enum class VAlign {
		Bottom, Center, Top
	};

	enum class HAlign {
		Left, Center, Right
	};
}