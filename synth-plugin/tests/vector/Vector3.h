#pragma once

struct Vector3
{
    float x{};
    float y{};
    float z{};

public:
    Vector3(float x, float y, float z);
    float dot(const Vector3& other) const;
    float norm() const;
    Vector3 operator+(const Vector3& other) const;
    Vector3 operator-(const Vector3& other) const;
    Vector3 operator*(float scalar) const;
};

Vector3 operator*(float scalar, const Vector3& vec);

