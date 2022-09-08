#pragma once

#include "helpers.h"
#include "Texture.h"
#include "GL/glew.h"
#include<map>

namespace af {
	struct Character {
	public:
		Texture* tex;  // Glyph texture. takes ownership (for now. eventually it will be a part of an atlas and that will take ownership)
		ivec2   size;       // Size of glyph
		ivec2   bearing;    // Offset from baseline to left/top of glyph
		uint advance;    // Offset to advance to next glyph

		inline Character() : tex(nullptr), advance(0)  {}

		inline Character(Texture *tex, ivec2 size, ivec2 bearing, uint advance) 
			: tex(tex), size(size), bearing(bearing), advance(advance) {}

		inline ~Character() {
			print("char being destructed. idk whic one tho");
			delete tex;
		}
	};


	class Font {
	private:
		int sizePx; // font height in pixels (? I think)

		string fontName;
		std::map<char, Character*> *characters;

	public:
		Font(const std::string& fontName, int fontSizePx);
		~Font();

		inline bool getChar(char c, Character** out) {
			if (characters->count(c) == 0) {
				return false;
			}

			out[0] = characters->at(c);
			return true;
		}
	};
}