//
//  parse.hpp
//  assignment2
//
//  Created by Leiya Ma on 11/01/15.
//  Copyright © 2015 Leiya Ma. All rights reserved.
//

#ifndef parse_hpp
#define parse_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "transform.hpp"
#include "light.hpp"
#include "object.hpp"
#include "camera.hpp"
#include "material.hpp"

using namespace std;

struct NewObject
{
    string newobj_name;
    vector<Triple> vertex_buffer;
    vector<Triple> normal_buffer;
    vector<Transforms> transforms_sets;
    Material material;
    
    void update_buffers(const Object &obj);
};

class Parse
{
public:
    
    Camera camera;
    vector<Point_Light> lights;
    vector<NewObject> objects;
    vector<Object> pure_objects;

    void parser(const char * s);
    
};

#endif /* parse_hpp */