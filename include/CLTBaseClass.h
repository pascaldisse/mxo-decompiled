#ifndef _CLT_BASE_CLASS_H_
#define _CLT_BASE_CLASS_H_

#include <stdint.h>
#include <string>

/**
 * @brief Base class for all LithTech engine objects
 * 
 * This is the root class of the LithTech engine hierarchy.
 * All major engine components inherit from this base class.
 */
class CLTBaseClass {
public:
    /**
     * @brief Default constructor
     */
    CLTBaseClass();
    
    /**
     * @brief Virtual destructor
     */
    virtual ~CLTBaseClass();
    
    /**
     * @brief Initialize the object
     * 
     * @param pInitParams Initialization parameters
     * @return true if initialization succeeded, false otherwise
     */
    virtual bool Init(void* pInitParams = nullptr);
    
    /**
     * @brief Clean up resources used by the object
     */
    virtual void Term();
    
    /**
     * @brief Get the class name
     * 
     * @return The class name as a string
     */
    virtual const char* GetClassName() const;
    
    /**
     * @brief Get the class GUID
     * 
     * @return A unique identifier for this class
     */
    virtual uint32_t GetClassGUID() const;
    
    /**
     * @brief Check if this object is of the specified class or inherits from it
     * 
     * @param pClassName The class name to check against
     * @return true if this object is of the specified class or inherits from it
     */
    virtual bool IsKindOf(const char* pClassName) const;
    
    /**
     * @brief Get the object's reference count
     * 
     * @return The current reference count
     */
    uint32_t GetRefCount() const;
    
    /**
     * @brief Increment the reference count
     * 
     * @return The new reference count
     */
    uint32_t AddRef();
    
    /**
     * @brief Decrement the reference count
     * 
     * @return The new reference count
     */
    uint32_t Release();

protected:
    uint32_t m_nRefCount;    ///< Reference count for this object
    uint32_t m_nClassGUID;   ///< Unique identifier for this class
    std::string m_sClassName; ///< Class name
};

#endif // _CLT_BASE_CLASS_H_