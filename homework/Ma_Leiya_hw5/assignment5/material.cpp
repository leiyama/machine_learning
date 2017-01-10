
//
//  material.cpp
//  assignment2
//
//  Created by Leiya Ma on 10/23/15.
//  Copyright © 2015 Leiya Ma. All rights reserved.
//

#include "material.hpp"
void Material::load_material(const string &s){
    istringstream iss(s);
    string str;
    float shin;
    iss>>str;
    
    if (str=="ambient") {
        iss>>ambiant_param[0]>>ambiant_param[1]>>ambiant_param[2];
    }else if (str=="diffuse"){
        iss>>diffuse_param[0]>>diffuse_param[1]>>diffuse_param[2];
    }else if (str=="specular"){
        iss>>specular_param[0]>>specular_param[1]>>specular_param[2];
    }else if (str=="shininess"){
        iss>>shin;
        shininess_param=shin;
    }
}