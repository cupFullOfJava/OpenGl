//Based originally on "Anton's OpenGL 4 Tutorials" and code provided by Dr. Eric Patterson                            |
//drawLine, drawCircle, readFile, drawBuffer, drawPixel functions by Jeremy Timothy Brown

//This program can load in and display a .sif file, draw lines and circles to the screen, and can write images out to a .sif file.

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

//used for .sif image processing
GLubyte * readFile() {
	
	int imageWidth, imageHeight;
	GLubyte mb1, mb2, hiByteWidth, loByteWidth, hiByteHeight, loByteHeight;
	FILE *ptr_myfile;
	
	//need to uncomment when writing for avg gray or green weighted grey
	/*
	FILE *wrt_f;
	*/
	
	ptr_myfile=fopen("codeResources/lines.sif","rb");
	
	//write file for avg grey
	/*
	wrt_f = fopen("codeResources/avgGrey.sif", "wb+");
	*/
	
	//write file for green weight grey
	/*
	wrt_f = fopen("codeResources/greenWeightGrey.sif", "wb+");
	*/
	
	fread(&mb1, 1, 1, ptr_myfile);
	fread(&mb2, 1, 1, ptr_myfile);
	fread(&hiByteWidth,1, 1, ptr_myfile);
	fread(&loByteWidth, 1, 1, ptr_myfile);
	fread(&hiByteHeight, 1, 1, ptr_myfile);
	fread(&loByteHeight, 1, 1, ptr_myfile);
	imageWidth = (hiByteWidth*256)+loByteWidth;
	imageHeight = (hiByteHeight*256)+loByteHeight;

	//write data to sif file
	//need to uncomment when writing for avg gray or green weighted grey
	/*
	fwrite(&mb1, 1, 1, wrt_f);
	fwrite(&mb2, 1, 1, wrt_f);
	fwrite(&hiByteWidth,1, 1, wrt_f);
	fwrite(&loByteWidth, 1, 1, wrt_f);
	fwrite(&hiByteHeight, 1, 1, wrt_f);
	fwrite(&loByteHeight, 1, 1, wrt_f);
	*/
	
	int bpp = 3 * sizeof(GLubyte);
	GLubyte *readBuffer = (unsigned char*) malloc(imageHeight*imageWidth*bpp);
	fread(readBuffer, bpp, imageHeight*imageWidth, ptr_myfile);
	fclose(ptr_myfile);
	
	//close writer
	//need to uncomment when writing for avg gray or green weighted grey
	/*
	fclose(wrt_f);
	*/
	
	return readBuffer;
}

//Function with malloc to reserve buffer, pointers to access buffer bytes
//Used for image processing
GLubyte * drawBuffer (int x, int y) {
	
	
	//need to write to file
	//need to uncomment when writing for avg gray or green weighted grey
	/*
	FILE *apd_f;
	GLubyte r,g,b;
	*/
	
	int bpp = 3 * sizeof(GLubyte);
	GLubyte *buffer =(unsigned char*) malloc(x * y * bpp); 
	GLubyte *dataPtr;
	dataPtr = readFile();
	
	//write avg grey
	/*
	apd_f = fopen("codeResources/greenWeightGrey.sif", "ab");
	*/
	
	//write green weight grey
	/*
	apd_f = fopen("codeResources/greenWeightGrey.sif", "ab");
	*/
	

	//regular image	read
	for (int n=0; n<x*y*3; n+=3)
	{
		
		*(buffer+n) = (*(dataPtr+n));
		*(buffer+n+1) = (*(dataPtr+n+1));
		*(buffer+n+2) = (*(dataPtr+n+2));

	}
	
	
	
	//Grayscale conversion.  Average weighted
	/*
	for (int n=0; n<x*y*3; n+=3)
	{
		*(buffer+n) = ((*(dataPtr+n))+(*(dataPtr+n+1))+(*(dataPtr+n+2)))/3;
		*(buffer+n+1) = ((*(dataPtr+n))+(*(dataPtr+n+1))+(*(dataPtr+n+2)))/3;
		*(buffer+n+2) = ((*(dataPtr+n))+(*(dataPtr+n+1))+(*(dataPtr+n+2)))/3;
		
		r = *(buffer+n);
		g = *(buffer+n+1);
		b = *(buffer+n+2);
		
		fwrite(&r, 1, 1, apd_f);
		fwrite(&g, 1, 1, apd_f);
		fwrite(&b, 1, 1, apd_f);

	}*/
	
	
	//Grayscale conversion.  Green weighted
	/*for (int n=0; n<x*y*3; n+=3)
	{
		*(buffer+n) = (*(dataPtr+n))*0.2126+(*(dataPtr+n+1))*0.7152+(*(dataPtr+n+2))*0.0722;
		*(buffer+n+1) = (*(dataPtr+n))*0.2126+(*(dataPtr+n+1))*0.7152+(*(dataPtr+n+2))*0.0722;
		*(buffer+n+2) = (*(dataPtr+n))*0.2126+(*(dataPtr+n+1))*0.7152+(*(dataPtr+n+2))*0.0722;
		
		r = *(buffer+n);
		g = *(buffer+n+1);
		b = *(buffer+n+2);
		
		fwrite(&r, 1, 1, apd_f);
		fwrite(&g, 1, 1, apd_f);
		fwrite(&b, 1, 1, apd_f);
	}
	*/
	
	//close writer
	//fclose(apd_f);
	
	return buffer;
}

GLubyte * drawPixel(GLubyte * buffer, int x, int y, int r, int g, int b){
	
	//need to write to file
	FILE *apd_f;
	apd_f = fopen("codeResources/circles.sif", "ab");
	
	int bpp = 4 * sizeof(GLubyte);
	*(buffer+((y*g_gl_height)*bpp)+(x*bpp)) = r;
	*(buffer+((y*g_gl_height)*bpp)+(x*bpp)+1) = g;
	*(buffer+((y*g_gl_height)*bpp)+(x*bpp)+2) = b;
	
	
	GLubyte red, green, blue;
	
	red = *(buffer+((y*g_gl_height)*bpp)+(x*bpp));
	green = *(buffer+((y*g_gl_height)*bpp)+(x*bpp)+1);
	blue = *(buffer+((y*g_gl_height)*bpp)+(x*bpp)+2);
	
	fwrite(&red, 1, 1, apd_f);
	fwrite(&green, 1, 1, apd_f);
	fwrite(&blue, 1, 1, apd_f);
	
	//close writer
	fclose(apd_f);
	
	return buffer;
}

GLubyte * drawLine(GLubyte * buffer, int x0, int y0, int x1, int y1, int r, int g, int b){
	
	int dx, dy, xs, ys, eps, y, x;
	
	dx = x1 - x0;
	dy = y1 - y0;
	
	if(dy > 0){
		ys = 1;
	}
	else{
		ys = -1;
		dy = -dy;
	}

	if (dx > 0){
		xs = 1;
	}
	else{
		xs = -1;
		dx = -dx;
	}
	
	
	eps = 0;
	if(dx > dy){
		y = y0;
		for(x=x0; x!=x1; x+=xs){
			drawPixel(buffer,x,y,r,g,b);
			eps +=dy;
			if((eps << 1) >= dx){
				y += ys;
				eps -= dx;
			}
		}
	}
	else{
		x = x0;
		for(y=y0; y!=y1; y+=ys){
			drawPixel(buffer,x,y,r,g,b);
			eps += dx;
			if((eps << 1) >= dy){
				x += xs;
				eps -= dy;
			}
		}
	}
	
	return buffer;
}

//needed to draw lines and circles to the screen
GLubyte * drawBlack(int x, int y){
	int bpp = 4 * sizeof(GLubyte);
	
	GLubyte *readBuffer = (unsigned char*) malloc(x*y*bpp);
	
	
	for(int n=0; n<x*y*4; n+=3)
	{
		
		*(readBuffer+n) = 50;
		*(readBuffer+n+1) = 50;
		*(readBuffer+n+2) = 50;

	}
	return readBuffer;
}

//draw circle...duh
void drawCircle(GLubyte * buffer, int x0, int y0, int rad, int r, int g, int b){
	int x = 0;
	int y = rad;
	int p = 1 - r;
	
	while(x <= y){
		drawPixel(buffer, x0+x,y0-y,r,g,b);
		drawPixel(buffer, x0+y,y0+x,r,g,b);
		drawPixel(buffer, x0-x,y0+y,r,g,b);
		drawPixel(buffer, x0-y,y0+x,r,g,b);
		drawPixel(buffer, x0-x,y0-y,r,g,b);
		drawPixel(buffer, x0-y,y0-x,r,g,b);
		drawPixel(buffer, x0+x,y0+y,r,g,b);
		drawPixel(buffer, x0+y,y0-x,r,g,b);
	
		if(p<0){
			p=p+2*x+3;
		}
		else{
			p=p+2*(x-y)+5;
			y--;
		}
	x++;
	}
}

int main () {
	assert (restart_gl_log ());  // Keeping a log file for useful debug messages, etc. 
	assert (start_gl ());

//Two triangles and texture coordinates to make a textured square.
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
	glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 0, NULL); // normalise!
	glEnableVertexAttribArray (0);
	glEnableVertexAttribArray (1);
	GLuint shader_program = create_programme_from_files (
	"test_vs.glsl", "test_fs.glsl");
	#define ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0 // 0.017444444

	GLfloat proj_mat[] = {
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, -1.0f,
	0.0f, 0.0f, 1.0f, 0.0f
	};
	float cam_pos[] = {0.0f, 0.0f, 1.0f};   // look down Z-axis 
	float cam_yaw = 0.0f; // y-rotation in degrees
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
			
	//for drawing image
	/*
	bufferBytes = drawBuffer(g_gl_width, g_gl_height);
	*/
	
	//for drawing lines
	bufferBytes = drawBlack(g_gl_width, g_gl_height);
	
	glGenTextures (1, &tex);
	glActiveTexture (GL_TEXTURE0);
	glBindTexture (GL_TEXTURE_2D, tex);
	
	//used for image processing
	/*
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
	*/
	
	//draw lines
	/*
	FILE *wrt_f;
	wrt_f = fopen("codeResources/lines.sif", "wb+");
	
	drawLine(bufferBytes, 256, 256, 512, 256, 255, 255, 255);
	drawLine(bufferBytes, 256, 256, 256, 0, 255, 255, 255);
	drawLine(bufferBytes, 256, 256, 0, 256, 255, 255, 255);
	drawLine(bufferBytes, 256, 256, 256, 512, 255, 255, 255);
	drawLine(bufferBytes, 256, 256, 512, 512, 255, 255, 255);
	drawLine(bufferBytes, 256, 256, 0, 0, 255, 255, 255);
	drawLine(bufferBytes, 256, 256, 512, 0, 255, 255, 255);
	drawLine(bufferBytes, 256, 256, 0, 512, 255, 255, 255);
	
	glTexImage2D (
	GL_TEXTURE_2D,
	0,
	GL_RGBA,
	g_gl_width,
	g_gl_height,
	0,
	GL_RGBA,
	GL_UNSIGNED_BYTE,
		   bufferBytes 
	);
	fclose(wrt_f);
	*/
	//end draw lines
	
	//draw circle
	FILE *wrt_f;
	wrt_f = fopen("codeResources/circles.sif", "wb+");
	
	
	drawCircle(bufferBytes, 256, 256, 256, 255, 255, 255);
	drawCircle(bufferBytes, 257, 257, 246, 245, 255, 255);
	drawCircle(bufferBytes, 258, 258, 236, 235, 255, 255);
	drawCircle(bufferBytes, 259, 259, 226, 225, 255, 255);
	drawCircle(bufferBytes, 260, 260, 216, 215, 255, 255);
	drawCircle(bufferBytes, 261, 261, 206, 205, 255, 255);
	drawCircle(bufferBytes, 262, 262, 196, 195, 255, 255);
	drawCircle(bufferBytes, 263, 263, 186, 185, 255, 255);
	
	glTexImage2D (
	GL_TEXTURE_2D,
	0,
	GL_RGBA,
	g_gl_width,
	g_gl_height,
	0,
	GL_RGBA,
	GL_UNSIGNED_BYTE,
		   bufferBytes 
	);
	fclose(wrt_f);
	//end draw circle
	
	
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
		}
		if (GLFW_PRESS == glfwGetKey (g_window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose (g_window, 1);
		}

		// put the stuff we've been drawing onto the display
		glfwSwapBuffers (g_window);
	}
	
	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}

