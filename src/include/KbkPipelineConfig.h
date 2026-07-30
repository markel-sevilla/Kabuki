#pragma once
 
#include <glad/glad.h> // o el header de carga de GL que uses (GLEW, etc.)
#include <array>
 
/**
 * @file    PipelineConfig.h
 * @brief   Configuración centralizada del estado del pipeline gráfico OpenGL.
 *
 * @details Agrupa todos los parámetros configurables del pipeline fijo de
 *          OpenGL (culling, depth, blending, stencil, scissor, rasterización,
 *          multisampling, máscaras de color, clear y framebuffer) en
 *          subestructuras independientes. Permite modificar el estado de
 *          render de forma centralizada y aplicar solo lo necesario.
 */
 
// ============================================================
// Culling
// ============================================================
struct CullingConfig
{
    bool    enabled   = true;
    GLenum  cullFace  = GL_BACK;   ///< GL_BACK | GL_FRONT | GL_FRONT_AND_BACK
    GLenum  frontFace = GL_CCW;    ///< GL_CCW | GL_CW
};
 
// ============================================================
// Depth Testing
// ============================================================
struct DepthConfig
{
    bool    testEnabled = true;
    bool    writeMask   = true;    ///< glDepthMask
    GLenum  depthFunc   = GL_LESS; ///< GL_LESS, GL_LEQUAL, GL_ALWAYS...
    double  nearVal     = 0.0;     ///< glDepthRange
    double  farVal      = 1.0;
    float   clearValue  = 1.0f;    ///< glClearDepth
};
 
// ============================================================
// Blending
// ============================================================
struct BlendConfig
{
    bool    enabled     = false;
    GLenum  srcFactor   = GL_SRC_ALPHA;
    GLenum  dstFactor   = GL_ONE_MINUS_SRC_ALPHA;
    GLenum  srcFactorA  = GL_ONE;   ///< para glBlendFuncSeparate (canal alpha)
    GLenum  dstFactorA  = GL_ZERO;
    bool    separateAlpha = false;  ///< si true, usa srcFactorA/dstFactorA
    GLenum  equation    = GL_FUNC_ADD; ///< GL_FUNC_ADD, GL_FUNC_SUBTRACT...
    std::array<float, 4> constantColor = {1.0f, 1.0f, 1.0f, 1.0f}; ///< glBlendColor
};
 
// ============================================================
// Stencil Testing
// ============================================================
struct StencilConfig
{
    bool    enabled    = false;
    GLenum  func       = GL_ALWAYS;
    GLint   ref        = 1;
    GLuint  funcMask   = 0xFF;
    GLuint  writeMask  = 0xFF;
    GLenum  sfail      = GL_KEEP;   ///< falla el stencil test
    GLenum  dpfail     = GL_KEEP;   ///< pasa stencil, falla depth
    GLenum  dppass     = GL_REPLACE;///< pasan ambos
    GLint   clearValue = 0;
};
 
// ============================================================
// Scissor Test
// ============================================================
struct ScissorConfig
{
    bool    enabled = false;
    GLint   x = 0;
    GLint   y = 0;
    GLsizei width  = 0;
    GLsizei height = 0;
};
 
// ============================================================
// Polygon / Rasterización
// ============================================================
struct PolygonConfig
{
    GLenum  mode        = GL_FILL; ///< GL_FILL, GL_LINE, GL_POINT
    GLenum  face        = GL_FRONT_AND_BACK;
    float   lineWidth    = 1.0f;
    float   pointSize    = 1.0f;
    bool    offsetFillEnabled = false;
    float   offsetFactor = 0.0f;
    float   offsetUnits  = 0.0f;
};
 
// ============================================================
// Multisampling / Antialiasing
// ============================================================
struct MultisampleConfig
{
    bool multisampleEnabled       = false;
    bool sampleAlphaToCoverage    = false;
    bool lineSmoothEnabled        = false;
    bool polygonSmoothEnabled     = false;
};
 
// ============================================================
// Máscaras de color / escritura
// ============================================================
struct ColorMaskConfig
{
    bool red   = true;
    bool green = true;
    bool blue  = true;
    bool alpha = true;
};
 
// ============================================================
// Clear (color de fondo + buffers a limpiar)
// ============================================================
struct ClearConfig
{
    std::array<float, 4> clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
    GLbitfield buffersToClear = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
};
 
// ============================================================
// Framebuffer / Viewport
// ============================================================
struct FramebufferConfig
{
    GLuint  fboID   = 0; ///< 0 = framebuffer por defecto
    GLint   viewportX = 0;
    GLint   viewportY = 0;
    GLsizei viewportWidth  = 1280;
    GLsizei viewportHeight = 720;
};
 
// ============================================================
// Estado misceláneo
// ============================================================
struct MiscConfig
{
    bool ditherEnabled            = true;
    bool programPointSizeEnabled  = false;
    bool primitiveRestartEnabled  = false;
    GLuint primitiveRestartIndex  = 0xFFFF;
    bool depthClampEnabled        = false;
};
 
/**
 * @brief  Configuración completa del estado del pipeline OpenGL.
 * @details Agrupa todas las subestructuras de configuración. Se puede
 *          modificar cualquier apartado y luego aplicarlo con apply().
 */
class KPipelineConfig
{
public:
    CullingConfig       culling;
    DepthConfig         depth;
    BlendConfig         blend;
    StencilConfig       stencil;
    ScissorConfig       scissor;
    PolygonConfig       polygon;
    MultisampleConfig   multisample;
    ColorMaskConfig     colorMask;
    ClearConfig         clear;
    FramebufferConfig   framebuffer;
    MiscConfig          misc;
 
    /// @brief Aplica todo el estado configurado a la GPU.
    void apply() const
    {
        applyCulling();
        applyDepth();
        applyBlend();
        applyStencil();
        applyScissor();
        applyPolygon();
        applyMultisample();
        applyColorMask();
        applyFramebuffer();
        applyMisc();
    }
 
    void applyCulling() const
    {
        culling.enabled ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
        glCullFace(culling.cullFace);
        glFrontFace(culling.frontFace);
    }
 
    void applyDepth() const
    {
        depth.testEnabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
        glDepthMask(depth.writeMask ? GL_TRUE : GL_FALSE);
        glDepthFunc(depth.depthFunc);
        glDepthRange(depth.nearVal, depth.farVal);
        glClearDepth(depth.clearValue);
    }
 
    void applyBlend() const
    {
        blend.enabled ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
        if (blend.separateAlpha)
            glBlendFuncSeparate(blend.srcFactor, blend.dstFactor,
                                 blend.srcFactorA, blend.dstFactorA);
        else
            glBlendFunc(blend.srcFactor, blend.dstFactor);
        glBlendEquation(blend.equation);
        glBlendColor(blend.constantColor[0], blend.constantColor[1],
                     blend.constantColor[2], blend.constantColor[3]);
    }
 
    void applyStencil() const
    {
        stencil.enabled ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
        glStencilFunc(stencil.func, stencil.ref, stencil.funcMask);
        glStencilOp(stencil.sfail, stencil.dpfail, stencil.dppass);
        glStencilMask(stencil.writeMask);
        glClearStencil(stencil.clearValue);
    }
 
    void applyScissor() const
    {
        scissor.enabled ? glEnable(GL_SCISSOR_TEST) : glDisable(GL_SCISSOR_TEST);
        if (scissor.enabled)
            glScissor(scissor.x, scissor.y, scissor.width, scissor.height);
    }
 
    void applyPolygon() const
    {
        glPolygonMode(polygon.face, polygon.mode);
        glLineWidth(polygon.lineWidth);
        glPointSize(polygon.pointSize);
        polygon.offsetFillEnabled ? glEnable(GL_POLYGON_OFFSET_FILL)
                                   : glDisable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(polygon.offsetFactor, polygon.offsetUnits);
    }
 
    void applyMultisample() const
    {
        multisample.multisampleEnabled ? glEnable(GL_MULTISAMPLE) : glDisable(GL_MULTISAMPLE);
        multisample.sampleAlphaToCoverage ? glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE)
                                           : glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
        multisample.lineSmoothEnabled ? glEnable(GL_LINE_SMOOTH) : glDisable(GL_LINE_SMOOTH);
        multisample.polygonSmoothEnabled ? glEnable(GL_POLYGON_SMOOTH) : glDisable(GL_POLYGON_SMOOTH);
    }
 
    void applyColorMask() const
    {
        glColorMask(colorMask.red, colorMask.green, colorMask.blue, colorMask.alpha);
    }
 
    /// @brief Limpia los buffers configurados con el color/valor actual.
    void applyClearAndClear() const
    {
        glClearColor(clear.clearColor[0], clear.clearColor[1],
                     clear.clearColor[2], clear.clearColor[3]);
        glClear(clear.buffersToClear);
    }
 
    void applyFramebuffer() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.fboID);
        glViewport(framebuffer.viewportX, framebuffer.viewportY,
                   framebuffer.viewportWidth, framebuffer.viewportHeight);
    }
 
    void applyMisc() const
    {
        misc.ditherEnabled ? glEnable(GL_DITHER) : glDisable(GL_DITHER);
        misc.programPointSizeEnabled ? glEnable(GL_PROGRAM_POINT_SIZE)
                                      : glDisable(GL_PROGRAM_POINT_SIZE);
        misc.primitiveRestartEnabled ? glEnable(GL_PRIMITIVE_RESTART)
                                      : glDisable(GL_PRIMITIVE_RESTART);
        if (misc.primitiveRestartEnabled)
            glPrimitiveRestartIndex(misc.primitiveRestartIndex);
        misc.depthClampEnabled ? glEnable(GL_DEPTH_CLAMP) : glDisable(GL_DEPTH_CLAMP);
    }
};
 