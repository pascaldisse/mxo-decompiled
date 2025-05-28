#ifndef _CLT_NAVMESH_SYSTEM_H_
#define _CLT_NAVMESH_SYSTEM_H_

#include "../CLTBaseClass.h"
#include "../CLTVector.h"
#include <vector>
#include <map>
#include <string>

// Forward declarations
class CLTNavMeshController;
class CLTNavMeshPath;
class CLTNavMeshTrigger;

/**
 * @brief Structure representing a navigation mesh polygon
 */
struct NavMeshPoly {
    uint32_t id;                 ///< Unique polygon ID
    std::vector<CLTVector> vertices; ///< Polygon vertices
    CLTVector center;           ///< Polygon center point
    float height;               ///< Polygon height (Y coordinate)
    std::vector<uint32_t> neighbors; ///< Neighboring polygon IDs
    uint8_t flags;              ///< Polygon flags
    uint8_t area;               ///< Area type
};

/**
 * @brief Structure representing a path node
 */
struct NavMeshPathNode {
    CLTVector position;     ///< Node position
    uint32_t polyId;        ///< Polygon ID containing this node
    float cost;             ///< Path cost to this node
    float heuristic;        ///< Heuristic cost to goal
    float totalCost;        ///< Total cost (cost + heuristic)
    NavMeshPathNode* parent; ///< Parent node in path
};

/**
 * @brief Navigation mesh system
 * 
 * The CLTNavMeshSystem manages navigation meshes for character movement
 * and pathfinding. It provides functionality for loading navigation data,
 * finding paths, and testing positions for validity.
 */
class CLTNavMeshSystem : public CLTBaseClass {
public:
    /**
     * @brief Path finding result codes
     */
    enum PathFindResult {
        PATHFIND_SUCCESS,            ///< Path found successfully
        PATHFIND_PARTIAL,            ///< Partial path found
        PATHFIND_NO_PATH,            ///< No path found
        PATHFIND_INVALID_START,      ///< Invalid start position
        PATHFIND_INVALID_END,        ///< Invalid end position
        PATHFIND_OUT_OF_NODES,       ///< Ran out of path nodes
        PATHFIND_TIMEOUT,            ///< Operation timed out
        PATHFIND_ERROR               ///< Other error
    };
    
    /**
     * @brief Area type flags
     */
    enum AreaFlags {
        AREA_WALKABLE      = 0x01,   ///< Normal walkable area
        AREA_JUMP          = 0x02,   ///< Jump required
        AREA_WATER         = 0x04,   ///< Water area
        AREA_DOOR          = 0x08,   ///< Door area
        AREA_STAIRS        = 0x10,   ///< Stairs
        AREA_INDOORS       = 0x20,   ///< Indoor area
        AREA_NO_NAVIGATION = 0x40,   ///< Area where navigation is not allowed
        AREA_RESTRICTED    = 0x80    ///< Restricted area
    };
    
    /**
     * @brief Path finding options
     */
    struct PathFindOptions {
        uint32_t maxIterations;    ///< Maximum iterations for path finding
        uint32_t maxNodes;         ///< Maximum nodes to consider
        float maxDistance;         ///< Maximum path distance
        float straightPathTolerance; ///< Tolerance for straight path optimization
        bool optimizePath;         ///< Whether to optimize the path
        uint32_t areaFlags;        ///< Area flags to consider (bitfield)
        uint32_t excludedAreaFlags; ///< Area flags to exclude (bitfield)
        float timeout;             ///< Timeout in seconds
    };
    
    /**
     * @brief Ray casting result
     */
    struct RayCastResult {
        bool hit;               ///< Whether the ray hit anything
        CLTVector position;     ///< Hit position
        CLTVector normal;       ///< Surface normal at hit position
        float distance;         ///< Distance to hit
        uint32_t polyId;        ///< Polygon ID that was hit
    };

public:
    /**
     * @brief Default constructor
     */
    CLTNavMeshSystem();
    
    /**
     * @brief Virtual destructor
     */
    virtual ~CLTNavMeshSystem();
    
    /**
     * @brief Initialize the navigation system
     * 
     * @param pInitParams Initialization parameters
     * @return true if initialization succeeded, false otherwise
     */
    virtual bool Init(void* pInitParams = nullptr) override;
    
    /**
     * @brief Clean up resources used by the navigation system
     */
    virtual void Term() override;
    
    /**
     * @brief Get the class name
     * 
     * @return The class name as a string
     */
    virtual const char* GetClassName() const override;
    
    /**
     * @brief Load a navigation mesh from file
     * 
     * @param pFilename Path to the navigation mesh file
     * @param worldId Optional world ID
     * @return true if loaded successfully, false otherwise
     */
    bool LoadNavMesh(const char* pFilename, uint32_t worldId = 0);
    
    /**
     * @brief Unload a navigation mesh
     * 
     * @param worldId World ID of the mesh to unload
     * @return true if unloaded successfully, false otherwise
     */
    bool UnloadNavMesh(uint32_t worldId);
    
    /**
     * @brief Get a path between two points
     * 
     * @param start Start position
     * @param end End position
     * @param pPath Pointer to receive the path
     * @param pOptions Optional path finding options
     * @return Path finding result code
     */
    PathFindResult FindPath(const CLTVector& start, const CLTVector& end, 
                            CLTNavMeshPath* pPath, const PathFindOptions* pOptions = nullptr);
    
    /**
     * @brief Find a path with iteration limits
     * 
     * This is the main pathfinding implementation that was reverse engineered
     * from the game client.
     * 
     * @param start Start position
     * @param end End position
     * @param maxIterations Maximum iterations allowed
     * @param maxNodeCount Maximum nodes to process
     * @param pPath Pointer to receive the path
     * @return Path finding result code
     */
    PathFindResult FindPathToo(const CLTVector& start, const CLTVector& end,
                              int maxIterations, int maxNodeCount,
                              CLTNavMeshPath* pPath);
    
    /**
     * @brief Continue path finding from a previous partial result
     * 
     * @param pPath Path to continue finding
     * @param maxIterations Maximum iterations allowed
     * @param maxNodeCount Maximum nodes to process
     * @return Path finding result code
     */
    PathFindResult ContinuePath(CLTNavMeshPath* pPath, int maxIterations, int maxNodeCount);
    
    /**
     * @brief Find the nearest valid position on the navigation mesh
     * 
     * @param position Position to check
     * @param maxDistance Maximum search distance
     * @param pResult Pointer to receive the result
     * @return true if a valid position was found, false otherwise
     */
    bool FindNearestValidPosition(const CLTVector& position, float maxDistance, CLTVector* pResult);
    
    /**
     * @brief Check if a position is on the navigation mesh
     * 
     * @param position Position to check
     * @param polyId Optional pointer to receive the polygon ID
     * @return true if the position is on the navigation mesh, false otherwise
     */
    bool IsPositionValid(const CLTVector& position, uint32_t* polyId = nullptr);
    
    /**
     * @brief Check if a position is indoors
     * 
     * @param position Position to check
     * @return true if the position is indoors, false otherwise
     */
    bool IsIndoors(const CLTVector& position);
    
    /**
     * @brief Cast a ray against the navigation mesh
     * 
     * @param start Ray start position
     * @param end Ray end position
     * @param pResult Pointer to receive the result
     * @return true if the ray hit something, false otherwise
     */
    bool RayCast(const CLTVector& start, const CLTVector& end, RayCastResult* pResult);
    
    /**
     * @brief Get a random position on the navigation mesh
     * 
     * @param center Center position
     * @param radius Maximum distance from center
     * @param pResult Pointer to receive the random position
     * @return true if a position was found, false otherwise
     */
    bool GetRandomPosition(const CLTVector& center, float radius, CLTVector* pResult);
    
    /**
     * @brief Get polygon data
     * 
     * @param polyId Polygon ID
     * @param pPoly Pointer to receive polygon data
     * @return true if the polygon was found, false otherwise
     */
    bool GetPolygon(uint32_t polyId, NavMeshPoly* pPoly);
    
    /**
     * @brief Get all polygons in a region
     * 
     * @param center Center position
     * @param radius Radius around center
     * @param pPolygons Pointer to vector to receive polygons
     * @return Number of polygons found
     */
    uint32_t GetPolygonsInRegion(const CLTVector& center, float radius, std::vector<NavMeshPoly>* pPolygons);
    
    /**
     * @brief Add a navigation mesh trigger
     * 
     * @param pTrigger Trigger to add
     * @return Trigger ID for future reference
     */
    uint32_t AddTrigger(CLTNavMeshTrigger* pTrigger);
    
    /**
     * @brief Remove a navigation mesh trigger
     * 
     * @param triggerId ID of the trigger to remove
     * @return true if successful, false otherwise
     */
    bool RemoveTrigger(uint32_t triggerId);
    
    /**
     * @brief Update triggers
     * 
     * Check if any characters have entered or exited trigger volumes.
     */
    void UpdateTriggers();
    
    /**
     * @brief Draw the navigation mesh for debugging
     * 
     * @param bDraw Whether to draw the mesh
     */
    void DrawNavMesh(bool bDraw);
    
    /**
     * @brief Set navigation mesh parameters
     * 
     * @param checkBottom Bottom height check value
     * @param checkTop Top height check value
     */
    void SetNavMeshParams(float checkBottom, float checkTop);
    
    /**
     * @brief Get the current navigation mesh controller
     * 
     * @return Pointer to the active controller
     */
    CLTNavMeshController* GetNavMeshController() const;

private:
    // Internal helper methods
    NavMeshPathNode* AllocateNode();
    void FreeNode(NavMeshPathNode* pNode);
    uint32_t FindPolygon(const CLTVector& position, float maxDistance = 2.0f);
    bool IsPositionInPolygon(const CLTVector& position, const NavMeshPoly& poly);
    float CalculateHeuristic(const CLTVector& position, const CLTVector& goal);
    CLTNavMeshPath* ReconstructPath(NavMeshPathNode* pEndNode);
    CLTNavMeshPath* GetBestPartialPath();
    void OptimizePath(CLTNavMeshPath* pPath);
    
    // Navigation data
    std::map<uint32_t, CLTNavMeshController*> m_controllers; ///< NavMesh controllers by world ID
    CLTNavMeshController* m_pActiveController;              ///< Currently active controller
    std::map<uint32_t, NavMeshPoly> m_polygons;             ///< NavMesh polygons by ID
    std::map<uint32_t, CLTNavMeshTrigger*> m_triggers;      ///< NavMesh triggers by ID
    
    // Pathfinding data
    std::vector<NavMeshPathNode*> m_nodePool;               ///< Pool of pre-allocated path nodes
    std::vector<NavMeshPathNode*> m_openList;               ///< Open list for A* algorithm
    std::vector<NavMeshPathNode*> m_closedList;             ///< Closed list for A* algorithm
    uint32_t m_nextTriggerId;                               ///< Next trigger ID to assign
    
    // Navigation parameters
    float m_checkNavMeshBottom;                             ///< Bottom height check value
    float m_checkNavMeshTop;                                ///< Top height check value
    bool m_drawNavMesh;                                     ///< Whether to draw the NavMesh
    PathFindOptions m_defaultOptions;                       ///< Default path finding options
};

#endif // _CLT_NAVMESH_SYSTEM_H_