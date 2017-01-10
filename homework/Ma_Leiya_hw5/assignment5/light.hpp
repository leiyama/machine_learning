//
//  light.hpp
//  assignment2
//
//  Created by Leiya Ma on 10/22/15.
//  Copyright © 2015 Leiya Ma. All rights reserved.
//

#ifndef light_hpp
#define light_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "transform.hpp"
#include "material.hpp"

using namespace std;

class Point_Light
{
public:
    float position[4];
    float color[3];
    float attenuation_k;
    void load_light(const string &s);
};

#endif /* light_hpp */
