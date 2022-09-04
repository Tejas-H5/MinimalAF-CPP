#pragma once

#include<string>
#include<map>
#include<gl/glew.h>
#include"glm.hpp"
#include"gtc/type_ptr.hpp"
#include<iostream>
#include"helpers.h"

namespace af {
    /// <summary>
    /// Shader globals:
    /// "model"
    /// "view"
    /// "projection"
    /// "position"
    /// "uv"
    /// </summary>
    class Shader {
    private:
        GLuint handle;
        GLuint modelLoc;
        GLuint viewLoc;
        GLuint projectionLoc;
        std::map<std::string, GLuint> uniformLocations;

        std::string vertexSource;
        std::string fragmentSource;
    public:
        Shader(const std::string& vertexSource, const std::string& fragmentSource);
        ~Shader();

        inline void use() {
            glUseProgram(handle);
        }

        inline int loc(const std::string& name) {
            if (uniformLocations.count(name) == 0) {
                std::cerr << name << " isn't presint in this shader (" << handle << ")" << std::endl;
                return -1;
            }

            return uniformLocations[name];
        }

        inline void setInt(GLint uniformLocation, int data) {
            use();
            glUniform1i(uniformLocation, data);
        }

        inline void setFloat(GLint uniformLocation, float data) {
            use();
            glUniform1f(uniformLocation, data);
        }

        inline void setMatrix4(GLint uniformLocation, glm::mat4x4& data) {
            use();
            glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(data));
        }

        inline void setVec2(GLint uniformLocation, glm::vec2 data) {
            use();
            glUniform2f(uniformLocation, data.x, data.y);
        }

        inline void setVec3(GLint uniformLocation, glm::vec3 data) {
            use();
            glUniform3f(uniformLocation, data.x, data.y, data.z);
        }

        inline void setVec4(GLint uniformLocation, glm::vec4 data) {
            use();
            glUniform4f(uniformLocation, data.x, data.y, data.z, data.w);
        }

        inline void setModel(mat4 m) {
            setMatrix4(modelLoc, m);
        }

        inline void setView(mat4 m) {
            setMatrix4(viewLoc, m);
        }

        inline void setProjection(mat4 m) {
            setMatrix4(projectionLoc, m);
        }
    };

    class InternalShader : public Shader {
    private:
        vec4 color; uint colorLoc;
    public:
        InternalShader() : Shader(
            // vertex shader
            "#version 330\n"
            "{{globals}}\n"
            "out vec2 uv0;\n"
            "void main(){\n"
            "    gl_Position =  vec4(position, 1) * model * view * projection;\n"
            "    uv0 = uv;\n"
            "}\n",
            // fragment shader
            "#version 330\n"
            "uniform vec4 color;\n"
            "uniform sampler2D sampler;\n"
            "in vec2 uv0;\n"
            "void main(){\n"
            "    vec4 texColor = texture2D(sampler, uv0.xy);\n"
            "    gl_FragColor = color * texColor;\n"
            "}\n"
        ), color(vec4(0,0,0,0)), colorLoc(0) {
            colorLoc = loc("color");
        }

        inline vec4 getColor() { return color; }
        inline void setColor(vec4 col) {
            color = col;
            setVec4(colorLoc, col);
        }
    };
}
