#ifndef _CLT_OBJECT_H_
#define _CLT_OBJECT_H_

#include "CLTBaseClass.h"
#include <vector>

/**
 * @brief Base class for all LithTech game objects
 * 
 * CLTObject is the base class for all game objects in the LithTech engine.
 * It extends CLTBaseClass with functionality for object identification,
 * properties, and message handling.
 */
class CLTObject : public CLTBaseClass {
public:
    /**
     * @brief Default constructor
     */
    CLTObject();
    
    /**
     * @brief Virtual destructor
     */
    virtual ~CLTObject();
    
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
     * @brief Get the object's unique ID
     * 
     * @return The object ID
     */
    uint32_t GetObjectID() const;
    
    /**
     * @brief Set the object's unique ID
     * 
     * @param nID The new object ID
     */
    void SetObjectID(uint32_t nID);
    
    /**
     * @brief Handle a message sent to this object
     * 
     * @param pMessage Pointer to the message data
     * @param nMessageSize Size of the message data in bytes
     * @return true if the message was handled, false otherwise
     */
    virtual bool HandleMessage(void* pMessage, uint32_t nMessageSize);
    
    /**
     * @brief Check if the object is active
     * 
     * @return true if the object is active, false otherwise
     */
    bool IsActive() const;
    
    /**
     * @brief Set the object's active state
     * 
     * @param bActive The new active state
     */
    void SetActive(bool bActive);
    
    /**
     * @brief Process update for this object
     * 
     * @param fDeltaTime Time in seconds since the last update
     */
    virtual void Update(float fDeltaTime);

protected:
    uint32_t m_nObjectID;      ///< Unique identifier for this object
    bool m_bActive;            ///< Whether this object is active
    std::vector<void*> m_vProperties; ///< Object properties
};

#endif // _CLT_OBJECT_H_