#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
//#include <GLUT/GLUT.h>
#include <math.h>
#define _USE_MATH_DEFINES

#include <iostream>
#include <vector>
#include "parse.hpp"
#include "transform.hpp"
#include "light.hpp"
#include "object.hpp"
#include "camera.hpp"
#include "material.hpp"
#include "quaternions.hpp"
#include "Eigen/Dense"
#include "Eigen/Sparse"


using namespace std;
using namespace Eigen;

///////////////////////////////////////////////////////////////////////////////////////////////////

void init(void);
void reshape(int width, int height);
void display(void);

void init_lights();
void set_lights();
void draw_objects();

void init_quaternion(Quaternions &q);
void mouse_pressed(int button, int state, int x, int y);
void mouse_moved(int x, int y);
void key_pressed(unsigned char key, int x, int y);

///////////////////////////////////////////////////////////////////////////////////////////////////

void init_quaternion(Quaternions &q)
{
    
    q.s = 1;
    q.x = 0;
    q.y = 0;
    q.z = 0;
    
}
///////////////////////////////////////////////////////////////////////////////////////////////////

Parse my_parse;

int xres, yres;
float h; //time step h
int start_x, start_y, end_x, end_y;
float mouse_scale_x, mouse_scale_y;

Triple p_start, p_end;
Quaternions last_rotate_q, current_rotate_q;
Matrix4f last_rotation = Matrix4f::Identity();

bool is_pressed = false;
bool is_smoothed = false;

///////////////////////////////////////////////////////////////////////////////////////////////////

void init(void)
{
    init_quaternion(current_rotate_q);
    init_quaternion(last_rotate_q);
    
    glShadeModel(GL_SMOOTH);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    glEnable(GL_DEPTH_TEST);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    
    glMatrixMode(GL_PROJECTION);
    
    glLoadIdentity();
    
    glFrustum(my_parse.camera.left_param, my_parse.camera.right_param,
              my_parse.camera.bottom_param, my_parse.camera.top_param,
              my_parse.camera.near_param, my_parse.camera.far_param);
    
    glMatrixMode(GL_MODELVIEW);
    
    init_lights();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void reshape(int width, int height)
{
    
    height = (height == 0) ? 1 : height;
    width = (width == 0) ? 1 : width;
    
    mouse_scale_x = (float) (my_parse.camera.right_param - my_parse.camera.left_param) / (float) width;
    mouse_scale_y = (float) (my_parse.camera.top_param - my_parse.camera.bottom_param) / (float) height;
    
    glutPostRedisplay();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

//Converts given angle in degrees to radians.

float rad2deg(float angle)
{
    return angle * 180 / M_PI;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void display(void)
{
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    
    float angle;
    
    angle = rad2deg(my_parse.camera.cam_orientation_angle);
    
    glRotatef(-angle,
              my_parse.camera.cam_orientation_axis[0],
              my_parse.camera.cam_orientation_axis[1],
              my_parse.camera.cam_orientation_axis[2]);
    
    glTranslatef(-my_parse.camera.cam_position[0],
                 -my_parse.camera.cam_position[1],
                 -my_parse.camera.cam_position[2]);
    
    set_lights();
    
    draw_objects();
    
    glutSwapBuffers();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void init_lights()
{
    
    glEnable(GL_LIGHTING);
    
    int num_lights = my_parse.lights.size();
    
    for(int i = 0; i < num_lights; ++i)
    {
        
        int light_id = GL_LIGHT0 + i;
        
        glEnable(light_id);
        
        glLightfv(light_id, GL_AMBIENT, my_parse.lights[i].color);
        glLightfv(light_id, GL_DIFFUSE, my_parse.lights[i].color);
        glLightfv(light_id, GL_SPECULAR, my_parse.lights[i].color);
        
        glLightf(light_id, GL_QUADRATIC_ATTENUATION, my_parse.lights[i].attenuation_k);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void set_lights()
{
    int num_lights = my_parse.lights.size();
    
    for(int i = 0; i < num_lights; ++i)
    {
        int light_id = GL_LIGHT0 + i;
        
        glLightfv(light_id, GL_POSITION, my_parse.lights[i].position);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/* 'draw_objects' function:
 *
 * This function has OpenGL render our objects to the display screen. It
 */
void draw_objects()
{
    int num_objects = my_parse.objects.size();
    
    for(int i = 0; i < num_objects; ++i)
    {
        glPushMatrix();//push a modelview matrix on the top of the stack;
        
        {
            int num_transform_sets = my_parse.objects[i].transforms_sets.size();
            
            for(int j = num_transform_sets - 1; j >= 0; --j)
            {
                
                if (my_parse.objects[i].transforms_sets[j].type == 1) {
                    glTranslatef(my_parse.objects[i].transforms_sets[j].transformation[0],
                                 my_parse.objects[i].transforms_sets[j].transformation[1],
                                 my_parse.objects[i].transforms_sets[j].transformation[2]);
                }
                if (my_parse.objects[i].transforms_sets[j].type == 2) {
                    float angle;
                    angle = rad2deg(my_parse.objects[i].transforms_sets[j].rotation_angle);
                    glRotatef(angle,
                              my_parse.objects[i].transforms_sets[j].transformation[0],
                              my_parse.objects[i].transforms_sets[j].transformation[1],
                              my_parse.objects[i].transforms_sets[j].transformation[2]);
                }
                if (my_parse.objects[i].transforms_sets[j].type == 3) {
                    glScalef(my_parse.objects[i].transforms_sets[j].transformation[0],
                             my_parse.objects[i].transforms_sets[j].transformation[1],
                             my_parse.objects[i].transforms_sets[j].transformation[2]);
                }
                
            }
            
            //put the Arcball matrx here;
            last_rotation = qua2matx(product(current_rotate_q, last_rotate_q));
            glMultMatrixf(last_rotation.data());
            
            glMaterialfv(GL_FRONT, GL_AMBIENT, my_parse.objects[i].material.ambiant_param);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, my_parse.objects[i].material.diffuse_param);
            glMaterialfv(GL_FRONT, GL_SPECULAR, my_parse.objects[i].material.specular_param);
            glMaterialf(GL_FRONT, GL_SHININESS, my_parse.objects[i].material.shininess_param);
            
            glVertexPointer(3, GL_FLOAT, 0, &my_parse.objects[i].vertex_buffer[0]);
            glNormalPointer(GL_FLOAT, 0, &my_parse.objects[i].normal_buffer[0]);
            
            int buffer_size = my_parse.objects[i].vertex_buffer.size();
            
            glDrawArrays(GL_TRIANGLES, 0, buffer_size);
            
        }
        glPopMatrix();
    }
    
    glPopMatrix();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void mouse_pressed(int button, int state, int x, int y)
{
    
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        
        start_x = x;
        start_y = y;
        
        p_start = scr2sph(xres, yres, start_x, start_y);
        
        is_pressed = true;
        
    }else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP){
        
        last_rotate_q = product(current_rotate_q, last_rotate_q);
        init_quaternion(current_rotate_q);
        
        is_pressed = false;
        
    }
    
    
}

void mouse_moved(int x, int y)
{
    
    if(is_pressed)
    {
        end_x = x;
        end_y = y;
        p_end = scr2sph(xres, yres, end_x, end_y);
        
        current_rotate_q = rotate_quaternion(p_start, p_end);
        
        glutPostRedisplay();
        
    }
}

void key_pressed(unsigned char key, int x, int y)
{
    /* If 'q' is pressed, quit the program.
     */
    if(key == 'q')
    {
        exit(0);
    }else if(key == 'm')
    {
        for (int i = 0; i < my_parse.pure_objects.size(); i++) {
            
            smooth_mesh(my_parse.pure_objects[i], h);
            update_normal(my_parse.pure_objects[i]);
            my_parse.objects[i].update_buffers(my_parse.pure_objects[i]);
            
        }
        glutPostRedisplay();
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
    xres = atoi(argv[2]);
    yres = atoi(argv[3]);
    
    h = atof(argv[4]);
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(xres, yres);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Test");
    
    my_parse.parser(argv[1]);
    
    init();
    
    glutDisplayFunc(display);
    
    glutReshapeFunc(reshape);
    
    glutMouseFunc(mouse_pressed);
    
    glutMotionFunc(mouse_moved);
    
    glutKeyboardFunc(key_pressed);
    
    glutMainLoop();
}






