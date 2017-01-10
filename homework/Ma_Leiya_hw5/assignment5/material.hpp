//
//  material.hpp
//  assignment2
//
//  Created by Leiya Ma on 10/23/15.
//  Copyright © 2015 Leiya Ma. All rights reserved.
//

#ifndef material_hpp
#define material_hpp


#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

class Material
{
public:
    float ambiant_param[3];
    float diffuse_param[3];
    float specular_param[3];
    float shininess_param;
    
    void load_material(const string &s);
};
#endif /* material_hpp */