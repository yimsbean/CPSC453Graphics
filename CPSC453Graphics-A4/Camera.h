/*
* Data.cpp
*
*  Created on: Nov 14, 2018
*      Author: SeungBin, Yim
*/

#ifndef CAMERA_H
#define CAMERA_H

class Camera
{
public:
    static float CameraLocation_X;
    static float CameraLocation_Y;
    static float CameraLocation_Z;

    static float yaw;
    static float pitch; 
    static float back;

    static float roll; // Probably not going to use it

    static float FOV;
};
#endif