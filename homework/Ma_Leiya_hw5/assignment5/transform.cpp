//
//  transform.cpp
//  assignment1
//
//  Created by Leiya Ma on 10/13/15.
//  Copyright © 2015 Leiya Ma. All rights reserved.
//

#include "transform.hpp"

void Transforms::load_transform(const string &s)
{
    istringstream iss(s);
    string str;
    iss>>str;
    
    if (str=="t") {
        type = 1;
        iss>>transformation[0]>>transformation[1]>>transformation[2];
    }else if (str=="r") {
        type = 2;
        iss>>transformation[0]>>transformation[1]>>transformation[2];
        float n = sqrt(pow(transformation[0],2)+pow(transformation[1],2)+pow(transformation[2],2));
        transformation[0] /= n;
        transformation[1] /= n;
        transformation[2] /= n;
        iss>>rotation_angle;
    }else if (str=="s") {
        type = 3;
        iss>>transformation[0]>>transformation[1]>>transformation[2];
    }
}

void Tcross_product(const Triple &qa, const Triple &qb, Triple &q)
{
    q.x = qa.y * qb.z - qa.z * qb.y;
    q.y = qa.z * qb.x - qa.x * qb.z;
    q.z = qa.x * qb.y - qa.y * qb.x;
}

float Tdot_product(const Triple &qa, const Triple &qb)
{
    return qa.x * qb.x + qa.y * qb.y + qa.z * qb.z;
}


