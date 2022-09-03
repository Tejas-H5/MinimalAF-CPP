#pragma once

#include<string>
#include<map>
#include<gl/glew.h>
#include"glm.hpp"
#include"gtc/type_ptr.hpp"

namespace af {
    struct ShaderSources {
        std::string vertexSource;
        std::string fragmentSource;
    };

    class Shader {
    private:
        GLuint handle;
        GLuint modelLoc;
        GLuint viewLoc;
        GLuint projectionLoc;
        std::map<std::string, GLuint> uniformLocations;

        ShaderSources sources;
    public:
        Shader(ShaderSources sources);
        ~Shader();

        inline void use() {
            glUseProgram(handle);
        }

        inline void SetInt(GLint uniformLocation, int data) {
            use();
            glUniform1i(uniformLocation, data);
        }

        inline void SetFloat(GLint uniformLocation, float data) {
            use();
            glUniform1f(uniformLocation, data);
        }

        inline void SetMatrix4(GLint uniformLocation, glm::mat4x4& data) {
            use();
            glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(data));
        }

        inline void SetVector2(GLint uniformLocation, glm::vec2 data) {
            use();
            glUniform2f(uniformLocation, data.x, data.y);
        }

        inline void SetVector3(GLint uniformLocation, glm::vec3 data) {
            use();
            glUniform3f(uniformLocation, data.x, data.y, data.z);
        }

        inline void SetVector4(GLint uniformLocation, glm::vec4 data) {
            use();
            glUniform4f(uniformLocation, data.x, data.y, data.z, data.w);
        }
    };
}

/*
"uniform mat4 model;" +
"uniform mat4 projection;" +
"uniform mat4 view;" +
"layout(location = 0) in vec3 position;" +
"layout(location = 1) in vec2 uv;";
*/