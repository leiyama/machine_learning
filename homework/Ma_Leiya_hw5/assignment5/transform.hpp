//
//  transform.hpp
//  assignment1
//
//  Created by Leiya Ma on 10/13/15.
//  Copyright © 2015 Leiya Ma. All rights reserved.
//

#ifndef transform_hpp
#define transform_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

using namespace std;

struct Triple
{
    float x;
    float y;
    float z;
};

class Transforms
{
public:
    int type;// this type will differentiate the type of transform: translation, rotation or scaling;
    float transformation[3];
    float rotation_angle;
    
    void load_transform(const string &s);
};

void Tcross_product(const Triple &qa, const Triple &qb, Triple &q);
float Tdot_product(const Triple &qa, const Triple &qb);

#endif /* transform_hpp */
