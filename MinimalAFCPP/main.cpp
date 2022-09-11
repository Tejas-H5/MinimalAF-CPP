#include "helpers.h"
#include<iostream>
#include "MinimalAF.h"

class App : public af::Window {
    af::Texture tex;
    af::Font font;
    af::Font framerateFont;
    // af::Shader* shader;

    af::FpsTimer renderFpsTimer;
    float updateFps;
    af::FpsTimer updateFpsTimer;

    af::Shader shader;

    char framerateStrBuffer[64];
public:
    inline App() : af::Window(1280, 720, "hi there bro") {
        // glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    }

    inline void initialize() override {
        tex.load("res/textures/heavy_metal.jpg", af::TextureImportSettings(
            af::FilteringType::NearestNeighbour,
            af::ClampingType::Repeat
        ));
        font.load("Source Code Pro", 55);
        framerateFont.load("Source Code Pro", 22);

        //shader.loadInternal(
        //    // vertex shader
        //    "#version 330\n"
        //    "{{globals}}\n"
        //    "out vec2 uv0;\n"
        //    "void main(){\n"
        //     "    gl_Position =  projection * view * model * vec4(position, 1);\n"
        //    "    uv0 = uv;\n"
        //    "}\n",
        //    // fragment shader
        //    "#version 330\n"
        //    "uniform vec4 color;\n"
        //    "uniform sampler2D sampler;\n"
        //    "in vec2 uv0;\n"
        //    "void main(){\n"
        //    //"    vec4 texColor = texture2D(sampler, uv0.xy);\n"
        //    // "    vec4 texColor = \n"
        //    "    gl_FragColor = vec4(1,0,0,1);\n"
        //    "}\n"
        //);
    }

    inline void render() override {
        setDrawColor(af::rgb(1, 1, 1));

        float fps = renderFpsTimer.tick(1000);
        int len = sprintf(framerateStrBuffer, "r: %F, u: %F", fps, updateFps);

        setFont(&framerateFont);
        drawText(framerateStrBuffer, len, 0, getHeight(), af::HAlign::Left, af::VAlign::Top);
    }

    inline void update() override {
        updateFps = updateFpsTimer.tick(1000);
    }
};


int main(int argc, char* argv[]) {
    std::cout << "cwd: " << argv[0] << std::endl;

    if (af::init() != 0) {
        std::cin.get();
        return -1;
    }

    {
        App a;
        a.setTitle("bro, innit");

        print("Started");
        a.run();
    }

    print("ended");
    af::uninit();

    return 0;
}