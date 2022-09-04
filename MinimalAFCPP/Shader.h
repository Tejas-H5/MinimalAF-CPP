#pragma once

#include<string>
#include<map>
#include<gl/glew.h>
#include"glm.hpp"
#include"gtc/type_ptr.hpp"
#include<iostream>

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
    };
}
