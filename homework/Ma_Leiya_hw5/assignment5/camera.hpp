//
//  camera.hpp
//  assignment
//
//  Created by Leiya Ma on 10/12/15.
//  Copyright © 2015 Leiya Ma. All rights reserved.
//

#ifndef camera_hpp
#define camera_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

class Camera
{
public:
    float cam_position[3];
    float cam_orientation_axis[3];
    float cam_orientation_angle;
    
    float near_param, far_param,
          left_param, right_param,
          top_param, bottom_param;
    
    void load_camera(const string &s);
    
};





#endif /* camera_hpp */
