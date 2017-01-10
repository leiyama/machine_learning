//
//  camera.cpp
//  assignment
//
//  Created by Leiya Ma on 10/12/15.
//  Copyright © 2015 Leiya Ma. All rights reserved.
//

#include "camera.hpp"

void Camera::load_camera(const string &s){
    string cstring;
    istringstream iss(s);
    iss>>cstring;
    if (cstring=="position") {
        iss>>cam_position[0]>>cam_position[1]>>cam_position[2];
    }
    if (cstring=="orientation") {
        iss>>cam_orientation_axis[0]>>cam_orientation_axis[1]>>cam_orientation_axis[2];
        iss>>cam_orientation_angle;
    }
    if (cstring=="near") {
        iss>>near_param;
    }
    if (cstring=="far") {
        iss>>far_param;
    }
    if (cstring=="left") {
        iss>>left_param;
    }
    if (cstring=="right") {
        iss>>right_param;
    }
    if (cstring=="top") {
        iss>>top_param;
    }
    if (cstring=="bottom") {
        iss>>bottom_param;
    }
}



