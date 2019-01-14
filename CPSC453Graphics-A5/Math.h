// This is reused from my Assignment 4
#ifndef MATH_H_
#define MATH_H_

#include <glm/vec3.hpp>

class Math {
    public:
        static glm::vec3 getDirection(glm::vec3 start, glm::vec3 end);
        static float getMagnitude(glm::vec3 vec);
        static glm::vec3 getCrossProduct(glm::vec3 a, glm::vec3 b);
        static float getDotProduct(glm::vec3 a, glm::vec3 b);
        static float getDiscriminant(float a, float b, float c);
        static float max(float a, float b);
        static float min(float a, float b);
        static glm::vec3 normalize(glm::vec3 vec);
        static float degreesToRadians(float degrees);
        glm::vec3 sphericalToCartesianCoorinates(float radius, float theta, float phi);
};

#endif // MATH_H_
