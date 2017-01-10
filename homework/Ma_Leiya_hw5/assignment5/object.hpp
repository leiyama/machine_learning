//
//  object.hpp
//  assignment
//
//  Created by Leiya Ma on 10/12/15.
//  Copyright © 2015 Leiya Ma. All rights reserved.
//

#ifndef object_hpp
#define object_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "transform.hpp"
#include "halfedge.hpp"
#include "Eigen/Dense"
#include "Eigen/Sparse"

using namespace std;
using namespace Eigen;
class Object
{
public:
    string obj_name;
    vector < Triple > vertice;
    vector < Triple > vn;
    vector < Face > face;
    
    vector< HEV* > *hevs;
    vector< HEF* > *hefs;
    
    void load_objfile(const string &s);
    void objfile(const string &objfilename);
    
};

void Build_Mesh_Data(Object &obj);

void update_vertex(Object &obj);
void update_normal(Object &obj);
void smooth_mesh(Object &obj, const float h);

Vec3f calc_vertex_normal(HEV *vertex);
Vec3f calc_normal(HEF *face);
double calc_area(Vec3f &normal);
Vec3f Vcross_product(const Vertex &va, const Vertex &vb);
double Vdot_product(const Vertex &va, const Vertex &vb);
Vec3f normalize(const Vec3f &vn);

double compute_cot(HEV *vi, HEV *vj, HE *he);
void index_vertices( vector< HEV* > *hevs );
SparseMatrix<double> build_F_operator( vector< HEV* > *hevs, const float h);
void solve( vector< HEV* > *hevs, const float h );

#endif /* object_hpp */
