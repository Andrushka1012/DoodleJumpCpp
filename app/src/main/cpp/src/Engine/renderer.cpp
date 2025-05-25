#include "engine/renderer.h"
#include <GLES2/gl2.h>
#include <android/log.h>
#include <array>
#include <memory>
#include <cmath>

namespace DoodleJumpGame {

    namespace {
        constexpr const char *VERTEX_SHADER_SOURCE = R"(
            attribute vec4 position;
            uniform mat4 u_transform;
            void main() {
                gl_Position = u_transform * position;
            }
        )";

        constexpr const char *FRAGMENT_SHADER_SOURCE = R"(
            precision mediump float;
            uniform vec4 u_color;
            void main() {
                gl_FragColor = u_color;
            }
        )";

        // Квадрат (-0.5..0.5) Вершины по часовой стрелке для TRIANGLE_FAN
        constexpr std::array<float, 8> QUAD_VERTICES = {
                -0.5f, -0.5f,
                0.5f, -0.5f,
                0.5f,  0.5f,
                -0.5f,  0.5f
        };

        void logError(const char *msg) { __android_log_print(ANDROID_LOG_ERROR, "Renderer", "%s", msg); }
        void logInfo(const char *fmt, float a=0,float b=0,float c=0,float d=0,float e=0) {
            __android_log_print(ANDROID_LOG_INFO, "Renderer", fmt, a,b,c,d,e);
        }
    }

    /*======================  Lifecycle  ======================*/
    Renderer::~Renderer() { cleanup(); }

    Renderer::Renderer(Renderer &&other) noexcept {
        *this = std::move(other);
    }

    Renderer &Renderer::operator=(Renderer &&o) noexcept {
        if (this != &o) {
            cleanup();
            shaderProgram   = o.shaderProgram;   o.shaderProgram = 0;
            quadVbo         = o.quadVbo;         o.quadVbo = 0;
            positionAttr    = o.positionAttr;    o.positionAttr = 0;
            transformUniform= o.transformUniform; o.transformUniform = 0;
            colorUniform    = o.colorUniform;    o.colorUniform = 0;
            isValid         = o.isValid;         o.isValid = false;
        }
        return *this;
    }

    bool Renderer::initialize() {
        if (isValid) return true;

        shaderProgram = createShaderProgram();
        if (!shaderProgram) return false;

        positionAttr = static_cast<GLuint>(glGetAttribLocation(shaderProgram, "position"));
        transformUniform = static_cast<GLuint>(glGetUniformLocation(shaderProgram, "u_transform"));
        colorUniform = static_cast<GLuint>(glGetUniformLocation(shaderProgram, "u_color"));
        if (positionAttr == static_cast<GLuint>(-1) ||
            transformUniform == static_cast<GLuint>(-1) ||
            colorUniform == static_cast<GLuint>(-1)) {
            logError("Failed to get attribute/uniform location");
            cleanup();
            return false;
        }

        createQuadBuffer();
        isValid = quadVbo != 0;
        return isValid;
    }

    void Renderer::cleanup() {
        if (quadVbo) { glDeleteBuffers(1, &quadVbo); quadVbo = 0; }
        if (shaderProgram) { glDeleteProgram(shaderProgram); shaderProgram = 0; }
        isValid = false;
    }

    /*======================  Frame helpers  ======================*/
    void Renderer::clear(float r, float g, float b, float a) {
        if (!isValid) return;
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Renderer::setViewport(int width, int height) {
        glViewport(0, 0, width, height);
    }

    /*======================  Draw helpers  ======================*/
    void Renderer::setColor(float r, float g, float b, float a) {
        if (!isValid) return;
        glUseProgram(shaderProgram);
        glUniform4f(colorUniform, r, g, b, a);
    }

    void Renderer::setTransform(float x, float y, float sx, float sy, float rotDeg) {
        if (!isValid) return;
        float rad = rotDeg * static_cast<float>(M_PI) / 180.0f;
        float c = cosf(rad);
        float s = sinf(rad);
        float m[16] = {
                c*sx,  s*sx, 0, 0,
                -s*sy,  c*sy, 0, 0,
                0 ,     0, 1, 0,
                x,     y, 0, 1
        };
        glUseProgram(shaderProgram);
        glUniformMatrix4fv(transformUniform, 1, GL_FALSE, m);
    }

    void Renderer::resetTransform() {
        if (!isValid) return;
        const float I[16] = {1,0,0,0,  0,1,0,0,  0,0,1,0,  0,0,0,1};
        glUseProgram(shaderProgram);
        glUniformMatrix4fv(transformUniform, 1, GL_FALSE, I);
    }

    /*======================  Primitive draw  ======================*/
    void Renderer::drawQuad() const {
        if (!isValid) return;
        glUseProgram(shaderProgram);
        glBindBuffer(GL_ARRAY_BUFFER, quadVbo);
        glEnableVertexAttribArray(positionAttr);
        glVertexAttribPointer(positionAttr, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        glDrawArrays(GL_TRIANGLE_FAN, 0, QUAD_VERTEX_COUNT);
        glDisableVertexAttribArray(positionAttr);
    }

    /*======================  Internal  ======================*/
    GLuint Renderer::compileShader(GLenum type, const char* src) const {
        GLuint sh = glCreateShader(type);
        glShaderSource(sh, 1, &src, nullptr);
        glCompileShader(sh);
        if (!checkShaderCompilation(sh)) {
            glDeleteShader(sh);
            return 0;
        }
        return sh;
    }

    GLuint Renderer::createShaderProgram() {
        GLuint vs = compileShader(GL_VERTEX_SHADER, VERTEX_SHADER_SOURCE);
        if (!vs) return 0;
        GLuint fs = compileShader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER_SOURCE);
        if (!fs) { glDeleteShader(vs); return 0; }
        GLuint prog = glCreateProgram();
        glAttachShader(prog, vs);
        glAttachShader(prog, fs);
        glLinkProgram(prog);
        glDeleteShader(vs);
        glDeleteShader(fs);
        if (!checkProgramLinking(prog)) { glDeleteProgram(prog); return 0; }
        return prog;
    }

    void Renderer::createQuadBuffer() {
        glGenBuffers(1, &quadVbo);
        glBindBuffer(GL_ARRAY_BUFFER, quadVbo);
        glBufferData(GL_ARRAY_BUFFER, QUAD_VERTICES.size()*sizeof(float), QUAD_VERTICES.data(), GL_STATIC_DRAW);
    }

    bool Renderer::checkShaderCompilation(GLuint sh) const {
        GLint ok; glGetShaderiv(sh, GL_COMPILE_STATUS, &ok);
        if(!ok){
            char log[256]; glGetShaderInfoLog(sh, 256, nullptr, log);
            logError(log);
        }
        return ok == GL_TRUE;
    }

    bool Renderer::checkProgramLinking(GLuint prog) const {
        GLint ok; glGetProgramiv(prog, GL_LINK_STATUS, &ok);
        if(!ok){
            char log[256]; glGetProgramInfoLog(prog, 256, nullptr, log);
            logError(log);
        }
        return ok == GL_TRUE;
    }

} // namespace DoodleJumpGame