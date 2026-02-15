#include <cmath>
#include "Vector3.h"

namespace ModalSynth
{
Vector3::Vector3(float x, float y, float z) : x{x}, y{y}, z{z}
{
}

float Vector3::dot(const Vector3& other) const
{
    return x * other.x + y * other.y + z * other.z;
}

float Vector3::norm() const
{
    return sqrtf(x * x + y * y + z * z);
}

Vector3 Vector3::operator+(const Vector3& other) const
{
    return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator-(const Vector3& other) const
{
    return Vector3(x - other.x, y - other.y, z - other.z);
}

Vector3 Vector3::operator*(float scalar) const
{
    return Vector3(x * scalar, y * scalar, z * scalar);
} 

Vector3 operator*(float scalar, const Vector3& vec)
{
    return vec * scalar;
}
};
