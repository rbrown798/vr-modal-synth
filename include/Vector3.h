#pragma once

namespace ModalSynth
{
struct Vector3
{
    float x{};
    float y{};
    float z{};

public:
    Vector3(float x=0, float y=0, float z=0);
    float dot(const Vector3& other) const;
    float norm() const;
    Vector3 operator+(const Vector3& other) const;
    Vector3 operator-(const Vector3& other) const;
    Vector3 operator*(float scalar) const;
};

Vector3 operator*(float scalar, const Vector3& vec);
};

