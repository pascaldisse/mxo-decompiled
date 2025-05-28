#include "../../include/gameplay/CLTGameObject.h"
#include "../../include/CLTTransform.h"
#include "../../include/CLTVector.h"
#include <cstring>

// Forward declaration of collision info
class CLTCollisionInfo {};

CLTGameObject::CLTGameObject()
    : CLTObject()
    , m_pTransform(nullptr)
    , m_bVisible(true)
    , m_sName("")
    , m_nFlags(0)
{
    m_sClassName = "CLTGameObject";
    m_nClassGUID = 0x2001;  // Example GUID, actual value would be different
    
    // Create a default transform (identity)
    m_pTransform = new CLTTransform();
}

CLTGameObject::~CLTGameObject()
{
    // Clean up the transform
    if (m_pTransform)
    {
        delete m_pTransform;
        m_pTransform = nullptr;
    }
}

bool CLTGameObject::Init(void* pInitParams)
{
    // Call the base class implementation
    if (!CLTObject::Init(pInitParams))
        return false;
    
    // Initialize our transform if it doesn't exist
    if (!m_pTransform)
    {
        m_pTransform = new CLTTransform();
    }
    
    return true;
}

void CLTGameObject::Term()
{
    // Clean up our resources
    if (m_pTransform)
    {
        delete m_pTransform;
        m_pTransform = nullptr;
    }
    
    // Then call the base class implementation
    CLTObject::Term();
}

const char* CLTGameObject::GetClassName() const
{
    return m_sClassName.c_str();
}

bool CLTGameObject::IsKindOf(const char* pClassName) const
{
    if (!pClassName)
        return false;
    
    // Check if this class name matches
    if (strcmp(GetClassName(), pClassName) == 0)
        return true;
    
    // Check if base class matches
    return CLTObject::IsKindOf(pClassName);
}

void CLTGameObject::Update(float fDeltaTime)
{
    // Default implementation does nothing specific
    // but would typically update physics, animation, etc.
    
    // Call base class implementation
    CLTObject::Update(fDeltaTime);
}

void CLTGameObject::GetPosition(CLTVector* pPos) const
{
    if (pPos && m_pTransform)
    {
        *pPos = m_pTransform->GetPosition();
    }
}

void CLTGameObject::SetPosition(const CLTVector* pPos)
{
    if (pPos && m_pTransform)
    {
        m_pTransform->SetPosition(*pPos);
    }
}

void CLTGameObject::GetRotation(CLTVector* pRot) const
{
    if (pRot && m_pTransform)
    {
        *pRot = m_pTransform->GetRotation();
    }
}

void CLTGameObject::SetRotation(const CLTVector* pRot)
{
    if (pRot && m_pTransform)
    {
        m_pTransform->SetRotation(*pRot);
    }
}

void CLTGameObject::GetTransform(CLTTransform* pTransform) const
{
    if (pTransform && m_pTransform)
    {
        *pTransform = *m_pTransform;
    }
}

void CLTGameObject::SetTransform(const CLTTransform* pTransform)
{
    if (pTransform && m_pTransform)
    {
        *m_pTransform = *pTransform;
    }
}

bool CLTGameObject::CheckCollision(CLTGameObject* pOther, CLTCollisionInfo* pInfo)
{
    // Default implementation - would normally do collision detection
    // between this object and the other
    
    // For now just return false
    return false;
}

bool CLTGameObject::IsVisible() const
{
    return m_bVisible;
}

void CLTGameObject::SetVisible(bool bVisible)
{
    m_bVisible = bVisible;
}

const char* CLTGameObject::GetName() const
{
    return m_sName.c_str();
}

void CLTGameObject::SetName(const char* pName)
{
    if (pName)
    {
        m_sName = pName;
    }
}