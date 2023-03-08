#ifndef MINIMAL_AF_H
#define MINIMAL_AF_H

// Going to make one of those header-only libs like that stb guy and fill it with comments
// (Also I am too lazy to figure out how to compile it to dll, so it will be easier for me to use this in other projects this way)

// ---- includes
// 3rd party
#include<glm.hpp>
#include<gtx/quaternion.hpp>
#include<gl/glew.h>
#include"GL/glew.h"
#include "GLFW/glfw3.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// stdlib
#include<string>
#include<map>
#include<iostream>
#include<cstdlib>
#include<memory>
#include<vector>
#include<chrono>

#define _USE_MATH_DEFINES
#include<math.h>



// ---- helpers
// TODO: remove some of these if they are not really needed

typedef std::chrono::high_resolution_clock high_resolution_clock;
typedef std::chrono::steady_clock::time_point time_point;
typedef std::chrono::seconds seconds;
typedef std::chrono::milliseconds milliseconds;

inline void print(const std::string& text) {
    std::cout << text << std::endl;
}

inline void err(const std::string& text) {
    std::cerr << text << std::endl;
}

inline void todo(const std::string& text) {
    std::cerr << "This functionality has not yet been implemented: " << text << std::endl;
}

inline float max(float a, float b) {
    return a > b ? a : b;
}

inline float min(float a, float b) {
    return a < b ? a : b;
}

inline float clamp(float v, float minVal, float maxVal) {
    if (v < minVal) {
        v = minVal;
    }

    if (v > maxVal) {
        v = maxVal;
    }

    return v;
}

template<class T>
inline void printArray(T* arr, const std::string& name, int len) {
    std::cout << name << ": ";
    for (int i = 0; i < len; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

typedef unsigned int uint;
typedef unsigned char uint8;

inline int find(char* str, int len, char c, int start) {
    for (int i = start; i < len; i++) {
        if (str[i] == c) {
            return i;
        }
    }
    return -1;
}

namespace af {
    // TODO: delete this
    using namespace glm;

    // ---- colors

    // same as glm::vec4, but with intent to make a color
    static inline glm::vec4 rgb(float r, float g, float b, float a = 1.0f) {
        return glm::vec4(r, g, b, a);
    }

    // same as glm::vec4, but intent to make a color and values are bytes between 0-255 that are converted to floats
    static inline glm::vec4 rgb255(uint8 r, uint8 g, uint8 b, uint8 a = 1.0f) {
        return rgb(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
    }

    // Make a color from HSV. Think of that colour circle you often see in drawing programs. 
    // h is the hue/color, s is the saturation (0 is no color, 1 is 100% color), v is the value (0 = pitch black, 1 = 100% brightness)
    static inline glm::vec4 hsv(float h, float s, float v, float a = 1.0f) {
        float c = v * s;
        float x = c * (1.0f - fmod(std::abs(h / 60.0f), 2.0f) - 1.0f);
        float m = v - c;
        float r = 0, g = 0, b = 0;
        if (h < 60) {
            r = c;
            g = x;
        }
        else if (h < 120) {
            r = x;
            g = c;
        }
        else if (h < 180) {
            g = c;
            b = x;
        }
        else if (h < 240) {
            g = x;
            b = c;
        }
        else if (h < 300) {
            r = x;
            b = c;
        }
        else {
            r = c;
            b = x;
        }

        return rgb(r + m, g + m, b + m, a);
    }

    // ---- datatypes


    struct SizeI {
        int width; int height;
        inline SizeI(int w, int h) : width(w), height(h) {}
        inline SizeI() : width(0), height(0) {}
    };

    struct FpsTimer {
        time_point lastFrame;
        float frames = 0;
        float framerate = 0;

        // returns the current FPS based on how fast we're calling this method
        inline float tick(int frequencyMilliseconds) {
            frames++;

            time_point t = high_resolution_clock::now();
            if (t - lastFrame >= milliseconds(frequencyMilliseconds)) {
                lastFrame = high_resolution_clock::now();
                framerate = frames / ((float)frequencyMilliseconds / 1000.0f);
                frames = 0.0f;
            }

            return framerate;
        }
    };

    enum class CapType {
        None,
        Circle
    };

    enum class VAlign {
        Bottom, Center, Top
    };

    enum class HAlign {
        Left, Center, Right
    };

    // ---- Objects

    template <class T> struct Mesh {
        T* vertices;
        GLuint* indices;
        uint indexCount;
        uint indexDrawCount;
        uint vertexCount;
        uint vertexDrawCount; // possibly redundant

        GLuint vbo;
        GLuint ebo;
        GLuint vao;
    
        // This mesh will take ownership of the verts and indices you pass in.
        // 
        // Set stream=true if you are going to update the data every frame with UpdateBuffers.
        // 
        // Use the initVertexAttributes function to initialize your mesh's vertex attributes somehow. (Javscript API moment) (No idea if this will work lmao)
        inline Mesh(T* data, uint numVerts, uint* indices, uint numIndices, void (*initVertexAttributes)(void), bool stream = false) {
            int bufferUsage = stream ? GL_STREAM_DRAW : GL_STATIC_DRAW;

            // make vertex array object, specify vertex attributes
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            // make vertex buffer object, give it our vertices
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, bufferUsage);


            /*GLuint currentOffset = 0;
            CreateVertexAttribPointer(0, 3, GL_FLOAT, currentOffset);
            currentOffset += 3 * sizeof(float);
            CreateVertexAttribPointer(1, 2, GL_FLOAT, currentOffset);
            currentOffset += 2 * sizeof(float);*/
            initVertexAttributes();

            // make element buffer object, give it our indices aka elements
            glGenBuffers(1, &ebo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(uint), indices, bufferUsage);

            printArray(indices, "indices", 3);

            glBindVertexArray(0);
        }

        // Uploads ALL the current verts and indices to the GPU. 
        // (You need to mutate them first though, hence no arguments)
        inline updateBuffers() {
            if (newIndexCount > indexCount || newVertexCount > vertexCount) {
                err("The mesh buffer does not have this many vertices. You may only specify new index and vertex counts that are less than the amount initially allocated");
            }

            vertexDrawCount = newVertexCount;
            indexDrawCount = newIndexCount;

            glBindVertexArray(vao);

            // Update vertex buffer
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, vertexDrawCount * sizeof(Vertex), vertices);

            // Update index buffer
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indexDrawCount * sizeof(uint), indices);
        }

        /// <summary>
        /// Uploads the first however many verts and indices to the GPU, and sets new vertex/index counts
        ///
        /// newVertexCount and newIndexCount MUST be less than the total number of vertices and indices on this mesh object.
        /// </summary>
        inline void updateBuffers(uint newVertexCount, uint newIndexCount) {
            updateBuffers((uint)vertexCount, (uint)indexCount);
        }

        inline void draw() {
            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, indexDrawCount, GL_UNSIGNED_INT, NULL);
        }

        inline ~Mesh() {
            glDeleteBuffers(1, &vbo);
            glDeleteBuffers(1, &ebo);
            glDeleteVertexArrays(1, &vao);

            delete vertices;
            delete indices;

            print("Mesh destructed");
        }
    };

    // Creates a vertex attrib pointer at index, with a given size, and whatever
    // TODO: should this really be a method? let's delete this as soon as we can. 
    template <class T> void CreateVertexAttribPointer(GLuint index, int size, GLenum type, int offsetInArray) {
        glVertexAttribPointer(index, size, type, false, sizeof(T), (const void*)offsetInArray);
        glEnableVertexAttribArray(index);
    }


    // A minimal shader class, I am too lazy to write this every time.
    // The source code, + some mechanism of storing uniform locations is up to you
    class Shader {
    private:
        static int compileShader(const std::string& code, GLenum type) {
            GLuint shader = glCreateShader(type);
            const char* cStr = code.c_str();
            GLint len = code.length();
            glShaderSource(shader, 1, &cStr, &len);

            glCompileShader(shader);

            GLint success = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

            if (!success) {
                GLchar error[1024] = { 0 };
                glGetShaderInfoLog(shader, sizeof(error), NULL, error);
                fprintf(stderr, "Error occurred while compiling shader (%d):\n\n%s", shader, error);
            }

            return shader;
        }

        static int linkProgram(int vertexShader, int fragmentShader) {
            int program = glCreateProgram();
            glAttachShader(program, vertexShader);
            glAttachShader(program, fragmentShader);

            glLinkProgram(program);
            int linkSuccess;
            glGetProgramiv(program, GL_LINK_STATUS, &linkSuccess);

            if (!linkSuccess) {
                GLchar error[1024] = { 0 };
                glGetShaderInfoLog(program, sizeof(error), NULL, error);
                fprintf(stderr, "Error occurred while linking program (%d):\n\n%s", program, error);
                return 0;
            }

            glDetachShader(program, vertexShader);
            glDetachShader(program, fragmentShader);
            glDeleteShader(fragmentShader);
            glDeleteShader(vertexShader);

            return program;
        }
    public:
        GLuint handle;
        std::string vertexSource;
        std::string fragmentSource;

        // make a new shader from a vertex and fragment.
        // takes in a handleUnifroms function like handleUniforms(name*, nameLength, location), you can optionally save the uniform
        // locations somewhere
        inline Shader(const std::string& vertexSource, const std::string& fragmentSource, void (*handleUniforms)(GLchar*, GLint, int)) {
            int vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
            int fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);
            handle = linkProgram(vertexShader, fragmentShader);

            if (handleUniforms != nullptr) {
                // get locations of all uniforms in the shader
                int numUniforms;
                glGetProgramiv(handle, GL_ACTIVE_UNIFORMS, &numUniforms);
                for (int i = 0; i < numUniforms; i++) {
                    GLchar name[128] = { 0 };
                    GLenum type;
                    GLsizei size;
                    GLint len;
                    glGetActiveUniform(handle, i, 64, &len, &size, &type, name);
                    int location = glGetUniformLocation(handle, name);

                    handleUniforms(name, len, location);
                }
            }
        }
        inline void unload() {
            glDeleteProgram(handle);
            handle = 0;
        }

        inline virtual ~Shader() {
            std::cout << "Shader destructed" << std::endl;
            unload();
        }

        inline void use() {
            glUseProgram(handle);
        }

        inline static void setInt(GLint uniformLocation, int data) {
            glUniform1i(uniformLocation, data);
        }

        inline static void setFloat(GLint uniformLocation, float data) {
            glUniform1f(uniformLocation, data);
        }

        inline static void setMatrix4(GLint uniformLocation, glm::mat4x4& data) {
            glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(data));
        }

        inline static void setVec2(GLint uniformLocation, glm::vec2 data) {
            glUniform2f(uniformLocation, data.x, data.y);
        }

        inline static void setVec3(GLint uniformLocation, glm::vec3 data) {
            glUniform3f(uniformLocation, data.x, data.y, data.z);
        }

        inline static void setVec4(GLint uniformLocation, glm::vec4 data) {
            glUniform4f(uniformLocation, data.x, data.y, data.z, data.w);
        }
    };

    // TODO: remove this commented thing when we can
    //class TrivialShader : public Shader {
    //private:
    //    glm::vec4 color; uint colorLoc;
    //public:
    //    void init() {
    //        loadInternal(
    //            // vertex shader
    //            "#version 330\n"
    //            "{{globals}}\n"
    //            "out vec2 uv0;\n"
    //            "void main(){\n"
    //            "    gl_Position =  projection * view * model * vec4(position, 1);\n"
    //            "    uv0 = uv;\n"
    //            "}\n",
    //            // fragment shader
    //            "#version 330\n"
    //            "uniform vec4 color;\n"
    //            "uniform sampler2D sampler;\n"
    //            "in vec2 uv0;\n"
    //            "void main(){\n"
    //            "    vec4 texColor = texture2D(sampler, uv0.xy);\n"
    //            "    gl_FragColor = color * texColor;\n"
    //            "}\n"
    //        );

    //        color = glm::vec4(0, 0, 0, 0);
    //        colorLoc = loc("color");
    //    }

    //    inline glm::vec4 getColor() { return color; }
    //    inline void setColor(glm::vec4 col) {
    //        color = col;
    //        setVec4(colorLoc, col);
    //    }
    //};


    struct Character {
    public:
        // Glyph texture data. It will most likely be null, we are using it as a temp storage location
        // for rect packing
        unsigned char* bitmap;
        glm::ivec2   size;       // Size of glyph
        glm::ivec2   bearing;    // Offset from baseline to left/top of glyph
        uint advance;    // Offset to advance to next glyph

        // the bitmap can be deleted
        glm::ivec2 textureCoordPixels;
        glm::ivec2 texSize;
        Rect textureCoords;

        inline Rect getTexCoord() { return textureCoords; }
        inline void setTextureLocationPixels(glm::ivec2 pos, glm::ivec2 textureSize) {
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

        inline Character(unsigned char* bitmap, glm::ivec2 size, glm::ivec2 bearing, uint advance)
            : size(size),
            bearing(bearing),
            advance(advance),
            bitmap(bitmap) {}

        inline ~Character() {
            // print("char being destructed. idk whic one tho");
        }
    };

    // TODO: signed distance field
    class Font {
    private:
        int sizePx; // font height in pixels (? I think)

        std::string fontName;
        Texture fontTexture;
        Character characters[128];

        int height = 0;

    public:
        inline Font(const std::string& fontName, int fontSizePx) {
            load(fontName, fontSizePx);
        }

        // Might want to make this callable multiple times or something, but I will need to use it a bit more to see the value in that
        inline void load(const std::string& fontName, int fontSizePx) {
            sizePx = fontSizePx;
            this->fontName = fontName;
            height = 0;

            FT_Library ft;
            if (FT_Init_FreeType(&ft)) {
                std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
                return;
            }

            FT_Face face;
            std::string path = std::string("res/fonts/") + fontName + ".ttf";
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
                    uint8 c = row * cols + col;
                    Character ch = characters[c];

                    characters[c].setTextureLocationPixels(glm::ivec2(col * maxWidth, row * maxHeight), ivec2(imgWidth, imgHeight));

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

        inline Texture* getTexture() { return &fontTexture; }
        inline int getHeight() { return height; }

        inline Character* getChar(char c) {
            if (c >= 128) {
                return nullptr;
            }

            return &characters[c];
        }
    };

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
        inline TextureImportSettings() {}

        inline TextureImportSettings(FilteringType filtering, ClampingType clamping) :
            filtering(filtering), clampingType(clamping) {}
    };

    class Texture {
    private:
        int width, height, numChannels;
        GLuint textureID;
        std::string path;
        TextureImportSettings settings;
        bool loaded;

        inline bool init(unsigned char* data) {
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
    public:
        inline Texture(int width, int height, TextureImportSettings settings) : loaded(false) {
            this->settings = settings;
            this->width = width;
            this->height = height;
            this->numChannels = 4;

            // yes this is supposed to work. ?
            init(nullptr);
        }

        inline Texture(const std::string& path, TextureImportSettings settings) : loaded(false) {
            this->settings = settings;

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

        inline Texture(int width, int height, int numChannels, unsigned char* data, TextureImportSettings settings) {
            this->settings = settings;
            this->width = width;
            this->height = height;
            this->numChannels = numChannels;

            init(data);
        }

        inline ~Texture() {
            std::cout << "Texture destructed" << std::endl;
            unload();
        }

        inline void unload() {
            glDeleteTextures(1, &textureID);
        }

        inline int getWidth() { return width; }
        inline int getHeight() { return height; }
         
        // Please use the window.setTexture(this) wrapper instead of this->use(), otherwise state won't be intact anymore
        // TODO: Make this the way we set textures and not 
        inline void use(GLenum textureUnit = GL_TEXTURE0) {
            glActiveTexture(textureUnit);
            glBindTexture(GL_TEXTURE_2D, textureID);
        }
    };


    int init();
    void uninit();

    enum class WindowMode {
        WINDOWED,
        FULLSCREEN,
        MAXIMIZED,
        MINIMIZED
    };

    struct ArcDrawConfig {
        int circleEdgeLength, maxCircleEdgeCount;
        ArcDrawConfig() : circleEdgeLength(5), maxCircleEdgeCount(32) {}
    };

    struct CurrentTextureState {
        Texture* currentTexture = nullptr;
        bool globalTextureChanged = true;
    };

    struct CurrentFontState {
        Font* currentFont = nullptr;
    };

    struct CurrentFramebufferState {

    };

    struct CurrentShaderState {
        mat4 model, view, projection;
        Shader* currentShader; // non-owning
    };



    int init() {
        if (!glfwInit()) {
            print("couldn't init glfw");
            return -1;
        }

        return 0;
    }

    void uninit() {
        print("glfwTerminate();");
        glfwTerminate();
    }




    class Window {
    private:
        GLFWwindow* window;

        SizeI windowSize;

        float current2DDepth = 0;
        Rect currentClippingRect;
        vec4 clearColor;

        ArcDrawConfig arcState;
        CurrentTextureState textureState;
        CurrentFramebufferState framebufferState;
        CurrentShaderState shaderState;
        CurrentFontState fontState;

        BufferedMeshOutput meshOutput;
        Texture nullTexture;	// a 1x1 white square

/*
*
*
            internal static TextureManager Texture = > s_textureManager;
            internal static FramebufferManager Framebuffer = > s_framebufferManager;
            public static ShaderManager Shader = > s_shaderManager

            internal static Color GetClearColor() {
            internal static void SetClearColor(Color color) {
            internal static void Clear() {
            internal static void Clear(Color col) {
            internal static void Flush() {
            internal static void SwapBuffers() {
            internal static void SetViewport(Rect screenRect) {
            internal static void Cartesian2D(float scaleX = 1, float scaleY = 1, float offsetX = 0, float offsetY = 0) {
            internal static void ViewLookAt(Vector3 position, Vector3 target, Vector3 up) {
            internal static void ViewOrientation(Vector3 position, Quaternion rotation) {
            internal static void Perspective(float fovy, float aspect, float depthNear, float depthFar, float centerX = 0, float centerY = 0) {
            internal static void Orthographic(float width, float height, float depthNear, float depthFar, float centerX = 0, float centerY = 0) {
            internal static void SetProjection(Matrix4 matrix) {
            internal static void SetBackfaceCulling(bool onOrOff) {
            internal static void SetTransform(Matrix4 matrix) {
            internal static void SetDrawColor(float r, float g, float b, float a) {
            internal static void SetDrawColor(Color col) {
            internal static void StartStencillingWhileDrawing(bool inverseStencil = false) {
            internal static void StartStencillingWithoutDrawing(bool inverseStencil = false) {
            private static void StartStencilling(bool canDraw, bool inverseStencil) {
            internal static void StartUsingStencil() {
            internal static void LiftStencil() {

                */

        void nlineStartLineSegment(float x, float y, vec2 dir, vec2 perp);
        void nlineMoveLineSegmentInDirectionOf(float x, float y, bool useAverage, vec2 dir, vec2 perp);
    public:
        static Window* s_instance;

        void onResize(GLFWwindow* window, int width, int height);
        void onTextureLoaded(Texture* t);

        Window(int w, int h, const std::string& title);
        virtual ~Window();

        void setState(WindowMode mode);
        void setSize(int w, int h);
        void setTitle(const std::string& title);
        SizeI getSize();
        int getWidth();
        int getHeight();
        bool shouldClose();
        void pollEvents();

        void setScreenRect(Rect rect);
        Rect getScreenRect();

        // ---- entire API

        template <class Vertex> inline void drawTriangle(Vertex& v1, Vertex& v2, Vertex& v3) {

        }
        template <class Vertex> inline void drawTriangle(float x0, float y0, float x1, float y1, float x2, float y2, float u0 = 0.0f, float v0 = 0.0f, float u1 = 0.5f, float v1 = 1.0f, float u2 = 1, float v2 = 0) {

        }
        template <class Vertex> inline void drawTriangleOutline(float thickness, float x0, float y0, float x1, float y1, float x2, float y2) {

        }
        template <class Vertex> inline void drawQuad(Vertex v1, Vertex v2, Vertex v3, Vertex v4) {

        }
        template <class Vertex> inline void drawQuad2D(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, float u0 = 0.0f, float v0 = 0.0f, float u1 = 1.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f, float u3 = 0.0f, float v3 = 1.0f) {

        }

        void drawRect(float x0, float y0, float x1, float y1, float u0 = 0, float v0 = 0, float u1 = 1, float v1 = 1);
        void drawRect(Rect rect, Rect uvs);
        void drawRect(Rect rect);
        void drawRectOutline(float thickness, Rect rect);
        void drawRectOutline(float thickness, float x0, float y0, float x1, float y1);

        void drawArc(float xCenter, float yCenter, float radius, float startAngle, float endAngle);
        void drawArc(float xCenter, float yCenter, float radius, float startAngle, float endAngle, int edgeCount);
        void drawArcOutline(float thickness, float x0, float y0, float r, float startAngle, float endAngle);
        void drawArcOutline(float thickness, float xCenter, float yCenter, float radius, float startAngle, float endAngle, int edgeCount);


        void drawCircle(float x0, float y0, float r, int edges);
        void drawCircle(float x0, float y0, float r);
        void drawCircleOutline(float thickness, float x0, float y0, float r, int edges);
        void drawCircleOutline(float thickness, float x0, float y0, float r);


        void drawLine(float x0, float y0, float x1, float y1, float thickness, CapType cap);
        void drawLineCap(float x0, float y0, float radius, CapType cap, float startAngle);
        void drawLineCircleCap(float x0, float y0, float thickness, float angle);

        // void drawLineOutline(float outlineThickness, float x0, float y0, float x1, float y1, float thickness, CapType cap);
        // void drawLineCapOutline(float outlineThickness, float x0, float y0, float radius, CapType cap, float startAngle);
        // void drawLineDefaultCapOutline(float thickness, float x0, float y0, float radius, float angle);
        // void drawLineCircleCapOutline(float thickness, float x0, float y0, float radius, float angle);

        // Texture getTextTexture();

        void setFont(Font* font);
        vec2 drawText(char* text, int len, float startX, float startY, HAlign hAlign, VAlign vAlign, float scale = 1.0f);
        vec2 drawText(char* text, int len, float startX, float startY, float scale = 1.0f);
        vec2 drawText(char* text, int len, int start, int end, float startX, float startY, float scale);
        float getTextHeight();
        float getTextWidth(char* s, int len);
        float getTextWidth(char* s, int len, int start, int end);

        // --- gl stuff
        vec4 getClearColor();
        void setClearColor(vec4 color);
        void clear();
        void flush();
        void swapBuffers();

        // --- internal shader uniforms state
        void setViewport(Rect screenRect);
        void cartesian2D(float scaleX = 1, float scaleY = 1, float offsetX = 0, float offsetY = 0);
        void setBackfaceCulling(bool onOrOff);
        void setDrawColor(vec4 col);
        void setShader(Shader* s, bool updateUniforms = true);

        // --- stencilling 
        // TODO: move to its own object, like struct StencilBuffer or something
        // void startStencillingWhileDrawing(bool inverseStencil = false);
        // void startStencillingWithoutDrawing(bool inverseStencil = false);
        // void startStencilling(bool canDraw, bool inverseStencil);
        // void startUsingStencil();
        // void liftStencil();

        // --- texture management
        void setTexture(Texture* texture);
        inline Texture* getTexture() { return textureState.currentTexture; }
    };
}
#include "Window.h"

#endif