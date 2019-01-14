// This is reused from my Assignment 4
#include "Math.h"

#include <glm/vec3.hpp>

#include <cmath>

#define _USE_MATH_DEFINES
#include <math.h>

/**
 * Get the normalized direction vector between 2 points.
 * Used for finding the direction of rays from camera to pixel.
 * from point of intersection to light source for shadow rays, and reflective
 * rays.
 */
glm::vec3 Math::getDirection(glm::vec3 start, glm::vec3 end) {
    return normalize(end - start);
}

float Math::getMagnitude(glm::vec3 vec) {
    return sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2));
}

glm::vec3 Math::getCrossProduct(glm::vec3 a, glm::vec3 b) {
    return glm::vec3(a.y * b.z - a.z * b.y,
                     a.z * b.x - a.x * b.z,
                     a.x * b.y - a.y * b.x);
}

float Math::getDotProduct(glm::vec3 a, glm::vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float Math::getDiscriminant(float a, float b, float c) {
    return pow(b, 2.0f) - 4.0f*a*c;
}

float Math::max(float a, float b) {
    return a > b ? a : b;
}

float Math::min(float a, float b) {
    return a < b ? a : b;
}

glm::vec3 Math::normalize(glm::vec3 vec) {
    return vec /= Math::getMagnitude(vec);
}

float Math::degreesToRadians(float degrees) {
    return degrees * M_PI/180.0f;
}

/**
 * @param radius
 * @param theta altitude degrees
 * @param radius azimuth degrees
 */
glm::vec3 Math::sphericalToCartesianCoorinates(float radius, float theta, float phi) {
	float thetaRadians = degreesToRadians(theta);
	float phiRadians = degreesToRadians(phi);
	float x = radius * sin(phiRadians) * sin(thetaRadians);
	float y = radius * cos(thetaRadians);
	float z = radius * cos(phiRadians) * sin(thetaRadians);
	return glm::vec3(x, y, z);
}

