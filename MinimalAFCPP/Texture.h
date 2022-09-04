#pragma once
#include "helpers.h"
#include <GL/glew.h>

namespace af {
	enum class FilteringType {
		NearestNeighbour,
		Bilinear
	};

	/*
	struct TextureImportSettings {
		public FilteringType Filtering = FilteringType.Bilinear;
		public ClampingType Clamping = ClampingType.Repeat;

		internal PixelInternalFormat InternalFormat = PixelInternalFormat.Rgba;
		internal PixelFormat PixelFormatType = PixelFormat.Bgra;
	}*/

	class Texture {
	private:
		GLint handle;
		int width, height;
		std::string path;
	public:
		Texture(int width, int height);
		Texture(const std::string& path);

		~Texture();
	};
}