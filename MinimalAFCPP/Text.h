#pragma once

#include "helpers.h"
#include "Texture.h"
#include "GL/glew.h"
#include<map>

namespace af {
	struct Character {
	public:
		Texture tex;  // Glyph texture. takes ownership (for now. eventually it will be a part of an atlas and that will take ownership)
		ivec2   size;       // Size of glyph
		ivec2   bearing;    // Offset from baseline to left/top of glyph
		uint advance;    // Offset to advance to next glyph

		inline Character() {};

		inline void load(uint8* rgbaBuffer, ivec2 size, ivec2 bearing, uint advance) {
			this->size = size;
			this->bearing = bearing;
			this->advance = advance;

			tex.load(size.x, size.y, 4, rgbaBuffer,
				TextureImportSettings(FilteringType::Bilinear, ClampingType::ClampToEdge)
			);
		}

		inline ~Character() {
			print("char being destructed. idk whic one tho");
		}
	};


	class Font {
	private:
		int sizePx; // font height in pixels (? I think)

		string fontName;
		std::map<char, Character> characters;

	public:
		inline Font() {}

		void load(const std::string& fontName, int fontSizePx);

		inline Character* getChar(char c) {
			if (characters.count(c) == 0) {
				return false;
			}

			return &characters[c];
		}
	};
}