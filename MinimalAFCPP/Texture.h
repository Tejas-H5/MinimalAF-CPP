#pragma once
#include "helpers.h"

namespace af {
	enum class FilteringType {
		NearestNeighbour,
		Bilinear
	}

	/*
	struct TextureImportSettings {
		public FilteringType Filtering = FilteringType.Bilinear;
		public ClampingType Clamping = ClampingType.Repeat;

		internal PixelInternalFormat InternalFormat = PixelInternalFormat.Rgba;
		internal PixelFormat PixelFormatType = PixelFormat.Bgra;
	}*/

	Texture loadFromFile(std::string path, TextureImportSettings settings = null);

	class Texture {
	private:
	public:
		Texture(int width, int height, TextureImportSettings settings);
		Texture(SKBitmap image, TextureImportSettings settings = null);
		void Resize(int width, int height);
		void Init(int width, int height, int* data, TextureImportSettings settings);
		void BindTextureHandle();
		void Use(GLenum unit);
	};
}