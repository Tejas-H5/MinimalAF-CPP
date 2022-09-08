#pragma once
#include "helpers.h"
#include <GL/glew.h>

namespace af {
	enum class FilteringType {
		NearestNeighbour,
		Bilinear	// TODO: rename to just 'Linear'
	};

	enum class ClampingType {
		Repeat,
		ClampToEdge
	};

	/*
	struct TextureImportSettings {
		public FilteringType Filtering = FilteringType.Bilinear;
		public ClampingType Clamping = ClampingType.Repeat;

		internal PixelInternalFormat InternalFormat = PixelInternalFormat.Rgba;
		internal PixelFormat PixelFormatType = PixelFormat.Bgra;
	}*/

	struct TextureImportSettings {
		FilteringType filtering;
		ClampingType clampingType;

	public:
		inline TextureImportSettings(FilteringType filtering, ClampingType clamping) :
			filtering(filtering), clampingType(clamping) {}
	};

	class Texture {
	private:
		int width, height, numChannels;

		GLuint textureID;

		std::string path;
		TextureImportSettings settings;

		bool init(unsigned char* data);
	public:
		Texture(int width, int height, TextureImportSettings settings);
		Texture(const std::string& path, TextureImportSettings settings);
		Texture(int width, int height, int numChannels, unsigned char* data, TextureImportSettings settings);

		// Please use the window.setTexture(this) wrapper instead of this->use(), otherwise state won't be intact anymore
		inline void use(GLenum textureUnit = GL_TEXTURE0) {
			glActiveTexture(textureUnit);
			glBindTexture(GL_TEXTURE_2D, textureID);
		}

		~Texture();
	};
}