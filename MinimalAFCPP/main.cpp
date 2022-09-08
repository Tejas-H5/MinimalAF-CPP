#include "helpers.h"
#include<iostream>

#include "MinimalAF.h"

class App : public af::Window {
    af::Texture* tex;
    af::Font* font;
    af::Shader* shader;
public:
    inline App() : af::Window(1280, 720, "hi there bro") {}

    ~App() {
        delete tex;
        delete font;
        delete shader;
    }

    inline void initialize() override {
        tex = new af::Texture("res/textures/heavy_metal.jpg", af::TextureImportSettings(
            af::FilteringType::NearestNeighbour,
            af::ClampingType::Repeat
        ));

        setDrawColor(af::rgb(1, 1, 1));

        font = new af::Font("Source Code Pro", 22);
        //shader = new af::Shader(
        //    // vertex shader
        //    "#version 330\n"
        //    "{{globals}}\n"
        //    "out vec2 uv0;\n"
        //    "void main(){\n"
        //    "    gl_Position =  projection * view * model * vec4(position, 1);\n"
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
        float z = 0;
        setTexture(tex);
        drawTriangle(
            af::Vertex(vec3(-0.5f, -0.5f, z), vec2(0, 0)),
            af::Vertex(vec3(0.5f, -0.5f, z), vec2(1, 0)),
            af::Vertex(vec3(0, 0.5f, z), vec2(0.5f, 1))
             //af::Vertex(vec3(-50.0f, -50.0f, z), vec2(0, 0)),
             //af::Vertex(vec3(50.0f, -50.0f, z), vec2(1, 0)),
             //af::Vertex(vec3(0, 0.5f, z), vec2(0.5f, 1))
        );

        drawRect(0, 0, 100, 100);

        setTexture(nullptr);
        setFont(font);
        drawText("hello there", 0, 0, 1);
    }

    inline void update() override {}
};


int main(int argc, char* argv[]) {
    std::cout << "cwd: " << argv[0] << std::endl;

    if (af::init() != 0) {
        std::cin.get();
        return -1;
    }

    App *a = new App();

    a->setTitle("bro, innit");

    print("Started");

    a->run();

    delete a;

    af::uninit();

    return 0;
}