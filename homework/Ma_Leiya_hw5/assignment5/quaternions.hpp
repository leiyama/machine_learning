//
//  quaternions.hpp
//  assignment3
//
//  Created by Leiya Ma on 11/4/15.
//  Copyright © 2015 Leiya Ma. All rights reserved.
//

#ifndef quaternions_hpp
#define quaternions_hpp

#include <stdio.h>
#include "transform.hpp"
#include "Eigen/Dense"

using namespace std;
using namespace Eigen;

class Quaternions
{
public:
    float s;
    float x;
    float y;
    float z;
};

void cross_product(const Quaternions &qa, const Quaternions &qb, float f[3]);
float dot_product(const Quaternions &qa, const Quaternions &qb);
Quaternions product(const Quaternions &qa, const Quaternions &qb);

Triple scr2sph(const int _xres, const int _yres, const int x, const int y);
Quaternions rotate_quaternion(const Triple &p1, const Triple &p2);
Matrix4f qua2matx(const Quaternions &q);

#endif /* quaternions_hpp */
