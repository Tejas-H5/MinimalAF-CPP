#include "Text.h"
#include "Window.h"

#include <ft2build.h>
#include FT_FREETYPE_H

using namespace af;


unsigned char* rToRgba(unsigned char* src, int w, int h) {
    int len = 4 * w * h;
    unsigned char* rgbaBuffer = new unsigned char[len];
    for (int i = 0, iSrc = 0; i < len; i += 4, iSrc++) {
        rgbaBuffer[i + 0] = (unsigned char)(0xFF);
        rgbaBuffer[i + 1] = (unsigned char)(0xFF);
        rgbaBuffer[i + 2] = (unsigned char)(0xFF);

        rgbaBuffer[i + 3] = src[iSrc];
    }
    return rgbaBuffer;
}

void af::Font::load(const std::string& fontName, int fontSizePx) {
    sizePx = fontSizePx;
    this->fontName = fontName;
    height = 0;

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

    // this has a bunch of chars in it, they need to be put into a single texture
    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "Freetype couldn't load char " << c << std::endl;
            continue;
        }

        int w = face->glyph->bitmap.width;
        int h = face->glyph->bitmap.rows;

        unsigned char* bufferCopy = new unsigned char[w * h];
        unsigned char* src = face->glyph->bitmap.buffer;
        for (int i = 0; i < w * h; i++) {
            bufferCopy[i] = src[i];
        }

        characters[c] = Character(
            bufferCopy,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        );
    }


    // --- tile mapping --- 

    // 1. calculate max height of chars
    // 2. calculate max width of chars
    int maxWidth = 0, maxHeight = 0;
    for (int i = 0; i < 128; i++) {
        if (maxWidth < characters[i].size.x) {
            maxWidth = characters[i].size.x;
        }

        if (maxHeight < characters[i].size.y) {
            maxHeight = characters[i].size.y;
        }
    }

    height = maxHeight;

    maxWidth += 2;
    maxHeight += 2;

    // 3. make a grid with cells maxWidth * maxHeight and put the characters into each of those cells.
    //      we can make it approximately square by doing 8 * 16 cells. (maybe this is incorrect actually.
    //      (This part of the algo should work because most characters are about the same size. 
    //          It is really suboptimal when they aren't)
    int rows = 8, cols = 16;
    int imgWidth = cols * maxWidth;
    int imgHeight = rows * maxHeight;
    unsigned char* atlas = new unsigned char[imgWidth * imgHeight * 4];
    memset(atlas, 0, imgWidth * imgHeight * 4);
    for (int col = 0; col < cols; col++) {
        for (int row = 0; row < rows; row++) {
            uint8 c = row * cols+ col;
            Character ch = characters[c];

            characters[c].setTextureLocationPixels(ivec2(col * maxWidth, row * maxHeight), ivec2(imgWidth, imgHeight));

            // copy over our bitmap into it's slot
            for (int chCol = 0; chCol < ch.size.x; chCol++) {
                for (int chRow = 0; chRow < ch.size.y; chRow++) {
                    // int atlasOffset = (col * maxWidth + chCol) * 4 + (row * maxHeight + chRow) * imgWidth * 4;
                    int atlasOffset = (col * maxWidth + chCol) * 4 + (row * maxHeight + chRow) * imgWidth * 4;

                    uint8 val = ch.bitmap[chCol + chRow * ch.size.x];

                    atlas[atlasOffset + 0] = 0xFF;
                    atlas[atlasOffset + 1] = 0xFF;
                    atlas[atlasOffset + 2] = 0xFF;

                    // the bitmap belongs in the alpha channel
                    atlas[atlasOffset + 3] = val;
                }
            }

            delete[] ch.bitmap;
        }
    }

    fontTexture.load(imgWidth, imgHeight, 4, atlas, 
        TextureImportSettings(FilteringType::NearestNeighbour, ClampingType::ClampToEdge));
    
    delete[] atlas;

    Window::s_instance->setTexture(previousTexture);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}
