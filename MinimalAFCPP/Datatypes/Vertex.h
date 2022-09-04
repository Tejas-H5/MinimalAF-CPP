#pragma once
#include "../helpers.h"

namespace af {
	struct Vertex {
		vec3 position;
		vec2 uv;

		Vertex(vec3 pos, vec2 uv) : position(pos), uv(uv) {}

		inline Vertex() {}
	};
}