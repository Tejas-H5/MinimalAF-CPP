#pragma once
#include "Color.h"
#include "Datatypes/Rect.h"
#include "Datatypes/Vertex.h"
#include "helpers.h"

namespace af {
	struct SizeI {
		int width; int height;
		inline SizeI(int w, int h) : width(w), height(h) {}
		inline SizeI() : width(0), height(0) {}
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