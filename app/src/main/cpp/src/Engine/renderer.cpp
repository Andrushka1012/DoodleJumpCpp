#include "engine/renderer.h"
#include <GLES2/gl2.h>
#include <android/log.h>
#include <array>
#include <memory>

namespace Engine {
    
    namespace {
        constexpr const char* VERTEX_SHADER_SOURCE = R"(
            attribute vec4 position;
            void main() {
                gl_Position = position;
            }
        )";
        
        constexpr const char* FRAGMENT_SHADER_SOURCE = R"(
            precision mediump float;
            void main() {
                gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
            }
        )";
        
        constexpr std::array<float, 8> RECTANGLE_VERTICES = {
            -0.5f, -0.5f,  // Bottom left
             0.5f, -0.5f,  // Bottom right
             0.5f,  0.5f,  // Top right
            -0.5f,  0.5f   // Top left
        };
        
        void logError(const char* message) {
            __android_log_print(ANDROID_LOG_ERROR, "Engine::Renderer", "%s", message);
        }
        
        void logInfo(const char* message) {
            __android_log_print(ANDROID_LOG_INFO, "Engine::Renderer", "%s", message);
        }
        
        // Global renderer instance for JNI compatibility
        std::unique_ptr<Renderer> g_renderer;
    }
    
    Renderer::~Renderer() {
        cleanup();
    }
    
    Renderer::Renderer(Renderer&& other) noexcept
        : shaderProgram(other.shaderProgram)
        , vertexBuffer(other.vertexBuffer)
        , positionAttribute(other.positionAttribute)
        , isValid(other.isValid) {
        
        other.shaderProgram = 0;
        other.vertexBuffer = 0;
        other.positionAttribute = 0;
        other.isValid = false;
    }
    
    Renderer& Renderer::operator=(Renderer&& other) noexcept {
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
            logError("Cannot render: renderer not initialized");
            return;
        }
        
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        
        glUseProgram(shaderProgram);
        
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glEnableVertexAttribArray(positionAttribute);
        glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        
        glDrawArrays(GL_TRIANGLE_FAN, 0, VERTEX_COUNT);
        
        glDisableVertexAttribArray(positionAttribute);
    }
    
    void Renderer::setViewport(int width, int height) {
        glViewport(0, 0, width, height);
        logInfo("Viewport set");
    }
    
    GLuint Renderer::createShader(unsigned int shaderType, const char* source) const {
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
                    RECTANGLE_VERTICES.size() * sizeof(float), 
                    RECTANGLE_VERTICES.data(), 
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
    
    // C-style API implementation
    void initRenderer() {
        if (!g_renderer) {
            g_renderer = std::make_unique<Renderer>();
        }
        g_renderer->initialize();
    }
    
    void renderFrame() {
        if (g_renderer) {
            g_renderer->render();
        }
    }
    
    void setViewport(int width, int height) {
        if (g_renderer) {
            g_renderer->setViewport(width, height);
        }
    }
    
    void cleanupRenderer() {
        g_renderer.reset();
    }
    
} // namespace Engine 