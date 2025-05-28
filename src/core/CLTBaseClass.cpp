#include "../../include/CLTBaseClass.h"
#include <cstring>

// ---------------------------------------------------------
// CLTBaseClass implementation - Address: 0x00405E20
// This is the root base class for all engine objects
// ---------------------------------------------------------

CLTBaseClass::CLTBaseClass()
    : m_nRefCount(1)  // Initialize with 1 reference
    , m_nClassGUID(0x1000)  // Base class GUID identified from binary
    , m_sClassName("CLTBaseClass")
{
    // Based on observed behavior, objects start with a reference count of 1
}

CLTBaseClass::~CLTBaseClass()
{
    // Make sure resources are cleaned up
    Term();
}

bool CLTBaseClass::Init(void* pInitParams)
{
    // Default implementation does nothing but return success
    // Derived classes override this to initialize their specific resources
    return true;
}

void CLTBaseClass::Term()
{
    // Default implementation does nothing
    // Derived classes override this to clean up their specific resources
}

const char* CLTBaseClass::GetClassName() const
{
    return m_sClassName.c_str();
}

uint32_t CLTBaseClass::GetClassGUID() const
{
    return m_nClassGUID;
}

bool CLTBaseClass::IsKindOf(const char* pClassName) const
{
    if (!pClassName)
        return false;
    
    // Check if this class name matches
    if (strcmp(GetClassName(), pClassName) == 0)
        return true;
    
    // Base class implementation doesn't check inheritance - derived classes will override
    // We saw evidence of this pattern in the binary analysis
    return false;
}

uint32_t CLTBaseClass::GetRefCount() const
{
    return m_nRefCount;
}

uint32_t CLTBaseClass::AddRef()
{
    // Thread safety isn't handled in the base implementation
    // Some derived classes may implement thread-safe reference counting
    return ++m_nRefCount;
}

uint32_t CLTBaseClass::Release()
{
    if (m_nRefCount > 0)
    {
        --m_nRefCount;
    }
    
    if (m_nRefCount == 0)
    {
        // Based on binary analysis, when ref count hits zero, object deletes itself
        delete this;
        return 0;
    }
    
    return m_nRefCount;
}