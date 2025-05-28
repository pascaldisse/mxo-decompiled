#include "../../include/CLTObject.h"
#include <cstring>

CLTObject::CLTObject()
    : CLTBaseClass()
    , m_nObjectID(0)
    , m_bActive(true)
{
    m_sClassName = "CLTObject";
    m_nClassGUID = 0x1001;  // Example GUID, actual value would be different
}

CLTObject::~CLTObject()
{
    // Clean up any allocated properties
    for (auto prop : m_vProperties)
    {
        if (prop)
        {
            delete prop;
        }
    }
    m_vProperties.clear();
}

bool CLTObject::Init(void* pInitParams)
{
    // Call the base class implementation
    if (!CLTBaseClass::Init(pInitParams))
        return false;
    
    // Default implementation does nothing else
    return true;
}

void CLTObject::Term()
{
    // First clean up our resources
    for (auto prop : m_vProperties)
    {
        if (prop)
        {
            delete prop;
        }
    }
    m_vProperties.clear();
    
    // Then call the base class implementation
    CLTBaseClass::Term();
}

const char* CLTObject::GetClassName() const
{
    return m_sClassName.c_str();
}

bool CLTObject::IsKindOf(const char* pClassName) const
{
    if (!pClassName)
        return false;
    
    // Check if this class name matches
    if (strcmp(GetClassName(), pClassName) == 0)
        return true;
    
    // Check if base class matches
    return CLTBaseClass::IsKindOf(pClassName);
}

uint32_t CLTObject::GetObjectID() const
{
    return m_nObjectID;
}

void CLTObject::SetObjectID(uint32_t nID)
{
    m_nObjectID = nID;
}

bool CLTObject::HandleMessage(void* pMessage, uint32_t nMessageSize)
{
    // Default implementation doesn't handle any messages
    return false;
}

bool CLTObject::IsActive() const
{
    return m_bActive;
}

void CLTObject::SetActive(bool bActive)
{
    m_bActive = bActive;
}

void CLTObject::Update(float fDeltaTime)
{
    // Default implementation does nothing
}