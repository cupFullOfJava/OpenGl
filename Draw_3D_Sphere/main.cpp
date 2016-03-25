/*
Created by Jeremy Timothy Brown

Program can create four spheres in 3D.
Writes out Image to .sif file

Keyboard Commands: C --> change color of top right sphere
*/

#include "maths_funcs.h"
#include "gl_utils.h"
#include <GL/glew.h> 
#include <GLFW/glfw3.h> 
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define _USE_MATH_DEFINES
#include <math.h>

#define GL_LOG_FILE "gl.log"

int g_gl_width = 512;
int g_gl_height = 512;

GLFWwindow* g_window = NULL;

struct Color{
	float r;
	float g;
	float b;
};

struct Sphere{
	float x;
	float y;
	float z;
	float r;
	float kar;
	float kag;
	float kab;
	float kdr;
	float kdg;
	float kdb;
	float ksr;
	float ksg;
	float ksb;
};

struct Vector3{
	float x;
	float y;
	float z;
};

struct Light{
	float x;
	float y;
	float z;
	float ir;
	float ig;
	float ib;
};

int writeFile(GLubyte *buffer){
	
	FILE *ptr_myfile;
	ptr_myfile = fopen("codeResources/Spheres.sif","wb");
	
	GLubyte mb1 = 72;
	GLubyte mb2 = 19;
	fwrite(&mb1, sizeof(GLubyte), 1, ptr_myfile);
	fwrite(&mb2, sizeof(GLubyte), 1, ptr_myfile);
	
	GLubyte hiByteHeight=2;
	GLubyte loByteHeight=0;
	GLubyte hiByteWidth=2;
	GLubyte loByteWidth=0;

	fwrite(&hiByteWidth, sizeof(GLubyte), 1, ptr_myfile);
	fwrite(&loByteWidth, sizeof(GLubyte), 1, ptr_myfile);
	fwrite(&hiByteHeight, sizeof(GLubyte), 1, ptr_myfile);
	fwrite(&loByteHeight, sizeof(GLubyte), 1, ptr_myfile);
	
	fwrite(buffer, 3, g_gl_width*g_gl_height,  ptr_myfile);
	
	fclose(ptr_myfile);
	return 0;
}

float dot(struct Vector3 *a, struct Vector3 *b){
	return((a->x * b->x) + (a->y * b->y) + (a->z * b->z));
}

float magnitude(struct Vector3 *v){
	return sqrt((v->x * v->x) + (v->y * v->y) + (v->z * v->z));
}

void normalize (struct Vector3 *v){
	float length = magnitude(v);
	if(length == 0)
	{
		v->x = 0;
		v->y = 0;
		v->z = 0;
	}
	v->x = v->x /length;
	v->y = v->y /length;
	v->z = v->z /length;
}

void drawPixel(int x, int y, GLubyte r,GLubyte g, GLubyte b, GLubyte *buffer){
	
	*(buffer+(y*512*3)+(x*3)) = r;
	*(buffer+(y*512*3)+(x*3)+1) = g;
	*(buffer+(y*512*3)+(x*3)+2) = b;
	
}

struct Color hit(struct Vector3 d, struct Sphere sph){
	
	struct Sphere s = sph;
	struct Color color;
	struct Light l1;
	struct Light l2;
	
	l1.x = 0.0;
	l1.y = 4;
	l1.z = -5;
	l1.ir = .7;
	l1.ig = .7;
	l1.ib = .7;
	
	l2.x = 0.0;
	l2.y = -5;
	l2.z = -5;
	l2.ir = .7;
	l2.ig = .7;
	l2.ib = .7;
	
	struct Vector3 v, n, l, h;
	v.x = -(s.x);
	v.y = -(s.y);
	v.z = -(s.z);
	
	float a, b, c, t1, t2, t;
	a = dot(&d, &d);
	b = 2.0 * dot(&d, &v);
	c = dot(&v, &v) - (s.r * s.r);
	t1 = (-b + sqrt((b*b) - (4*a*c))) / (2*a);
	t2 = (-b - sqrt((b*b) - (4*a*c))) / (2*a);
	
	if (t1 < t2){
		t = t1;
	}
	else{
		t = t2;
	}
	
	if(t>0){
		n.x = (d.x * t) - s.x;
		n.y = (d.y * t) - s.y;
		n.z = (d.z * t) - s.z;
		
		l.x = l1.x -(d.x * t);
		l.y = l1.y -(d.y * t);
		l.z = l1.z -(d.z * t);
		
		normalize(&n);
		normalize(&l);
		float lambert = dot(&n, &l);
		if (lambert<0){
			lambert = 0;
		}
		
		h.x = l.x - d.x;
		h.y = l.y - d.y;
		h.z = l.z - d.z;
		normalize(&h);
		float specular = dot(&n, &h);
		if (specular <0){
			specular = 0;
		}
		float P = 100;
		specular = pow(specular, P);
		//printf("%f", specular);
		
		
		n.x = (d.x * t) - s.x;
		n.y = (d.y * t) - s.y;
		n.z = (d.z * t) - s.z;
		
		l.x = l2.x -(d.x * t);
		l.y = l2.y -(d.y * t);
		l.z = l2.z -(d.z * t);
		
		normalize(&n);
		normalize(&l);
		float lambert2 = dot(&n, &l);
		if (lambert2<0){
			lambert2 = 0;
		}
		
		h.x = l.x - d.x;
		h.y = l.y - d.y;
		h.z = l.z - d.z;
		normalize(&h);
		float specular2 = dot(&n, &h);
		if (specular2 <0){
			specular2 = 0;
		}
		
		specular2 = pow(specular2, P);
		
		color.r = s.kar + (l1.ir * s.kdr * lambert) + (l1.ir * s.ksr * specular);
		color.g = s.kag + (l1.ig * s.kdg * lambert) + (l1.ig * s.ksg * specular);
		color.b = s.kab + (l1.ib * s.kdb * lambert) + (l1.ib * s.kdb * specular);
		
		//add second light
		/*
		color.r += (l2.ir * s.kdr * lambert2) + (l2.ir * s.ksr * specular2);
		color.g += (l2.ig * s.kdg * lambert2) + (l2.ig * s.ksg * specular2);
		color.b += (l2.ib * s.kdb * lambert2) + (l2.ib * s.kdb * specular2);
		*/
		
		if (color.r >1){
			color.r = 1;
		}
		if (color.g >1){
			color.g = 1;
		}
		if (color.b >1){
			color.b = 1;
		}
	}
	else
	{
		color.r = 0.0;
		color.g = 0.0;
		color.b = 0.0;
	}
	
	return color;
}

GLubyte * rayCast (int x, int y, float r, float g, float b) {
	int bpp = 3 * sizeof(GLubyte);
	GLubyte *buffer = (unsigned char*) malloc(x * y * bpp); 
	
	int red, green, blue;
	struct Color c;
	float xcoord, ycoord, zcoord;
	struct Vector3 ray;
	float width= float(x);
	float height = float(y);
	float vfov = 45;
	float side = -2.0 + tan(vfov/2.0);
	float zn = -1.0;
	
	struct Sphere sph[4];
	
	struct Sphere s1;
	s1.x = -2;
	s1.y = 2;
	s1.z = -9;
	s1.r = 1.5;
	s1.kar = 0.05;
	s1.kag = 0.25;
	s1.kab = 0.05;
	s1.kdr = 0.0;
	s1.kdg = 0.5;
	s1.kdb = 0.0;
	s1.ksr = 1.0;
	s1.ksg = 3.0;
	s1.ksb = 1.0;
	
	struct Sphere s2;
	s2.x = 2;
	s2.y = 2;
	s2.z = -8;
	s2.r = 1.5;
	s2.kar = 0.25;
	s2.kag = 0.05;
	s2.kab = 0.05;
	s2.kdr = r;
	s2.kdg = g;
	s2.kdb = b;
	s2.ksr = 5.0;
	s2.ksg = 1.0;
	s2.ksb = 1.0;
	
	struct Sphere s3;
	s3.x = -2;
	s3.y = -2;
	s3.z = -8;
	s3.r = 1.5;
	s3.kar = 0.05;
	s3.kag = 0.05;
	s3.kab = 0.25;
	s3.kdr = 0.0;
	s3.kdg = 0.0;
	s3.kdb = 1.0;
	s3.ksr = 1.0;
	s3.ksg = 1.0;
	s3.ksb = 5.0;
	
	struct Sphere s4;
	s4.x = 2;
	s4.y = -2;
	s4.z = -6;
	s4.r = 1.5;
	s4.kar = 0.05;
	s4.kag = 0.05;
	s4.kab = 0.25;
	s4.kdr = 1.0;
	s4.kdg = 0.0;
	s4.kdb = 1.0;
	s4.ksr = 1.0;
	s4.ksg = 1.0;
	s4.ksb = 1.0;
	
	sph[0] = s1;
	sph[1] = s2;
	sph[2] = s3;
	sph[3] = s4;
	
	for(int j = 0; j < y; j++){
		for(int i = 0; i < x; i++){
			xcoord = zn * ((float(i)/width) - 0.5) * side;
			ycoord = zn * -((float(j)/height) - 0.5) * side;
			zcoord = zn;
			
			ray.x = xcoord - 0;
			ray.y = ycoord - 0;
			ray.z = zcoord - 0;
			normalize(&ray);
			
			c = hit(ray, sph[0]);
			if(c.r == 0 && c.g == 0 && c.b == 0){
				c = hit(ray, sph[1]);
			}
			if(c.r == 0 && c.g == 0 && c.b == 0){
				c = hit(ray, sph[2]);
			}
			if(c.r == 0 && c.g == 0 && c.b == 0){
				c = hit(ray, sph[3]);
			}
			
			red = (int)(c.r *255);
			green = (int)(c.g *255);
			blue = (int)(c.b * 255);
			
			drawPixel(i, j, red, green, blue, buffer);
		}
	}
	
	writeFile(buffer);
	
	return buffer;
}

GLubyte * clearScreen(int x, int y){
	int bpp = 3 * sizeof(GLubyte);
	
	GLubyte *readBuffer = (unsigned char*) malloc(x*y*bpp);
	
	for(int n=0; n<x*y*3; n+=3)
	{
		*(readBuffer+n) = 50;
		*(readBuffer+n+1) = 50;
		*(readBuffer+n+2) = 50;
	}
	
	return readBuffer;
}

int main () {
	assert (restart_gl_log ());  // Keeping a log file for useful debug messages, etc. 
	assert (start_gl ());

	GLfloat points[] = {
		-1.0f, -1.0f,  0.0f,
		1.0f, -1.0f,  0.0f,
		1.0f,  1.0f,  0.0f,
		1.0f,  1.0f,  0.0f,
		-1.0f,  1.0f,  0.0f,
		-1.0f, -1.0f,  0.0f
	};

	GLfloat texcoords[] = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f
	};

	GLuint points_vbo;
	glGenBuffers (1, &points_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
	glBufferData (GL_ARRAY_BUFFER, 18 * sizeof (GLfloat), points, GL_STATIC_DRAW);
	GLuint texcoords_vbo;
	glGenBuffers (1, &texcoords_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, texcoords_vbo);
	glBufferData (GL_ARRAY_BUFFER, 12 * sizeof (GLfloat), texcoords, GL_STATIC_DRAW);
	GLuint vao;
	glGenVertexArrays (1, &vao);
	glBindVertexArray (vao);
	glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer (GL_ARRAY_BUFFER, texcoords_vbo);
	glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 0, NULL); // normalize!
	glEnableVertexAttribArray (0);
	glEnableVertexAttribArray (1);
	GLuint shader_program = create_programme_from_files ("test_vs.glsl", "test_fs.glsl");
	#define ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0 // 0.017444444

	GLfloat proj_mat[] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, -1.0f,
		0.0f, 0.0f, 1.0f, 0.0f
	};
	
	float cam_pos[] = {0.0f, 0.0f, 1.0f};   // look down Z-axis 
	float cam_yaw = 0.0f; //y-rotation in degrees
	mat4 T = translate (identity_mat4 (), vec3 (-cam_pos[0], -cam_pos[1], -cam_pos[2]));
	mat4 R = rotate_y_deg (identity_mat4 (), -cam_yaw);
	mat4 view_mat = R * T; 

	int view_mat_location = glGetUniformLocation (shader_program, "view");
	glUseProgram (shader_program);
	glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view_mat.m);
	int proj_mat_location = glGetUniformLocation (shader_program, "proj");
	glUseProgram (shader_program);
	glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, proj_mat);
	
	// load buffer as texture 
	GLuint tex;
	GLubyte *bufferBytes;
	
	float r = 1.0;
	float g = 0.0;
	float b = 0.0; 
	
	jump:
	
	bufferBytes = clearScreen(g_gl_width, g_gl_height);
	bufferBytes = rayCast(g_gl_width, g_gl_height, r, g, b);
	
	glGenTextures (1, &tex);
	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, tex);
	
	glTexImage2D (
		GL_TEXTURE_2D,
		0,
		GL_RGB,
		g_gl_width,
		g_gl_height,
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		bufferBytes 
	);
	
	glGenerateMipmap (GL_TEXTURE_2D);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	GLfloat max_aniso = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_aniso);
	
	while (!glfwWindowShouldClose (g_window)) {
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport (0, 0, g_gl_width, g_gl_height);
		glUseProgram (shader_program);
		glBindVertexArray (vao);
		glDrawArrays (GL_TRIANGLES, 0, 6);

		glfwPollEvents ();
		
		// control keys
		if (glfwGetKey (g_window, GLFW_KEY_C)) {
			printf("C-key pressed.\n");
			r = 0.0;
			g = 0.0;
			b = 1.0;
			goto jump;
		}
		if (GLFW_PRESS == glfwGetKey (g_window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose (g_window, 1);
		}

		glfwSwapBuffers (g_window);
	}
	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}