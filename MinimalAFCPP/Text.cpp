#include "Text.h"
#include "Window.h"

#include <ft2build.h>
#include FT_FREETYPE_H

using namespace af;


unsigned char* rToRgba(unsigned char* src, int w, int h) {
    int len = 4 * w * h;
    unsigned char* rgbaBuffer = new unsigned char[len];
    for (int i = 0, iSrc = 0; i < len; i += 4, iSrc++) {
        for (int j = 0; j < 3; j++) {
            rgbaBuffer[i + j] = (unsigned char)0xFF;
        }

        rgbaBuffer[i + 3] = src[iSrc];
    }
    return rgbaBuffer;
}

af::Font::Font(const std::string& fontName, int fontSizePx) : sizePx(fontSizePx), fontName(fontName) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return;
    }

    FT_Face face;
    std::string path = string("res/fonts/") + fontName + ".ttf";
    if (FT_New_Face(ft, path.c_str(), 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font from " << path << std::endl;
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, fontSizePx);

    Texture* previousTexture = Window::s_instance->getTexture();
    characters = new std::map<char, Character*>();

    for (uint8 c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "Freetype couldn't load char " << c << std::endl;
            continue;
        }

        int w = face->glyph->bitmap.width;
        int h = face->glyph->bitmap.rows;
        unsigned char* src = face->glyph->bitmap.buffer;
        unsigned char* rgbaBuffer = rToRgba(src, w, h);

        Texture* tex = new Texture(w, h, 4, rgbaBuffer,
            TextureImportSettings(FilteringType::Bilinear, ClampingType::ClampToEdge)
        );

        delete[] rgbaBuffer;

        characters->insert(std::pair<char, Character*>(c, new Character(
            tex,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        )));
    }

    Window::s_instance->setTexture(previousTexture);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

af::Font::~Font() {
    delete characters;
}
