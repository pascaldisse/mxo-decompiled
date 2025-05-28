#ifndef _CLT_RENDER_SYSTEM_H_
#define _CLT_RENDER_SYSTEM_H_

#include "../CLTBaseClass.h"
#include "../CLTVector.h"
#include <vector>
#include <string>
#include <map>

// Forward declarations
class CLTTransform;
class CLTCamera;
class CLTRenderPipeline;
class CLTShaderManager;
class CLTTextureManager;
class CLTRenderTarget;
class CLTMaterial;
class CLTModel;
class CLTLight;
class CLTViewport;

/**
 * @brief Graphics rendering system
 * 
 * The CLTRenderSystem class manages all rendering operations in the engine.
 * It provides interfaces for rendering 3D models, special effects, UI elements,
 * and manages the rendering pipeline, shaders, and textures.
 */
class CLTRenderSystem : public CLTBaseClass {
public:
    /**
     * @brief Rendering API type
     */
    enum RenderAPIType {
        RENDER_API_D3D8,     ///< DirectX 8
        RENDER_API_D3D9,     ///< DirectX 9
        RENDER_API_OPENGL,   ///< OpenGL
        RENDER_API_UNKNOWN   ///< Unknown API
    };
    
    /**
     * @brief Screen resolution structure
     */
    struct Resolution {
        uint32_t width;     ///< Screen width in pixels
        uint32_t height;    ///< Screen height in pixels
        uint32_t bpp;       ///< Bits per pixel
        uint32_t refreshRate; ///< Refresh rate in Hz
    };
    
    /**
     * @brief Rendering settings structure
     */
    struct RenderSettings {
        bool windowed;           ///< Whether to run in windowed mode
        bool vsync;              ///< Whether to use vertical sync
        bool tripleBuffering;    ///< Whether to use triple buffering
        bool antialiasing;       ///< Whether to use antialiasing
        uint32_t anisotropy;     ///< Anisotropic filtering level
        uint32_t textureQuality; ///< Texture quality level
        uint32_t shadowQuality;  ///< Shadow quality level
        uint32_t effectsQuality; ///< Special effects quality level
        float gamma;             ///< Gamma correction value
        float brightness;        ///< Brightness adjustment
        float contrast;          ///< Contrast adjustment
    };
    
    /**
     * @brief Fog parameters structure
     */
    struct FogParams {
        bool enabled;          ///< Whether fog is enabled
        CLTVector color;       ///< Fog color
        float start;           ///< Distance where fog starts
        float end;             ///< Distance where fog reaches maximum density
        float density;         ///< Fog density
        uint32_t mode;         ///< Fog mode (linear, exponential, etc.)
    };

public:
    /**
     * @brief Default constructor
     */
    CLTRenderSystem();
    
    /**
     * @brief Virtual destructor
     */
    virtual ~CLTRenderSystem();
    
    /**
     * @brief Initialize the rendering system
     * 
     * @param pInitParams Initialization parameters
     * @return true if initialization succeeded, false otherwise
     */
    virtual bool Init(void* pInitParams = nullptr) override;
    
    /**
     * @brief Clean up resources used by the rendering system
     */
    virtual void Term() override;
    
    /**
     * @brief Get the class name
     * 
     * @return The class name as a string
     */
    virtual const char* GetClassName() const override;
    
    /**
     * @brief Get the available screen resolutions
     * 
     * @param pResolutions Pointer to a vector to receive the resolutions
     * @return Number of resolutions found
     */
    uint32_t GetAvailableResolutions(std::vector<Resolution>* pResolutions);
    
    /**
     * @brief Set the screen resolution
     * 
     * @param width Screen width in pixels
     * @param height Screen height in pixels
     * @param bpp Bits per pixel
     * @param refreshRate Refresh rate in Hz
     * @return true if successful, false otherwise
     */
    bool SetResolution(uint32_t width, uint32_t height, uint32_t bpp = 32, uint32_t refreshRate = 60);
    
    /**
     * @brief Get the current screen resolution
     * 
     * @param pResolution Pointer to receive the resolution
     */
    void GetCurrentResolution(Resolution* pResolution);
    
    /**
     * @brief Set rendering settings
     * 
     * @param pSettings The new settings
     * @return true if successful, false otherwise
     */
    bool SetRenderSettings(const RenderSettings* pSettings);
    
    /**
     * @brief Get current rendering settings
     * 
     * @param pSettings Pointer to receive the settings
     */
    void GetRenderSettings(RenderSettings* pSettings);
    
    /**
     * @brief Set the active camera
     * 
     * @param pCamera The camera to use for rendering
     */
    void SetCamera(CLTCamera* pCamera);
    
    /**
     * @brief Get the active camera
     * 
     * @return Pointer to the active camera
     */
    CLTCamera* GetCamera() const;
    
    /**
     * @brief Begin a rendering frame
     * 
     * @return true if successful, false otherwise
     */
    bool BeginScene();
    
    /**
     * @brief End a rendering frame and present to the screen
     * 
     * @return true if successful, false otherwise
     */
    bool EndScene();
    
    /**
     * @brief Clear the render target
     * 
     * @param clearColor Whether to clear the color buffer
     * @param clearDepth Whether to clear the depth buffer
     * @param clearStencil Whether to clear the stencil buffer
     * @param color Color to clear with
     * @param depth Depth value to clear with
     * @param stencil Stencil value to clear with
     */
    void Clear(bool clearColor = true, bool clearDepth = true, bool clearStencil = false,
               const CLTVector& color = CLTVector(0, 0, 0), float depth = 1.0f, uint32_t stencil = 0);
    
    /**
     * @brief Set the active viewport
     * 
     * @param pViewport The viewport to use
     */
    void SetViewport(const CLTViewport* pViewport);
    
    /**
     * @brief Set the fog parameters
     * 
     * @param pFogParams The fog parameters to use
     */
    void SetFogParams(const FogParams* pFogParams);
    
    /**
     * @brief Get the current fog parameters
     * 
     * @param pFogParams Pointer to receive the fog parameters
     */
    void GetFogParams(FogParams* pFogParams);
    
    /**
     * @brief Set the ambient light color
     * 
     * @param color The ambient light color
     */
    void SetAmbientLight(const CLTVector& color);
    
    /**
     * @brief Get the ambient light color
     * 
     * @return The ambient light color
     */
    CLTVector GetAmbientLight() const;
    
    /**
     * @brief Add a dynamic light to the scene
     * 
     * @param pLight The light to add
     * @return Light ID for future reference
     */
    uint32_t AddLight(CLTLight* pLight);
    
    /**
     * @brief Remove a dynamic light from the scene
     * 
     * @param lightId The ID of the light to remove
     */
    void RemoveLight(uint32_t lightId);
    
    /**
     * @brief Update a dynamic light
     * 
     * @param lightId The ID of the light to update
     * @param pLight The updated light data
     */
    void UpdateLight(uint32_t lightId, CLTLight* pLight);
    
    /**
     * @brief Create a new material
     * 
     * @param pName Name for the material
     * @return Pointer to the created material
     */
    CLTMaterial* CreateMaterial(const char* pName);
    
    /**
     * @brief Load a material from a file
     * 
     * @param pFilename Path to the material file
     * @return Pointer to the loaded material
     */
    CLTMaterial* LoadMaterial(const char* pFilename);
    
    /**
     * @brief Find a material by name
     * 
     * @param pName Name of the material to find
     * @return Pointer to the material, or nullptr if not found
     */
    CLTMaterial* FindMaterial(const char* pName);
    
    /**
     * @brief Render a model
     * 
     * @param pModel The model to render
     * @param pTransform The transform to apply
     * @param pMaterial Optional material override
     */
    void RenderModel(CLTModel* pModel, const CLTTransform* pTransform, CLTMaterial* pMaterial = nullptr);
    
    /**
     * @brief Render the skybox
     */
    void RenderSkybox();
    
    /**
     * @brief Render a debug wireframe
     * 
     * @param pVertices Array of vertices
     * @param numVertices Number of vertices
     * @param pIndices Array of indices
     * @param numIndices Number of indices
     * @param color Color to use
     */
    void RenderDebugWireframe(const CLTVector* pVertices, uint32_t numVertices,
                             const uint32_t* pIndices, uint32_t numIndices,
                             const CLTVector& color);
    
    /**
     * @brief Take a screenshot
     * 
     * @param pFilename Path to save the screenshot
     * @return true if successful, false otherwise
     */
    bool TakeScreenshot(const char* pFilename);
    
    /**
     * @brief Get the current rendering API
     * 
     * @return The rendering API type
     */
    RenderAPIType GetRenderAPI() const;
    
    /**
     * @brief Get the shader manager
     * 
     * @return Pointer to the shader manager
     */
    CLTShaderManager* GetShaderManager() const;
    
    /**
     * @brief Get the texture manager
     * 
     * @return Pointer to the texture manager
     */
    CLTTextureManager* GetTextureManager() const;
    
    /**
     * @brief Get the render pipeline
     * 
     * @return Pointer to the render pipeline
     */
    CLTRenderPipeline* GetRenderPipeline() const;
    
    /**
     * @brief Set a rendering parameter
     * 
     * @param pName Parameter name
     * @param value Parameter value
     */
    void SetParameter(const char* pName, float value);
    
    /**
     * @brief Get a rendering parameter
     * 
     * @param pName Parameter name
     * @param defaultValue Default value to return if parameter is not found
     * @return The parameter value
     */
    float GetParameter(const char* pName, float defaultValue = 0.0f);
    
    /**
     * @brief Draw the NavMesh for debugging purposes
     * 
     * @param bShow Whether to show the NavMesh
     */
    void DrawNavMesh(bool bShow);
    
    /**
     * @brief Check if NavMesh drawing is enabled
     * 
     * @return true if NavMesh drawing is enabled, false otherwise
     */
    bool IsNavMeshDrawingEnabled() const;

private:
    RenderAPIType m_apiType;                     ///< Rendering API type
    Resolution m_currentResolution;              ///< Current screen resolution
    RenderSettings m_settings;                   ///< Current rendering settings
    FogParams m_fogParams;                       ///< Current fog parameters
    CLTVector m_ambientLight;                    ///< Ambient light color
    CLTCamera* m_pActiveCamera;                  ///< Active camera
    CLTRenderPipeline* m_pRenderPipeline;        ///< Render pipeline
    CLTShaderManager* m_pShaderManager;          ///< Shader manager
    CLTTextureManager* m_pTextureManager;        ///< Texture manager
    std::map<uint32_t, CLTLight*> m_lights;      ///< Dynamic lights
    std::map<std::string, CLTMaterial*> m_materials; ///< Materials
    std::map<std::string, float> m_parameters;   ///< Rendering parameters
    bool m_bDrawNavMesh;                         ///< Whether to draw NavMesh
    void* m_pD3DDevice;                          ///< DirectX device (if using D3D)
    void* m_pGLContext;                          ///< OpenGL context (if using OpenGL)
    bool m_bSceneActive;                         ///< Whether a scene is currently active
    uint32_t m_nextLightId;                      ///< Next light ID to assign
};

#endif // _CLT_RENDER_SYSTEM_H_