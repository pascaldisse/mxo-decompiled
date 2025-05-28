#ifndef _CLT_GAME_OBJECT_H_
#define _CLT_GAME_OBJECT_H_

#include "../CLTObject.h"
#include <string>

// Forward declarations
class CLTTransform;
class CLTVector;
class CLTCollisionInfo;

/**
 * @brief Base class for all game world objects
 * 
 * CLTGameObject is the base class for all objects that exist in the game world,
 * such as characters, items, and world models. It extends CLTObject with
 * functionality for positioning, movement, and collision.
 */
class CLTGameObject : public CLTObject {
public:
    /**
     * @brief Default constructor
     */
    CLTGameObject();
    
    /**
     * @brief Virtual destructor
     */
    virtual ~CLTGameObject();
    
    /**
     * @brief Initialize the object
     * 
     * @param pInitParams Initialization parameters
     * @return true if initialization succeeded, false otherwise
     */
    virtual bool Init(void* pInitParams = nullptr) override;
    
    /**
     * @brief Clean up resources used by the object
     */
    virtual void Term() override;
    
    /**
     * @brief Get the class name
     * 
     * @return The class name as a string
     */
    virtual const char* GetClassName() const override;
    
    /**
     * @brief Check if this object is of the specified class or inherits from it
     * 
     * @param pClassName The class name to check against
     * @return true if this object is of the specified class or inherits from it
     */
    virtual bool IsKindOf(const char* pClassName) const override;
    
    /**
     * @brief Process update for this object
     * 
     * @param fDeltaTime Time in seconds since the last update
     */
    virtual void Update(float fDeltaTime) override;
    
    /**
     * @brief Get the object's position
     * 
     * @param pPos Pointer to receive the position
     */
    virtual void GetPosition(CLTVector* pPos) const;
    
    /**
     * @brief Set the object's position
     * 
     * @param pPos New position
     */
    virtual void SetPosition(const CLTVector* pPos);
    
    /**
     * @brief Get the object's rotation
     * 
     * @param pRot Pointer to receive the rotation
     */
    virtual void GetRotation(CLTVector* pRot) const;
    
    /**
     * @brief Set the object's rotation
     * 
     * @param pRot New rotation
     */
    virtual void SetRotation(const CLTVector* pRot);
    
    /**
     * @brief Get the object's transformation matrix
     * 
     * @param pTransform Pointer to receive the transform
     */
    virtual void GetTransform(CLTTransform* pTransform) const;
    
    /**
     * @brief Set the object's transformation matrix
     * 
     * @param pTransform New transform
     */
    virtual void SetTransform(const CLTTransform* pTransform);
    
    /**
     * @brief Check if this object collides with another
     * 
     * @param pOther The other game object to check against
     * @param pInfo Optional pointer to receive collision information
     * @return true if the objects collide, false otherwise
     */
    virtual bool CheckCollision(CLTGameObject* pOther, CLTCollisionInfo* pInfo = nullptr);
    
    /**
     * @brief Get the object's visibility state
     * 
     * @return true if the object is visible, false otherwise
     */
    bool IsVisible() const;
    
    /**
     * @brief Set the object's visibility state
     * 
     * @param bVisible The new visibility state
     */
    void SetVisible(bool bVisible);
    
    /**
     * @brief Get the object's name
     * 
     * @return The object's name
     */
    const char* GetName() const;
    
    /**
     * @brief Set the object's name
     * 
     * @param pName The new name
     */
    void SetName(const char* pName);

protected:
    CLTTransform* m_pTransform;  ///< Object's transform in the world
    bool m_bVisible;             ///< Whether this object is visible
    std::string m_sName;         ///< Object's name
    uint32_t m_nFlags;           ///< Object flags
    
    // Animation and physics state would be here
};

#endif // _CLT_GAME_OBJECT_H_