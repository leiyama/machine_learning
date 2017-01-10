//
//  object.cpp
//  assignment
//
//  Created by Leiya Ma on 10/12/15.
//  Copyright © 2015 Leiya Ma. All rights reserved.
//

#include "object.hpp"

void Object::load_objfile(const string &s){
    //string cstring;
    string objfilename;
    istringstream iss(s);
    iss>>obj_name;//save“cube”
    iss>>objfilename;//save“cube.obj”
    objfile(objfilename);
}

void Object::objfile(const string &objfilename){
    ifstream objfile(objfilename);
    string objstring;
    string instring;
    Triple objvector;
    Face temp_f;
    while (getline(objfile, objstring)) {
        
        //if objfile not found, cout "error: objfile not found"
        
        istringstream in(objstring);
        in>>instring;
        if (instring=="v") {
            in >> objvector.x >> objvector.y >> objvector.z;
            vertice.push_back(objvector);
        }else if (instring=="f"){
            in >> temp_f.idx1>> temp_f.idx2>> temp_f.idx3;
            face.push_back(temp_f);
        }
    }
    Build_Mesh_Data(*this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void Build_Mesh_Data(Object &obj)
{
    Mesh_Data mesh_data;
    vector < Vertex* > vertices;
    vector < Face* > faces;
    
    mesh_data.vertices = &vertices;
    mesh_data.faces = &faces;
    
    vertices.clear();
    faces.clear();
    //push back first ont "NULL"
    vertices.push_back(NULL);
    
    for (int i = 0; i < obj.vertice.size(); i++) {
        Vertex *pv = new Vertex;
        pv->x = (double)obj.vertice[i].x;
        pv->y = (double)obj.vertice[i].y;
        pv->z = (double)obj.vertice[i].z;
        vertices.push_back(pv);
    }
    
    for (int i = 0; i < obj.face.size(); i++) {
        
        Face *pf = new Face;
        pf->idx1 = obj.face[i].idx1;
        pf->idx2 = obj.face[i].idx2;
        pf->idx3 = obj.face[i].idx3;
        faces.push_back(pf);
    }
    obj.hevs = new vector<HEV *>;
    obj.hefs = new vector<HEF *>;
    build_HE(&mesh_data, obj.hevs, obj.hefs);
    for (int i = 1; i < vertices.size(); i++) {
        if (vertices[i] != NULL) {
            delete vertices[i];
        }
    }
    for (int i = 1; i < faces.size(); i++) {
        if (faces[i] != NULL) {
            delete faces[i];
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

// smooth mesh_data
void smooth_mesh(Object &obj, const float h)
{
    solve(obj.hevs, h);
}

// function to assign each vertex in our mesh to an index
void index_vertices( vector< HEV* > *hevs )
{
    for( int i = 1; i < hevs->size(); ++i ) // start at 1 because obj files are 1-indexed
        hevs->at(i)->index = i;
}

// function to construct our F operator in matrix form
SparseMatrix<double> build_F_operator( vector< HEV* > *hevs, const float h)
{
    index_vertices( hevs );
    
    int num_vertices = hevs->size() - 1;
    int max_index = 0;
    
    SparseMatrix<double> F( num_vertices, num_vertices );
    
    for (int i = 1; i < hevs->size(); i++) {
        HE *he = hevs->at(i)->out;
        int num_index = 0;
        
        do
        {
            num_index += 1;
            he = he->flip->next;
        }
        while ( he != hevs->at(i)->out );
        
        if (max_index < num_index) max_index = num_index;
    }
    
    F.reserve( Eigen::VectorXi::Constant( num_vertices, max_index + 1) );

    for( int i = 1; i < hevs->size(); ++i )
    {
        HE *he = hevs->at(i)->out;
        double A = 0;
        
        do
        {
            // compute the normal of the plane of the face
            Vec3f face_normal = calc_normal(he->face);
            // compute the area of the triangular face
            A += calc_area(face_normal);
            
            he = he->flip->next;

        }
        while( he != hevs->at(i)->out );
        
        F.insert(i -1 , i -1) = 1;
        
        if (A < 1e-8) {
            
        }else
        {
            do // iterate over all vertices adjacent to v_i
            {
                int j = he->next->vertex->index; // get index of adjacent vertex to v_i
                
                // call function to compute cot_alpha_j + cot_belta_j;
                double cot_j = h * compute_cot(he->vertex, he->next->vertex, he) / (2 * A);
                
                // fill the j-th slot of row i of our F matrix with appropriate value
                F.coeffRef(i - 1, i - 1) -= -cot_j;
                F.insert( i - 1, j - 1 ) = -cot_j;
                
                he = he->flip->next;
            }
            while( he != hevs->at(i)->out );
        }
    }
    F.makeCompressed(); // optional; tells Eigen to more efficiently store our sparse matrix
    return F;
}

// function to solve Equation 4
void solve( vector< HEV* > *hevs, const float h)
{

    SparseMatrix<double> F = build_F_operator( hevs, h);
    
    SparseLU<Eigen::SparseMatrix<double>, Eigen::COLAMDOrdering<int> > solver;
    
    solver.analyzePattern( F );
    solver.factorize( F );
    
    int num_vertices = hevs->size() - 1;
    
    // initialize our vector representation of x0, y0, z0
    VectorXd x0_vector (num_vertices), y0_vecter (num_vertices), z0_vector(num_vertices);
    for( int i = 1; i < hevs->size(); ++i )
    {
        x0_vector(i - 1) = hevs->at(i)->x;
        y0_vecter(i - 1) = hevs->at(i)->y;
        z0_vector(i - 1) = hevs->at(i)->z;
    }
    
    // have Eigen solve for our xh, yh, zh;
    VectorXd xh_vector (num_vertices), yh_vecter (num_vertices), zh_vector(num_vertices);
    xh_vector = solver.solve( x0_vector );
    yh_vecter = solver.solve( y0_vecter );
    zh_vector = solver.solve( z0_vector );

    // update the smoothed vertex to original vertex
    for (int i = 1; i<xh_vector.size(); i++) {
        hevs->at(i)->x = xh_vector(i - 1);
        hevs->at(i)->y = yh_vecter(i - 1);
        hevs->at(i)->z = zh_vector(i - 1);
    }
}

double compute_cot(HEV *vi, HEV *vj, HE *he)
{
    double cot_a, cot_b;
    HEV *va = he->flip->next->next->vertex;
    HEV *vb = he->next->next->vertex;
    
    Vertex vv1, vv2;
    
    vv1.x = vi->x - va->x;
    vv1.y = vi->y - va->y;
    vv1.z = vi->z - va->z;
    
    vv2.x = vj->x - va->x;
    vv2.y = vj->y - va->y;
    vv2.z = vj->z - va->z;
    
    Vec3f cross_normal_a = Vcross_product(vv1, vv2);
    
    cot_a = Vdot_product(vv1,vv2)/(2 * calc_area(cross_normal_a));
    
    Vertex vv3, vv4;
    
    vv3.x = vi->x - vb->x;
    vv3.y = vi->y - vb->y;
    vv3.z = vi->z - vb->z;
    
    vv4.x = vj->x - vb->x;
    vv4.y = vj->y - vb->y;
    vv4.z = vj->z - vb->z;
    
    Vec3f cross_normal_b = Vcross_product(vv3, vv4);

    cot_b = Vdot_product(vv3, vv4)/(2 * calc_area(cross_normal_b));
    
    return cot_a + cot_b;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void update_normal(Object &obj)
{
        for (int i = 1; i < obj.hevs->size() ; i++) {
        
        Vec3f surface_normal;
        Triple triple_normal;
        
        surface_normal = calc_vertex_normal(obj.hevs->at(i));
        
        obj.hevs->at(i)->normal = surface_normal;
    }
}

Vec3f calc_vertex_normal(HEV *vertex)
{
    
    // initialize Vec3f structure of the surface normal for current vertex
    Vec3f normal;
    normal.x = 0;
    normal.y = 0;
    normal.z = 0;
    
    HE *he = vertex->out; // get outgoing halfedge from given vertex
    
    do
    {
        // compute the normal of the plane of the face
        Vec3f face_normal = calc_normal(he->face);
        // compute the area of the triangular face
        double face_area = calc_area(face_normal);
        
        face_normal = normalize(face_normal);
        
        // accummulate onto our normal vector
        normal.x += face_normal.x * face_area;
        normal.y += face_normal.y * face_area;
        normal.z += face_normal.z * face_area;
        
        // gives us the halfedge to the next adjacent vertex
        he = he->flip->next;
    }
    while(he != vertex->out);
    
    // normalize n and return it
    
    normal = normalize(normal);
    
    return normal;
};

Vec3f normalize(const Vec3f &vn)
{
    Vec3f norm_vn = vn;
    double n = sqrt(vn.x * vn.x + vn.y * vn.y + vn.z * vn.z);
    norm_vn.x /= n;
    norm_vn.y /= n;
    norm_vn.z /= n;
    
    return norm_vn;
}

Vec3f calc_normal(HEF *face)
{
    
    Vec3f face_normal;
    
    HEV *v1 = face->edge->vertex;
    HEV *v2 = face->edge->next->vertex;
    HEV *v3 = face->edge->next->next->vertex;
    
    Vertex vva,vvb;
    
    vva.x = v2->x - v1->x;
    vva.y = v2->y - v1->y;
    vva.z = v2->z - v1->z;
    
    vvb.x = v3->x - v1->x;
    vvb.y = v3->y - v1->y;
    vvb.z = v3->z - v1->z;

    face_normal = Vcross_product(vva, vvb);
    
    return face_normal;
    
}

Vec3f Vcross_product(const Vertex &va, const Vertex &vb)
{
    
    Vec3f face_normal;
    
    face_normal.x = va.y * vb.z - va.z * vb.y;
    face_normal.y = va.z * vb.x - va.x * vb.z;
    face_normal.z = va.x * vb.y - va.y * vb.x;
    
    return face_normal;
}

double Vdot_product(const Vertex &va, const Vertex &vb)
{
    return va.x * vb.x + va.y * vb.y + va.z * vb.z;
}

double calc_area(Vec3f &normal)
{
    
    double area;
    
    area = 0.5 * sqrt(normal.x * normal.x + normal.y *normal.y + normal.z *normal.z);
    
    return area;

}
