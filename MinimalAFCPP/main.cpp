#include "helpers.h"
#include<iostream>

#include "MinimalAF.h"

class App : public af::Window {
    af::Mesh* m;
    af::Shader* s;
public:
    inline App() : af::Window(1280, 720, "hi there bro") {}
    ~App() {
        delete m;
        delete s;
    }

    inline void initialize() override {
        float z = 0;
        af::Vertex* verts = new af::Vertex[]{
            af::Vertex(vec3(-0.5f, 0, z), vec2(0, 0)),
            af::Vertex(vec3(0.5f, 0, z), vec2(1, 0)),
            af::Vertex(vec3(0, 0.5f, z), vec2(0.5f, 1)),
        };

        uint* indices = new uint[]{ 0, 1, 2 };
        m = new af::Mesh(verts, 3, indices, 3, false);

        std::string vertexSource = "#version 330\n"
            "{{globals}}\n"
            "out vec2 uv0;\n"
            "void main() {\n"
            "   gl_Position = vec4(position, 1) * model * view * projection;\n"
            "   uv0 = uv;\n"
            "}";

        std::string fragSource = "#version 330\n"
            "uniform vec4 color;\n"
            "uniform sampler2D sampler;\n"
            "in vec2 uv0;\n"
            "void main(){\n"
            //"    vec4 texColor = texture2D(sampler, uv0.xy);\n"
            //"    gl_FragColor = color * texColor;\n"
            "    gl_FragColor = color;\n"
            "}\n";

        s = new af::Shader(vertexSource, fragSource);
        s->setVec4(s->loc("color"), af::rgb(1, 0, 0));
        s->setMatrix4(s->loc("model"), glm::identity<mat4>());
        s->setMatrix4(s->loc("view"), glm::identity<mat4>());
        s->setMatrix4(s->loc("projection"), glm::identity<mat4>());
    }

    inline void render() override {
        s->use();
        m->draw();
    }

    inline void update() override {}
};


int main(void) {
    if (af::init() != 0) {
        print("couldn not init af :(");
        std::cin.get();
        return -1;
    }

    App *a = new App();

    a->setState(af::WindowMode::MAXIMIZED);
    a->run();

    delete a;

    af::uninit();

    return 0;
}