#include "Vector3.h"
#include <iostream>


int main()
{
    Vector3 vec1(1.f, 2.f, 3.f);
    Vector3 vec2(4.f, 5.f, 6.f);
    Vector3 vec3 = vec1 + vec2; // 5, 7, 9?

    Vector3 vec4 = 2.f * vec1;

    std::cout << vec3.x << " " << vec3.y << " " << vec3.z << std::endl;
    std::cout << vec4.x << " " << vec4.y << " " << vec4.z << std::endl;
    std::cout << vec1.dot(vec2) << std::endl;
    std::cout << vec1.norm() << std::endl;

    return 0;
}


