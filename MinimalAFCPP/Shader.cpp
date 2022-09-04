#include "Shader.h"

using namespace af;

static std::string InsertGlobals(std::string& vertexSource) {
    int tStart = vertexSource.find("{{globals}}");
    int tEnd = tStart + 11;

    const char* s = "uniform mat4 model;\n"
        "uniform mat4 projection;\n"
        "uniform mat4 view;\n"
        "layout(location = 0) in vec3 position;\n"
        "layout(location = 1) in vec2 uv;\n";

    return vertexSource.replace(vertexSource.begin() + tStart, vertexSource.begin() + tEnd, s);
}


static int CompileShader(const std::string& code, GLenum type) {
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


static int LinkProgram(int vertexShader, int fragmentShader) {
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

Shader::Shader(const std::string& vertexSourceIn, const std::string& fragmentSource) {
    std::string vertexSource = std::string(vertexSourceIn);
    vertexSource = InsertGlobals(vertexSource);

    int vertexShader = CompileShader(vertexSource, GL_VERTEX_SHADER);
    int fragmentShader = CompileShader(fragmentSource, GL_FRAGMENT_SHADER);
    handle = LinkProgram(vertexShader, fragmentShader);

    // get locations of all uniforms in the shader
    uniformLocations = std::map<std::string, GLuint>();
    int numUniforms;
    glGetProgramiv(handle, GL_ACTIVE_UNIFORMS, &numUniforms);
    for (int i = 0; i < numUniforms; i++) {
        GLchar name[64] = { 0 };
        GLenum type;
        GLsizei size;
        GLint len;
        glGetActiveUniform(handle, i, 64, &len, &size, &type, name);
        int location = glGetUniformLocation(handle, name);
        uniformLocations[std::string(name, len)] = location;
    }

    modelLoc = uniformLocations["model"];
    viewLoc = uniformLocations["view"];
    projectionLoc = uniformLocations["projection"];
}

Shader::~Shader() {
    glDeleteProgram(handle);
    handle = 0;
}