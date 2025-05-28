#include "../../include/gameplay/CLTNavMeshSystem.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <queue>
#include <set>

// Forward declaration for a required class
class CLTNavMeshController {};
class CLTNavMeshPath {};
class CLTNavMeshTrigger {};

CLTNavMeshSystem::CLTNavMeshSystem()
    : CLTBaseClass()
    , m_pActiveController(nullptr)
    , m_nextTriggerId(1)
    , m_checkNavMeshBottom(-50.0f)
    , m_checkNavMeshTop(50.0f)
    , m_drawNavMesh(false)
{
    m_sClassName = "CLTNavMeshSystem";
    m_nClassGUID = 0x3001;  // Example GUID, actual value would be different

    // Initialize default options
    m_defaultOptions.maxIterations = 2000;
    m_defaultOptions.maxNodes = 4096;
    m_defaultOptions.maxDistance = 1000.0f;
    m_defaultOptions.straightPathTolerance = 0.1f;
    m_defaultOptions.optimizePath = true;
    m_defaultOptions.areaFlags = AREA_WALKABLE;
    m_defaultOptions.excludedAreaFlags = AREA_NO_NAVIGATION;
    m_defaultOptions.timeout = 1.0f;

    // Pre-allocate path nodes
    for (int i = 0; i < 4096; ++i) {
        NavMeshPathNode* node = new NavMeshPathNode();
        node->parent = nullptr;
        m_nodePool.push_back(node);
    }
}

CLTNavMeshSystem::~CLTNavMeshSystem()
{
    // Clean up all controllers
    for (auto& pair : m_controllers) {
        if (pair.second) {
            delete pair.second;
        }
    }
    m_controllers.clear();
    
    // Clean up all triggers
    for (auto& pair : m_triggers) {
        if (pair.second) {
            delete pair.second;
        }
    }
    m_triggers.clear();
    
    // Clean up all path nodes
    for (auto& node : m_nodePool) {
        if (node) {
            delete node;
        }
    }
    m_nodePool.clear();
}

bool CLTNavMeshSystem::Init(void* pInitParams)
{
    // Call the base class implementation
    if (!CLTBaseClass::Init(pInitParams)) {
        return false;
    }
    
    // Default initialization is minimal - NavMesh data is loaded on demand
    return true;
}

void CLTNavMeshSystem::Term()
{
    // Clean up all controllers
    for (auto& pair : m_controllers) {
        if (pair.second) {
            delete pair.second;
        }
    }
    m_controllers.clear();
    m_pActiveController = nullptr;
    
    // Clean up all triggers
    for (auto& pair : m_triggers) {
        if (pair.second) {
            delete pair.second;
        }
    }
    m_triggers.clear();
    
    // Call the base class implementation
    CLTBaseClass::Term();
}

const char* CLTNavMeshSystem::GetClassName() const
{
    return m_sClassName.c_str();
}

bool CLTNavMeshSystem::LoadNavMesh(const char* pFilename, uint32_t worldId)
{
    // Check if this world already has a controller
    auto it = m_controllers.find(worldId);
    if (it != m_controllers.end()) {
        // Unload the existing controller
        delete it->second;
        m_controllers.erase(it);
    }
    
    // Create a new controller
    CLTNavMeshController* pController = new CLTNavMeshController();
    
    // TODO: Load navigation mesh data from file
    // In the original implementation, this would parse the NavMesh file format
    // and populate the polygon data.
    
    // For this reconstructed version, we'll just create a dummy controller
    m_controllers[worldId] = pController;
    
    // Set as active controller if we don't have one yet
    if (!m_pActiveController) {
        m_pActiveController = pController;
    }
    
    return true;
}

bool CLTNavMeshSystem::UnloadNavMesh(uint32_t worldId)
{
    // Find the controller for this world
    auto it = m_controllers.find(worldId);
    if (it == m_controllers.end()) {
        return false;  // Not found
    }
    
    // If this is the active controller, clear it
    if (m_pActiveController == it->second) {
        m_pActiveController = nullptr;
    }
    
    // Delete the controller and remove from map
    delete it->second;
    m_controllers.erase(it);
    
    return true;
}

CLTNavMeshSystem::PathFindResult CLTNavMeshSystem::FindPath(
    const CLTVector& start, const CLTVector& end,
    CLTNavMeshPath* pPath, const PathFindOptions* pOptions)
{
    // Use default options if not provided
    PathFindOptions options = pOptions ? *pOptions : m_defaultOptions;
    
    // Call the core path finding function
    return FindPathToo(start, end, options.maxIterations, options.maxNodes, pPath);
}

CLTNavMeshSystem::PathFindResult CLTNavMeshSystem::FindPathToo(
    const CLTVector& start, const CLTVector& end,
    int maxIterations, int maxNodeCount,
    CLTNavMeshPath* pPath)
{
    // Log debug info if enabled
    if (GetParameter("AI_LogNavMeshPathfinding", 0.0f) > 0.0f) {
        // In the actual implementation, this would call a logging function
        // LogMessage("CNavMeshPath::FindPathToo, max iterations %d, max node count %d\n", 
        //            maxIterations, maxNodeCount);
    }
    
    // Clear existing data
    m_openList.clear();
    m_closedList.clear();
    
    // Find the polygons containing start and end points
    uint32_t startPolyId = FindPolygon(start);
    uint32_t endPolyId = FindPolygon(end);
    
    // Check if valid positions
    if (startPolyId == 0) {
        return PATHFIND_INVALID_START;
    }
    
    if (endPolyId == 0) {
        return PATHFIND_INVALID_END;
    }
    
    // Initialize starting node
    NavMeshPathNode* startNode = AllocateNode();
    if (!startNode) {
        return PATHFIND_OUT_OF_NODES;
    }
    
    startNode->position = start;
    startNode->polyId = startPolyId;
    startNode->cost = 0.0f;
    startNode->heuristic = CalculateHeuristic(start, end);
    startNode->totalCost = startNode->heuristic;
    startNode->parent = nullptr;
    
    // Add start node to open list
    m_openList.push_back(startNode);
    
    // A* main loop
    int iterations = 0;
    while (!m_openList.empty() && iterations < maxIterations) {
        // Find the node with the lowest total cost
        auto it = std::min_element(m_openList.begin(), m_openList.end(),
            [](NavMeshPathNode* a, NavMeshPathNode* b) {
                return a->totalCost < b->totalCost;
            });
        
        NavMeshPathNode* current = *it;
        
        // Remove from open list and add to closed list
        m_openList.erase(it);
        m_closedList.push_back(current);
        
        // Check if we've reached the destination
        if (current->polyId == endPolyId) {
            // We found a path! Reconstruct and return it
            *pPath = *ReconstructPath(current);
            return PATHFIND_SUCCESS;
        }
        
        // Get the polygon for the current node
        NavMeshPoly currentPoly;
        if (!GetPolygon(current->polyId, &currentPoly)) {
            continue;  // Skip if polygon not found
        }
        
        // Process neighbors
        for (uint32_t neighborId : currentPoly.neighbors) {
            // Skip if we've already processed this polygon
            bool inClosedList = false;
            for (NavMeshPathNode* node : m_closedList) {
                if (node->polyId == neighborId) {
                    inClosedList = true;
                    break;
                }
            }
            
            if (inClosedList) {
                continue;
            }
            
            // Get neighbor polygon
            NavMeshPoly neighborPoly;
            if (!GetPolygon(neighborId, &neighborPoly)) {
                continue;  // Skip if neighbor polygon not found
            }
            
            // Calculate the new position and cost
            CLTVector newPos = (current->position + neighborPoly.center) * 0.5f;
            float newCost = current->cost + current->position.Distance(newPos);
            
            // Check if we already have this neighbor in the open list
            NavMeshPathNode* existingNode = nullptr;
            for (NavMeshPathNode* node : m_openList) {
                if (node->polyId == neighborId) {
                    existingNode = node;
                    break;
                }
            }
            
            // If the neighbor is already in the open list with a lower cost, skip it
            if (existingNode && existingNode->cost <= newCost) {
                continue;
            }
            
            // Create or update the neighbor node
            NavMeshPathNode* neighborNode;
            if (existingNode) {
                neighborNode = existingNode;
            } else {
                neighborNode = AllocateNode();
                if (!neighborNode) {
                    return PATHFIND_OUT_OF_NODES;  // Out of nodes
                }
                m_openList.push_back(neighborNode);
            }
            
            // Update node data
            neighborNode->position = newPos;
            neighborNode->polyId = neighborId;
            neighborNode->cost = newCost;
            neighborNode->heuristic = CalculateHeuristic(newPos, end);
            neighborNode->totalCost = neighborNode->cost + neighborNode->heuristic;
            neighborNode->parent = current;
        }
        
        iterations++;
    }
    
    // If we get here, we didn't find a complete path
    if (!m_openList.empty()) {
        // Find the best partial path (closest to destination)
        *pPath = *GetBestPartialPath();
        return PATHFIND_PARTIAL;
    }
    
    return PATHFIND_NO_PATH;
}

CLTNavMeshSystem::PathFindResult CLTNavMeshSystem::ContinuePath(
    CLTNavMeshPath* pPath, int maxIterations, int maxNodeCount)
{
    // In the original implementation, this would continue path finding
    // from a previous partial result.
    
    // For this reconstructed version, we'll just return a placeholder result
    return PATHFIND_NO_PATH;
}

bool CLTNavMeshSystem::FindNearestValidPosition(
    const CLTVector& position, float maxDistance, CLTVector* pResult)
{
    // Find the nearest polygon
    uint32_t polyId = FindPolygon(position, maxDistance);
    if (polyId == 0) {
        return false;  // No valid position found
    }
    
    // Get the polygon
    NavMeshPoly poly;
    if (!GetPolygon(polyId, &poly)) {
        return false;  // Polygon not found (shouldn't happen)
    }
    
    // Project the position onto the polygon
    *pResult = position;
    pResult->y = poly.height;  // Set to polygon height
    
    return true;
}

bool CLTNavMeshSystem::IsPositionValid(const CLTVector& position, uint32_t* polyId)
{
    // Find the polygon containing the position
    uint32_t foundPolyId = FindPolygon(position);
    
    // Store the polygon ID if requested
    if (polyId) {
        *polyId = foundPolyId;
    }
    
    return foundPolyId != 0;
}

bool CLTNavMeshSystem::IsIndoors(const CLTVector& position)
{
    // This is a reconstructed implementation based on radare2 analysis
    // of the original function at address 0x621729a0
    
    // Find the polygon containing the position
    uint32_t polyId = FindPolygon(position);
    if (polyId == 0) {
        return false;  // Not on the NavMesh
    }
    
    // Get the polygon
    NavMeshPoly poly;
    if (!GetPolygon(polyId, &poly)) {
        return false;  // Polygon not found (shouldn't happen)
    }
    
    // Check if the polygon has the indoor flag
    return (poly.area & AREA_INDOORS) != 0;
}

bool CLTNavMeshSystem::RayCast(
    const CLTVector& start, const CLTVector& end, RayCastResult* pResult)
{
    // In the original implementation, this would trace a ray against the NavMesh
    // to find the first intersection.
    
    // For this reconstructed version, we'll just return a placeholder result
    if (pResult) {
        pResult->hit = false;
        pResult->position = end;
        pResult->normal = CLTVector(0, 1, 0);
        pResult->distance = start.Distance(end);
        pResult->polyId = 0;
    }
    
    return false;
}

bool CLTNavMeshSystem::GetRandomPosition(
    const CLTVector& center, float radius, CLTVector* pResult)
{
    // Get polygons in the region
    std::vector<NavMeshPoly> polygons;
    uint32_t numPolys = GetPolygonsInRegion(center, radius, &polygons);
    
    if (numPolys == 0) {
        return false;  // No polygons in range
    }
    
    // Pick a random polygon
    int randomIndex = rand() % numPolys;
    const NavMeshPoly& poly = polygons[randomIndex];
    
    // Pick a random point within the polygon
    // For simplicity, we'll just use the center in this reconstructed version
    *pResult = poly.center;
    
    return true;
}

bool CLTNavMeshSystem::GetPolygon(uint32_t polyId, NavMeshPoly* pPoly)
{
    // Find the polygon
    auto it = m_polygons.find(polyId);
    if (it == m_polygons.end()) {
        return false;  // Not found
    }
    
    // Copy the data
    *pPoly = it->second;
    
    return true;
}

uint32_t CLTNavMeshSystem::GetPolygonsInRegion(
    const CLTVector& center, float radius, std::vector<NavMeshPoly>* pPolygons)
{
    pPolygons->clear();
    
    // Calculate squared radius for faster distance checks
    float radiusSq = radius * radius;
    
    // Check each polygon
    for (const auto& pair : m_polygons) {
        const NavMeshPoly& poly = pair.second;
        
        // Check if the polygon's center is within range
        if (poly.center.DistanceSquared(center) <= radiusSq) {
            pPolygons->push_back(poly);
        }
    }
    
    return pPolygons->size();
}

uint32_t CLTNavMeshSystem::AddTrigger(CLTNavMeshTrigger* pTrigger)
{
    // Assign a new trigger ID
    uint32_t triggerId = m_nextTriggerId++;
    
    // Store the trigger
    m_triggers[triggerId] = pTrigger;
    
    return triggerId;
}

bool CLTNavMeshSystem::RemoveTrigger(uint32_t triggerId)
{
    // Find the trigger
    auto it = m_triggers.find(triggerId);
    if (it == m_triggers.end()) {
        return false;  // Not found
    }
    
    // Delete the trigger and remove from map
    delete it->second;
    m_triggers.erase(it);
    
    return true;
}

void CLTNavMeshSystem::UpdateTriggers()
{
    // In the original implementation, this would check if any characters
    // have entered or exited trigger volumes.
    
    // For this reconstructed version, we'll just leave it empty
}

void CLTNavMeshSystem::DrawNavMesh(bool bDraw)
{
    m_drawNavMesh = bDraw;
}

void CLTNavMeshSystem::SetNavMeshParams(float checkBottom, float checkTop)
{
    m_checkNavMeshBottom = checkBottom;
    m_checkNavMeshTop = checkTop;
}

CLTNavMeshController* CLTNavMeshSystem::GetNavMeshController() const
{
    return m_pActiveController;
}

NavMeshPathNode* CLTNavMeshSystem::AllocateNode()
{
    // Find an unused node in the pool
    for (NavMeshPathNode* node : m_nodePool) {
        if (node->parent == nullptr) {
            return node;
        }
    }
    
    // If we get here, all nodes are in use
    return nullptr;
}

void CLTNavMeshSystem::FreeNode(NavMeshPathNode* pNode)
{
    if (pNode) {
        pNode->parent = nullptr;
    }
}

uint32_t CLTNavMeshSystem::FindPolygon(const CLTVector& position, float maxDistance)
{
    // In the original implementation, this would use spatial partitioning
    // to quickly find polygons near the position.
    
    // For this reconstructed version, we'll just check all polygons
    uint32_t bestPolyId = 0;
    float bestDistanceSq = maxDistance * maxDistance;
    
    for (const auto& pair : m_polygons) {
        const NavMeshPoly& poly = pair.second;
        
        // Check height bounds
        if (position.y < poly.height + m_checkNavMeshBottom ||
            position.y > poly.height + m_checkNavMeshTop) {
            continue;
        }
        
        // Calculate distance to polygon center (ignoring Y)
        CLTVector flatPos(position.x, 0, position.z);
        CLTVector flatCenter(poly.center.x, 0, poly.center.z);
        float distanceSq = flatPos.DistanceSquared(flatCenter);
        
        // If this is closer than our current best, check if the position is inside
        if (distanceSq < bestDistanceSq) {
            if (IsPositionInPolygon(position, poly)) {
                bestPolyId = pair.first;
                bestDistanceSq = 0.0f;  // Inside, so distance is 0
            } else if (bestPolyId == 0) {
                // If we don't have a polygon yet, use this one
                bestPolyId = pair.first;
                bestDistanceSq = distanceSq;
            }
        }
    }
    
    return bestPolyId;
}

bool CLTNavMeshSystem::IsPositionInPolygon(const CLTVector& position, const NavMeshPoly& poly)
{
    // This is a simplified implementation
    // The original would do proper point-in-polygon testing
    
    // For this reconstructed version, we'll just check the distance to the center
    // and assume the polygon is roughly circular
    CLTVector flatPos(position.x, 0, position.z);
    CLTVector flatCenter(poly.center.x, 0, poly.center.z);
    
    // Assume average radius of 2.0 units
    const float POLY_RADIUS = 2.0f;
    
    return flatPos.DistanceSquared(flatCenter) <= (POLY_RADIUS * POLY_RADIUS);
}

float CLTNavMeshSystem::CalculateHeuristic(const CLTVector& position, const CLTVector& goal)
{
    // Use straight-line distance as the heuristic
    return position.Distance(goal);
}

CLTNavMeshPath* CLTNavMeshSystem::ReconstructPath(NavMeshPathNode* pEndNode)
{
    // Create a new path
    CLTNavMeshPath* pPath = new CLTNavMeshPath();
    
    // TODO: In the original implementation, this would reconstruct the path
    // by following the parent pointers from the end node back to the start.
    
    return pPath;
}

CLTNavMeshPath* CLTNavMeshSystem::GetBestPartialPath()
{
    // Create a new path
    CLTNavMeshPath* pPath = new CLTNavMeshPath();
    
    // TODO: In the original implementation, this would find the node
    // in the open list that's closest to the destination.
    
    return pPath;
}

void CLTNavMeshSystem::OptimizePath(CLTNavMeshPath* pPath)
{
    // TODO: In the original implementation, this would optimize the path
    // by removing unnecessary waypoints.
}