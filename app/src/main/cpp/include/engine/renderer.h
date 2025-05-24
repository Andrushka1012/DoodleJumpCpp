#pragma once

// Forward declarations
using GLuint = unsigned int;

namespace Engine {
    
    class Renderer {
    public:
        Renderer() = default;
        ~Renderer();
        
        // Delete copy constructor and assignment operator
        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;
        
        // Move constructor and assignment operator
        Renderer(Renderer&& other) noexcept;
        Renderer& operator=(Renderer&& other) noexcept;
        
        bool initialize();
        void cleanup();
        void render();
        void setViewport(int width, int height);
        
        bool isInitialized() const { return isValid; }
        
    private:
        GLuint createShader(unsigned int shaderType, const char* source) const;
        GLuint createShaderProgram();
        void createVertexBuffer();
        bool checkShaderCompilation(GLuint shader) const;
        bool checkProgramLinking(GLuint program) const;
        
        GLuint shaderProgram = 0;
        GLuint vertexBuffer = 0;
        GLuint positionAttribute = 0;
        bool isValid = false;
        
        static constexpr int VERTEX_COUNT = 4;
    };
    
    // C-style API for JNI compatibility
    void initRenderer();
    void renderFrame(); 
    void setViewport(int width, int height);
    void cleanupRenderer();
    
} // namespace Engine 