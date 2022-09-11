#include "Texture.h"
#include "Window.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

bool af::Texture::init(unsigned char* data) {
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	GLenum wrapping = settings.clampingType == ClampingType::ClampToEdge ? GL_CLAMP_TO_EDGE : 
		settings.clampingType == ClampingType::Repeat ? GL_REPEAT : GL_REPEAT;

	GLenum filtering = settings.filtering == FilteringType::NearestNeighbour ? GL_NEAREST :
		settings.filtering == FilteringType::Bilinear ? GL_LINEAR : GL_NEAREST;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);


	GLenum imageFormat = numChannels == 4 ? GL_RGBA :
		numChannels == 3 ? GL_RGB :
		numChannels == 2 ? GL_RG :
		numChannels == 1 ? GL_RED :
		-1;

	GLenum internalFormat = numChannels == 4 ? GL_RGBA : 
		numChannels == 3 ? GL_RGBA :	// convert RGB to RGBA
		numChannels == 2 ? GL_RG :
		numChannels == 1 ? GL_RED :
		-1;


	// This is an invalid format
	assert(internalFormat != -1);

	glPixelStorei(GL_UNPACK_ALIGNMENT, numChannels == 4 ? 4 :
		numChannels == 3 ? 4 :
		numChannels == 2 ? 2 :
		numChannels == 1 ? 1 :
		0	// unreachable code
	);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D); // TODO: try disabling this, that's how we had it before in C# land

	Window::s_instance->onTextureLoaded(this);

	return true;
}

af::Texture::Texture(int width, int height, TextureImportSettings settings) : settings(settings) {
	this->width = width;
	this->height = height;
	this->numChannels = 4;

	// yes this is supposed to work. ?
	init(nullptr);
}

af::Texture::Texture(const std::string& path, TextureImportSettings settings) : settings(settings) {
	// stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &numChannels, 0);

	if (data) {
		init(data);
	}
	else {
		std::cerr << "Failed to load texture: " << path << std::endl;
		std::cerr << "error: " << path << std::endl;
	}

	stbi_image_free(data);
}

af::Texture::Texture(int width, int height, int numChannels, unsigned char* data, TextureImportSettings settings)
	: settings(settings)
{
	this->width = width;
	this->height = height;
	this->numChannels = numChannels;

	init(data);
}

af::Texture::~Texture() {
	glDeleteTextures(1, &textureID);
}
