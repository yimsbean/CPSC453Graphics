#include "Camera.h"
#include <math.h>

float Camera::CameraLocation_X = 0;
float Camera::CameraLocation_Y = 0;
float Camera::CameraLocation_Z = 0;

float Camera::yaw = 0;
float Camera::pitch = 0;
float Camera::back = 0;

float Camera::roll = 0;

float Camera::FOV = (M_PI/4);