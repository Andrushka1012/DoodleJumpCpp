#pragma once

#include <array>
#include <GLES2/gl2.h>
#include "render-object.h"

// Forward declaration for GLuint (Android NDK provides typedef)
using GLuint = unsigned int;

namespace DoodleJumpGame {

    /**
     * Lightweight 2-D renderer built on top of OpenGL ES 2.0.
     * Рендерит простые примитивы (пока только квадрат) с возможностью
     * трансформации (позиция, масштаб, вращение) и установки цвета.
     * Все объекты игры используют общий статический vertex buffer —
     * меняется только матрица u_transform и цвет u_color.
     */
    class Renderer {
    public:
        Renderer() = default;
        ~Renderer();

        // Disable copy
        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;

        // Enable move semantics
        Renderer(Renderer&& other) noexcept;
        Renderer& operator=(Renderer&& other) noexcept;

        // Lifecycle
        bool initialize();  // Компиляция шейдеров, создание буферов
        void cleanup();
        void setViewport(int width, int height);

        // Frame helpers
        void clear(float r, float g, float b, float a = 1.0f);

        // Draw helpers
        void setColor(float r, float g, float b, float a = 1.0f);
        void setTransform(float x, float y, float scaleX = 1.0f, float scaleY = 1.0f, float rotationDeg = 0.0f);
        void resetTransform();

        // Primitive draw calls
        void drawQuad() const;

        void draw(RenderObject renderObject);

        [[nodiscard]] bool isInitialized() const { return isValid; }

    private:
        // Internal helpers
        GLuint compileShader(GLenum type, const char* source) const;
        GLuint createShaderProgram();
        void createQuadBuffer();
        bool checkShaderCompilation(GLuint shader) const;
        bool checkProgramLinking(GLuint program) const;

        // GL handles
        GLuint shaderProgram = 0;
        GLuint quadVbo = 0;
        GLuint positionAttr = 0;
        GLuint transformUniform = 0;
        GLuint colorUniform = 0;

        // State flag
        bool isValid = false;

        // Constants
        static constexpr int QUAD_VERTEX_COUNT = 4;
    };

} // namespace DoodleJumpGame