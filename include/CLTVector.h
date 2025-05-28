#ifndef _CLT_VECTOR_H_
#define _CLT_VECTOR_H_

#include <cmath>

/**
 * @brief 3D Vector class
 * 
 * Represents a 3D vector with x, y, and z components.
 * Used throughout the engine for positions, directions, etc.
 */
class CLTVector {
public:
    /**
     * @brief Default constructor (zero vector)
     */
    CLTVector() : x(0.0f), y(0.0f), z(0.0f) {}
    
    /**
     * @brief Constructor with initial values
     */
    CLTVector(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    
    /**
     * @brief Copy constructor
     */
    CLTVector(const CLTVector& other) : x(other.x), y(other.y), z(other.z) {}
    
    /**
     * @brief Assignment operator
     */
    CLTVector& operator=(const CLTVector& other) {
        if (this != &other) {
            x = other.x;
            y = other.y;
            z = other.z;
        }
        return *this;
    }
    
    /**
     * @brief Addition operator
     */
    CLTVector operator+(const CLTVector& other) const {
        return CLTVector(x + other.x, y + other.y, z + other.z);
    }
    
    /**
     * @brief Subtraction operator
     */
    CLTVector operator-(const CLTVector& other) const {
        return CLTVector(x - other.x, y - other.y, z - other.z);
    }
    
    /**
     * @brief Scalar multiplication
     */
    CLTVector operator*(float scalar) const {
        return CLTVector(x * scalar, y * scalar, z * scalar);
    }
    
    /**
     * @brief Scalar division
     */
    CLTVector operator/(float scalar) const {
        float invScalar = 1.0f / scalar;
        return CLTVector(x * invScalar, y * invScalar, z * invScalar);
    }
    
    /**
     * @brief In-place addition
     */
    CLTVector& operator+=(const CLTVector& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    
    /**
     * @brief In-place subtraction
     */
    CLTVector& operator-=(const CLTVector& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    
    /**
     * @brief In-place scalar multiplication
     */
    CLTVector& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }
    
    /**
     * @brief In-place scalar division
     */
    CLTVector& operator/=(float scalar) {
        float invScalar = 1.0f / scalar;
        x *= invScalar;
        y *= invScalar;
        z *= invScalar;
        return *this;
    }
    
    /**
     * @brief Equality comparison
     */
    bool operator==(const CLTVector& other) const {
        return (x == other.x && y == other.y && z == other.z);
    }
    
    /**
     * @brief Inequality comparison
     */
    bool operator!=(const CLTVector& other) const {
        return !(*this == other);
    }
    
    /**
     * @brief Calculate the dot product with another vector
     */
    float Dot(const CLTVector& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
    
    /**
     * @brief Calculate the cross product with another vector
     */
    CLTVector Cross(const CLTVector& other) const {
        return CLTVector(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }
    
    /**
     * @brief Calculate the length (magnitude) of the vector
     */
    float Length() const {
        return std::sqrt(x * x + y * y + z * z);
    }
    
    /**
     * @brief Calculate the squared length of the vector
     * (faster than Length() when only comparing distances)
     */
    float LengthSquared() const {
        return x * x + y * y + z * z;
    }
    
    /**
     * @brief Normalize the vector (make it unit length)
     * @return Reference to this vector after normalization
     */
    CLTVector& Normalize() {
        float length = Length();
        if (length > 0.0f) {
            float invLength = 1.0f / length;
            x *= invLength;
            y *= invLength;
            z *= invLength;
        }
        return *this;
    }
    
    /**
     * @brief Get a normalized copy of this vector
     */
    CLTVector GetNormalized() const {
        CLTVector result(*this);
        result.Normalize();
        return result;
    }
    
    /**
     * @brief Calculate the distance to another point
     */
    float Distance(const CLTVector& other) const {
        return (*this - other).Length();
    }
    
    /**
     * @brief Calculate the squared distance to another point
     */
    float DistanceSquared(const CLTVector& other) const {
        return (*this - other).LengthSquared();
    }
    
    /**
     * @brief Set the vector components
     */
    void Set(float _x, float _y, float _z) {
        x = _x;
        y = _y;
        z = _z;
    }
    
    /**
     * @brief Reset to zero vector
     */
    void Zero() {
        x = y = z = 0.0f;
    }
    
    float x, y, z;  ///< The vector components
};

#endif // _CLT_VECTOR_H_