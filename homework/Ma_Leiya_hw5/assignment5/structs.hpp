//
//  structs.hpp
//  assignment5
//
//  Created by Leiya Ma on 11/18/15.
//  Copyright © 2015 Leiya Ma. All rights reserved.
//

#ifndef structs_hpp
#define structs_hpp

#include <stdio.h>
#include <vector>

struct Vec3f
{
    double x, y, z;
};

struct Vertex
{
    int index;
    double x, y, z;
};

struct Face
{
    int idx1, idx2, idx3;
};

struct Mesh_Data
{
    std::vector<Vertex*> *vertices;
    std::vector<Face*> *faces;
};

#endif /* structs_hpp */
