#ifndef _CLT_CHARACTER_H_
#define _CLT_CHARACTER_H_

#include "CLTGameObject.h"
#include <string>
#include <vector>

// Forward declarations
class CLTAnimation;
class CLTModel;
class CLTEffect;

/**
 * @brief Base class for character objects
 * 
 * CLTCharacter is the base class for all character entities in the game,
 * including players and NPCs. It extends CLTGameObject with character-specific
 * functionality like animations, stats, and AI.
 */
class CLTCharacter : public CLTGameObject {
public:
    /**
     * @brief Default constructor
     */
    CLTCharacter();
    
    /**
     * @brief Virtual destructor
     */
    virtual ~CLTCharacter();
    
    /**
     * @brief Initialize the character
     * 
     * @param pInitParams Initialization parameters
     * @return true if initialization succeeded, false otherwise
     */
    virtual bool Init(void* pInitParams = nullptr) override;
    
    /**
     * @brief Clean up resources used by the character
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
     * @brief Process update for this character
     * 
     * @param fDeltaTime Time in seconds since the last update
     */
    virtual void Update(float fDeltaTime) override;
    
    /**
     * @brief Set the character model
     * 
     * @param pModel The model to use
     * @return true if successful, false otherwise
     */
    virtual bool SetModel(CLTModel* pModel);
    
    /**
     * @brief Get the character model
     * 
     * @return Pointer to the character's model
     */
    virtual CLTModel* GetModel() const;
    
    /**
     * @brief Play an animation
     * 
     * @param pAnimation The animation to play
     * @param fBlendTime Blend time in seconds
     * @param bLoop Whether to loop the animation
     * @return true if successful, false otherwise
     */
    virtual bool PlayAnimation(CLTAnimation* pAnimation, float fBlendTime = 0.3f, bool bLoop = false);
    
    /**
     * @brief Stop the current animation
     * 
     * @param fBlendTime Blend time in seconds
     */
    virtual void StopAnimation(float fBlendTime = 0.3f);
    
    /**
     * @brief Move the character to a position
     * 
     * @param pPosition The target position
     * @param fSpeed Movement speed
     * @return true if movement started successfully, false otherwise
     */
    virtual bool MoveTo(const CLTVector* pPosition, float fSpeed);
    
    /**
     * @brief Rotate the character to face a direction
     * 
     * @param pDirection The target direction
     * @param fSpeed Rotation speed
     * @return true if rotation started successfully, false otherwise
     */
    virtual bool RotateTo(const CLTVector* pDirection, float fSpeed);
    
    /**
     * @brief Check if the character is moving
     * 
     * @return true if the character is moving, false otherwise
     */
    bool IsMoving() const;
    
    /**
     * @brief Check if the character is alive
     * 
     * @return true if the character is alive, false otherwise
     */
    bool IsAlive() const;
    
    /**
     * @brief Set the character's alive state
     * 
     * @param bAlive The new alive state
     */
    void SetAlive(bool bAlive);
    
    /**
     * @brief Get the character's health
     * 
     * @return The current health value
     */
    float GetHealth() const;
    
    /**
     * @brief Set the character's health
     * 
     * @param fHealth The new health value
     */
    void SetHealth(float fHealth);
    
    /**
     * @brief Get the character's maximum health
     * 
     * @return The maximum health value
     */
    float GetMaxHealth() const;
    
    /**
     * @brief Set the character's maximum health
     * 
     * @param fMaxHealth The new maximum health value
     */
    void SetMaxHealth(float fMaxHealth);
    
    /**
     * @brief Apply damage to the character
     * 
     * @param fAmount Amount of damage to apply
     * @param pSource Optional source of the damage
     * @return Actual amount of damage applied
     */
    virtual float ApplyDamage(float fAmount, CLTGameObject* pSource = nullptr);
    
    /**
     * @brief Apply healing to the character
     * 
     * @param fAmount Amount of healing to apply
     * @param pSource Optional source of the healing
     * @return Actual amount of healing applied
     */
    virtual float ApplyHealing(float fAmount, CLTGameObject* pSource = nullptr);
    
    /**
     * @brief Add an effect to the character
     * 
     * @param pEffect The effect to add
     * @return true if successful, false otherwise
     */
    virtual bool AddEffect(CLTEffect* pEffect);
    
    /**
     * @brief Remove an effect from the character
     * 
     * @param pEffect The effect to remove
     * @return true if successful, false otherwise
     */
    virtual bool RemoveEffect(CLTEffect* pEffect);

protected:
    CLTModel* m_pModel;                   ///< Character model
    CLTAnimation* m_pCurrentAnimation;    ///< Current animation playing
    float m_fHealth;                      ///< Current health
    float m_fMaxHealth;                   ///< Maximum health
    bool m_bAlive;                        ///< Whether the character is alive
    bool m_bMoving;                       ///< Whether the character is moving
    CLTVector m_vMoveTarget;              ///< Target position for movement
    float m_fMoveSpeed;                   ///< Current movement speed
    std::vector<CLTEffect*> m_vEffects;   ///< Active effects on the character
    
    // Stats and other character-specific data would be here
};

#endif // _CLT_CHARACTER_H_