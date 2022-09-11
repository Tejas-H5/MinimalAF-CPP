#pragma once

#include "helpers.h"
#include "Texture.h"
#include "GL/glew.h"
#include "Datatypes/Rect.h"
#include <vector>

namespace af {
	struct Character {
	public:
		// Glyph texture data. It will most likely be null, we are using it as a temp storage location
		// for rect packing
		unsigned char* bitmap;  
		ivec2   size;       // Size of glyph
		ivec2   bearing;    // Offset from baseline to left/top of glyph
		uint advance;    // Offset to advance to next glyph

		// the bitmap can be deleted
		ivec2 textureCoordPixels;
		ivec2 texSize;
		Rect textureCoords;

		inline Rect getTexCoord() { return textureCoords;  }
		inline void setTextureLocationPixels(ivec2 pos, ivec2 textureSize) {
			texSize = textureSize;
			textureCoordPixels = pos;
			textureCoords = Rect(
				textureCoordPixels.x / (float)texSize.x,
				textureCoordPixels.y / (float)texSize.y,
				(textureCoordPixels.x + size.x) / (float)texSize.x,
				(textureCoordPixels.y + size.y) / (float)texSize.y
			);
		}


		inline Character() {}

		inline Character(unsigned char* bitmap, ivec2 size, ivec2 bearing, uint advance) 
			: size(size),
			bearing(bearing),
			advance(advance),
			bitmap(bitmap) {}

		inline ~Character() {
			// print("char being destructed. idk whic one tho");
		}
	};


	class Font {
	private:
		int sizePx; // font height in pixels (? I think)

		string fontName;
		Texture fontTexture;
		Character characters[128];

		int height = 0;

	public:
		inline Font() {}

		void load(const std::string& fontName, int fontSizePx);

		inline Texture* getTexture() { return &fontTexture; }
		inline int getHeight() { return height;  }

		inline Character* getChar(char c) {
			if (c >= 128) {
				return nullptr;
			}

			return &characters[c];
		}
	};
}