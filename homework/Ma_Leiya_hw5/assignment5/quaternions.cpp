//
//  quaternions.cpp
//  assignment3
//
//  Created by Leiya Ma on 11/4/15.
//  Copyright © 2015 Leiya Ma. All rights reserved.
//

#include "quaternions.hpp"

void cross_product(const Quaternions &qa, const Quaternions &qb, float f[3])
{
    f[0] = qa.y * qb.z - qa.z * qb.y;
    f[1] = qa.z * qb.x - qa.x * qb.z;
    f[2] = qa.x * qb.y - qa.y * qb.x;
}

float dot_product(const Quaternions &qa, const Quaternions &qb)
{
    return qa.x * qb.x + qa.y * qb.y + qa.z * qb.z;
}

Quaternions product(const Quaternions &qa, const Quaternions &qb)
{
    Quaternions q;
    float cp[3];
    cross_product(qa, qb, cp);
    q.s = qa.s * qb.s - dot_product(qa, qb);
    q.x = qa.s * qb.x + qb.s * qa.x + cp[0];
    q.y = qa.s * qb.y + qb.s * qa.y + cp[1];
    q.z = qa.s * qb.z + qb.s * qa.z + cp[2];
    
    return q;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

// transform points in screen to NDC sphere, return a Triple.
Triple scr2sph(const int _xres, const int _yres, const int x, const int y)
{
    Triple p;
    p.x = - 1.0 * (x - _xres/2)/(_xres/2);
    p.y = 1.0 * (y - _yres/2)/(_yres/2);
    if ((p.x * p.x + p.y * p.y) <= 1) {
        p.z = sqrt(1 - p.x * p.x - p.y * p.y);
    }else{
        p.z = 0;
    }
    return p;
}


Quaternions rotate_quaternion(const Triple &p1, const Triple &p2)
{
    float angle;
    Triple unit_vector;
    Triple norm_p1, norm_p2;
    float norm1, norm2;
    
    norm1 = sqrt(p1.x * p1.x + p1.y * p1.y + p1.z * p1.z);
    norm2 = sqrt(p2.x * p2.x + p2.y * p2.y + p2.z * p2.z);
    
    norm_p1.x = p1.x/norm1;
    norm_p1.y = p1.y/norm1;
    norm_p1.z = p1.z/norm1;
    
    norm_p2.x = p2.x/norm2;
    norm_p2.y = p2.y/norm2;
    norm_p2.z = p2.z/norm2;
    
    angle = -acos(Tdot_product(norm_p1, norm_p2));
    Tcross_product(norm_p1, norm_p2, unit_vector);
    
    {
        float n = sqrt(pow(unit_vector.x,2)+pow(unit_vector.y,2)+pow(unit_vector.z,2));
        unit_vector.x /= n;
        unit_vector.y /= n;
        unit_vector.z /= n;

    }
    Quaternions q;
    q.s = cos(angle / 2);
    q.x = unit_vector.x * sin(angle / 2);
    q.y = unit_vector.y * sin(angle / 2);
    q.z = unit_vector.z * sin(angle / 2);
    
    return q;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Matrix4f qua2matx(const Quaternions &q)
{
    Matrix4f f;
    float qxqx = q.x * q.x;
    float qyqy = q.y * q.y;
    float qzqz = q.z * q.z;
    float qxqy = q.x * q.y;
    float qxqz = q.x * q.z;
    float qyqz = q.y * q.z;
    float qxqs = q.x * q.s;
    float qyqs = q.y * q.s;
    float qzqs = q.z * q.s;
    
    f(0,0) = 1 - 2 * qyqy - 2 * qzqz;
    f(0,1) = 2 * (qxqy - qzqs);
    f(0,2) = 2 * (qxqz + qyqs);
    f(0,3) = 0;
    
    f(1,0) = 2 * (qxqy + qzqs);
    f(1,1) = 1 - 2 * qxqx - 2 * qzqz;
    f(1,2) = 2 * (qyqz - qxqs);
    f(1,3) = 0;
    
    f(2,0) = 2 * (qxqz - qyqs);
    f(2,1) = 2 * (qyqz + qxqs);
    f(2,2) = 1 - 2 * qxqx - 2 * qyqy;
    f(2,3) = 0;
    
    f(3,0) = 0;
    f(3,1) = 0;
    f(3,2) = 0;
    f(3,3) = 1;
    
    return f;

}











