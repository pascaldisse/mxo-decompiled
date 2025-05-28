#ifndef _CLT_TRANSFORM_H_
#define _CLT_TRANSFORM_H_

#include "CLTVector.h"

/**
 * @brief Transformation matrix class
 * 
 * Represents a 4x4 transformation matrix for position, rotation, and scale.
 * Used for object placement and movement in the game world.
 */
class CLTTransform {
public:
    /**
     * @brief Default constructor (identity matrix)
     */
    CLTTransform();
    
    /**
     * @brief Constructor with position
     */
    CLTTransform(const CLTVector& position);
    
    /**
     * @brief Constructor with position and rotation
     */
    CLTTransform(const CLTVector& position, const CLTVector& rotation);
    
    /**
     * @brief Copy constructor
     */
    CLTTransform(const CLTTransform& other);
    
    /**
     * @brief Assignment operator
     */
    CLTTransform& operator=(const CLTTransform& other);
    
    /**
     * @brief Set to identity matrix
     */
    void Identity();
    
    /**
     * @brief Get the position component
     */
    CLTVector GetPosition() const;
    
    /**
     * @brief Set the position component
     */
    void SetPosition(const CLTVector& position);
    
    /**
     * @brief Get the rotation component (as Euler angles)
     */
    CLTVector GetRotation() const;
    
    /**
     * @brief Set the rotation component (from Euler angles)
     */
    void SetRotation(const CLTVector& rotation);
    
    /**
     * @brief Get the right vector (x-axis)
     */
    CLTVector GetRight() const;
    
    /**
     * @brief Get the up vector (y-axis)
     */
    CLTVector GetUp() const;
    
    /**
     * @brief Get the forward vector (z-axis)
     */
    CLTVector GetForward() const;
    
    /**
     * @brief Apply this transform to a point
     */
    CLTVector TransformPoint(const CLTVector& point) const;
    
    /**
     * @brief Apply this transform to a direction vector
     */
    CLTVector TransformDirection(const CLTVector& direction) const;
    
    /**
     * @brief Combine two transforms
     */
    CLTTransform Multiply(const CLTTransform& other) const;
    
    /**
     * @brief Create a translation matrix
     */
    static CLTTransform Translation(const CLTVector& position);
    
    /**
     * @brief Create a rotation matrix from Euler angles
     */
    static CLTTransform Rotation(const CLTVector& rotation);
    
    /**
     * @brief Create a scale matrix
     */
    static CLTTransform Scale(const CLTVector& scale);
    
    /**
     * @brief Create a look-at matrix
     */
    static CLTTransform LookAt(const CLTVector& eye, const CLTVector& target, const CLTVector& up);

private:
    float m_Matrix[4][4];  ///< The 4x4 transformation matrix
};

#endif // _CLT_TRANSFORM_H_