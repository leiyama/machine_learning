//
//  light.cpp
//  assignment2
//
//  Created by Leiya Ma on 10/22/15.
//  Copyright © 2015 Leiya Ma. All rights reserved.
//

#include "light.hpp"

void Point_Light::load_light(const string &s){
    string lstring;
    istringstream iss(s);
    
    iss>>lstring;//light
    iss>>position[0]>>position[1]>>position[2];
    position[3] = 1;
    iss>>lstring;//,
    iss>>color[0]>>color[1]>>color[2];
    iss>>lstring;//,
    iss>>attenuation_k;
}