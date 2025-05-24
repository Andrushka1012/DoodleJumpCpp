#include "engine/renderer.h"
#include <GLES2/gl2.h>
#include <android/log.h>
#include <array>
#include <memory>
#include <cmath>

namespace Engine {

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
            void main() {
                gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
            }
        )";

        constexpr std::array<float, 12> TRIANGLE_VERTICES = {
                // Первый треугольник (нижний)
                -0.9f, -0.9f,  // Левый нижний
                 0.9f, -0.9f,  // Правый нижний  
                 0.0f,  0.0f,  // Центр
                
                // Второй треугольник (верхний)
                -0.9f,  0.9f,  // Левый верхний
                 0.9f,  0.9f,  // Правый верхний
                 0.0f,  0.0f   // Центр
        };

        void logError(const char *message) {
            __android_log_print(ANDROID_LOG_ERROR, "Engine::Renderer", "%s", message);
        }

        void logInfo(const char *message) {
            __android_log_print(ANDROID_LOG_INFO, "Engine::Renderer", "%s", message);
        }


    }

    Renderer::~Renderer() {
        cleanup();
    }

    Renderer::Renderer(Renderer &&other) noexcept
            : shaderProgram(other.shaderProgram), vertexBuffer(other.vertexBuffer), positionAttribute(other.positionAttribute),
              isValid(other.isValid) {

        other.shaderProgram = 0;
        other.vertexBuffer = 0;
        other.positionAttribute = 0;
        other.isValid = false;
    }

    Renderer &Renderer::operator=(Renderer &&other) noexcept {
        if (this != &other) {
            cleanup();

            shaderProgram = other.shaderProgram;
            vertexBuffer = other.vertexBuffer;
            positionAttribute = other.positionAttribute;
            isValid = other.isValid;

            other.shaderProgram = 0;
            other.vertexBuffer = 0;
            other.positionAttribute = 0;
            other.isValid = false;
        }
        return *this;
    }

    bool Renderer::initialize() {
        if (isValid) {
            logInfo("Renderer already initialized");
            return true;
        }

        logInfo("Initializing OpenGL renderer");

        shaderProgram = createShaderProgram();
        if (shaderProgram == 0) {
            logError("Failed to create shader program");
            return false;
        }

        positionAttribute = glGetAttribLocation(shaderProgram, "position");
        if (positionAttribute == static_cast<GLuint>(-1)) {
            logError("Failed to get position attribute location");
            cleanup();
            return false;
        }

        transformUniform = glGetUniformLocation(shaderProgram, "u_transform");
        if (transformUniform == static_cast<GLuint>(-1)) {
            logError("Failed to get transform uniform location");
            cleanup();
            return false;
        }

        createVertexBuffer();
        if (vertexBuffer == 0) {
            logError("Failed to create vertex buffer");
            cleanup();
            return false;
        }

        isValid = true;
        logInfo("Renderer initialized successfully");
        return true;
    }

    void Renderer::cleanup() {
        if (vertexBuffer != 0) {
            glDeleteBuffers(1, &vertexBuffer);
            vertexBuffer = 0;
        }

        if (shaderProgram != 0) {
            glDeleteProgram(shaderProgram);
            shaderProgram = 0;
        }

        isValid = false;
        logInfo("Renderer resources cleaned up");
    }

    void Renderer::render() {
        if (!isValid) {
            logError("Cannot renderOn: renderer not initialized");
            return;
        }

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        // Счетчик кадров для анимации
        static float frameCount = 0.0f;
        frameCount += 1.0f;
        
        // Вычисляем углы поворота
        float rotation1 = frameCount * 2.0f;        // Быстрое вращение
        float rotation2 = frameCount * 1.0f;        // Среднее вращение  
        float rotation3 = frameCount * 0.5f;        // Медленное вращение

        // Рисуем три вращающихся объекта
        setTransform(0.0f, 0.0f, 0.5f, 0.5f, rotation1);
        renderTriangles();
        
        setTransform(-0.6f, 0.0f, 0.3f, 0.3f, rotation2);
        renderTriangles();
        
        setTransform(0.6f, 0.0f, 0.3f, 0.3f, rotation3);
        renderTriangles();

        setTransform(0.0f, 0.5f, 0.3f, 0.3f, rotation1);
        renderTriangles();

        setTransform(0.0f, -0.5f, 0.3f, 0.3f, rotation1);
        renderTriangles();
    }

    void Renderer::setViewport(int width, int height) {
        glViewport(0, 0, width, height);
        logInfo("Viewport set");
    }

    GLuint Renderer::createShader(unsigned int shaderType, const char *source) const {
        GLuint shader = glCreateShader(shaderType);
        if (shader == 0) {
            logError("Failed to create shader");
            return 0;
        }

        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        if (!checkShaderCompilation(shader)) {
            glDeleteShader(shader);
            return 0;
        }

        return shader;
    }

    GLuint Renderer::createShaderProgram() {
        GLuint vertexShader = createShader(GL_VERTEX_SHADER, VERTEX_SHADER_SOURCE);
        if (vertexShader == 0) {
            return 0;
        }

        GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER_SOURCE);
        if (fragmentShader == 0) {
            glDeleteShader(vertexShader);
            return 0;
        }

        GLuint program = glCreateProgram();
        if (program == 0) {
            logError("Failed to create shader program");
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            return 0;
        }

        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        // Clean up shaders (they're linked into the program now)
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        if (!checkProgramLinking(program)) {
            glDeleteProgram(program);
            return 0;
        }

        return program;
    }

    void Renderer::createVertexBuffer() {
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER,
                     TRIANGLE_VERTICES.size() * sizeof(float),
                     TRIANGLE_VERTICES.data(),
                     GL_STATIC_DRAW);
    }

    bool Renderer::checkShaderCompilation(GLuint shader) const {
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success) {
            GLchar infoLog[512];
            glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
            logError("Shader compilation failed");
            __android_log_print(ANDROID_LOG_ERROR, "Engine::Renderer", "Shader error: %s", infoLog);
            return false;
        }

        return true;
    }

    bool Renderer::checkProgramLinking(GLuint program) const {
        GLint success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);

        if (!success) {
            GLchar infoLog[512];
            glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);
            logError("Program linking failed");
            __android_log_print(ANDROID_LOG_ERROR, "Engine::Renderer", "Program error: %s", infoLog);
            return false;
        }

        return true;
    }

    void Renderer::setTransform(float x, float y, float scaleX, float scaleY, float rotation) {
        if (!isValid) return;
        
        // Преобразуем градусы в радианы
        float radians = rotation * M_PI / 180.0f;
        float cosR = cos(radians);
        float sinR = sin(radians);
        
        // Матрица: Scale * Rotation * Translation
        float transform[16] = {
            cosR * scaleX,  sinR * scaleX,  0.0f, 0.0f,  // Rotation + Scale X
           -sinR * scaleY,  cosR * scaleY,  0.0f, 0.0f,  // Rotation + Scale Y
            0.0f,           0.0f,           1.0f, 0.0f,  // Z axis
            x,              y,              0.0f, 1.0f   // Translation
        };
        
        glUseProgram(shaderProgram);
        glUniformMatrix4fv(transformUniform, 1, GL_FALSE, transform);
    }
    
    void Renderer::resetTransform() {
        if (!isValid) return;
        
        // Единичная матрица (без трансформации)
        float identity[16] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
        
        glUseProgram(shaderProgram);
        glUniformMatrix4fv(transformUniform, 1, GL_FALSE, identity);
    }
    
    void Renderer::renderTriangles() const {
        if (!isValid) return;
        
        glUseProgram(shaderProgram);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glEnableVertexAttribArray(positionAttribute);
        glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        
        glDrawArrays(GL_TRIANGLES, 0, VERTEX_COUNT);
        
        glDisableVertexAttribArray(positionAttribute);
    }

} // namespace Engine 