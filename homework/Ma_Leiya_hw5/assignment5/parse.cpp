//
//  parse.cpp
//  assignment3
//
//  Created by Leiya Ma on 11/2/15.
//  Copyright © 2015 Leiya Ma. All rights reserved.
//

#include "parse.hpp"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

Triple convert(HEV *p) {
    Triple rtn;
    rtn.x = p->x;
    rtn.y = p->y;
    rtn.z = p->z;
    return rtn;
}

Triple convert(const Vec3f &p){
    Triple rtn;
    rtn.x = p.x;
    rtn.y = p.y;
    rtn.z = p.z;
    return rtn;
}

void NewObject::update_buffers(const Object &obj)
{
    vertex_buffer.clear();
    normal_buffer.clear();
    for (int i=0; i<obj.face.size(); i++)
    {
        vertex_buffer.push_back(convert(obj.hevs->at(obj.face[i].idx1)));
        vertex_buffer.push_back(convert(obj.hevs->at(obj.face[i].idx2)));
        vertex_buffer.push_back(convert(obj.hevs->at(obj.face[i].idx3)));
        
        normal_buffer.push_back(convert(obj.hevs->at(obj.face[i].idx1)->normal));
        normal_buffer.push_back(convert(obj.hevs->at(obj.face[i].idx2)->normal));
        normal_buffer.push_back(convert(obj.hevs->at(obj.face[i].idx3)->normal));
        
    }
}

NewObject TranstoNewObject(const Object &obj, const vector<Transforms> &trans, const Material &matrl)
{
    NewObject myobject;
    myobject.newobj_name = obj.obj_name;
    myobject.update_buffers(obj);
    myobject.transforms_sets = trans;
    myobject.material = matrl;
    return myobject;
    
}

void Parse::parser(const char *s)
{
    ifstream txtfile(s);//read txt file name to ifstream txtfile;
    string txtstring;
    string txtinstring;
    
    Point_Light light;
    Material material;
    
    Transforms transforms;
    vector<Transforms> transforms_sets;
    
    Object object;
    vector<Object> objsave;

    //load camera
    while (getline(txtfile, txtstring)) {
        if (txtstring!="") {
            camera.load_camera(txtstring);
        }else{
            break;
        }
    }
    
    //load light
    while (getline(txtfile, txtstring)) {
        if (txtstring!="") {
            light.load_light(txtstring);
            lights.push_back(light);
        }else{
            break;
        }
    }
    
    //skip 'object:'
    while (getline(txtfile, txtstring)) {
        if (txtstring=="objects:") {
            break;
        }
    }
    
    //load and save object;
    while (getline(txtfile, txtstring)) {
        
        if (txtstring!="") {
            
            object.load_objfile(txtstring);
            
            // smooth and compute the surface normal per vertex
            // function in object.cpp
            update_normal(object);
            objsave.push_back(object);
            pure_objects.push_back(object);
            
        }else{
            break;
        }
    }
    
    string objnamecheck;
    Object objtobemodified;
    int state=0;
    
    //start loading parameter specified to each object
    while (getline(txtfile, txtstring)) {
        istringstream intxt(txtstring);
        istringstream intxts(txtstring);
        string check;
        intxts>>check;
        
        if (state==0) {
            intxt>>txtinstring;
            state=1;
            {
                transforms_sets.clear();
            }
            objnamecheck=txtinstring;
            for (int i=0; i<objsave.size();i++) {
                if (objsave[i].obj_name==objnamecheck) {
                    objtobemodified=objsave[i];
                }
            }
        }else if(state==1){
            intxt>>txtinstring;
            if (txtinstring=="ambient"||txtinstring=="diffuse"||txtinstring=="specular"){
                material.load_material(txtstring);
                state=1;
            }else if(txtinstring=="shininess"){
                material.load_material(txtstring);
                state=2;
            }
        }else if (state==2){
            if (txtstring != "") {
                transforms.load_transform(txtstring);
                transforms_sets.push_back(transforms);
                state=2;
            }else{
                state=0;
                objects.push_back(TranstoNewObject(objtobemodified, transforms_sets, material));
            }
        }
    }
    objects.push_back(TranstoNewObject(objtobemodified, transforms_sets, material));
    
}
