#include "../../include/CLTBaseClass.h"
#include <cstring>

CLTBaseClass::CLTBaseClass()
    : m_nRefCount(0)
    , m_nClassGUID(0)
    , m_sClassName("CLTBaseClass")
{
}

CLTBaseClass::~CLTBaseClass()
{
}

bool CLTBaseClass::Init(void* pInitParams)
{
    // Default implementation does nothing
    return true;
}

void CLTBaseClass::Term()
{
    // Default implementation does nothing
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
    return false;
}

uint32_t CLTBaseClass::GetRefCount() const
{
    return m_nRefCount;
}

uint32_t CLTBaseClass::AddRef()
{
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
        delete this;
        return 0;
    }
    
    return m_nRefCount;
}