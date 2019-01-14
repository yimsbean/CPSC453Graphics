#include "Camera.h"

float Camera::PHI_DEFAULT = Math::degreesToRadians(30);
float Camera::THETA_DEFAULT = Math::degreesToRadians(-75);

int Camera::MIN_ZOOM = 7;

float Camera::θ;
float Camera::φ;
float Camera::zNear;
float Camera::zFar;
float Camera::fieldOfView;
float Camera::aspectRatio;
float Camera::zoom;

glm::vec3 Camera::baseLocation;
glm::vec3 Camera::modifiedLocation;
glm::vec3 Camera::direction;
glm::vec3 Camera::up;
