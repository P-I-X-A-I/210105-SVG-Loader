#pragma once

#include "OpenGL_Manager_Class.h"
#include "GUI_Manager_Class.h"
#include "Window_Manager_Class.h"
#include "matrixClass.h"

#include "SVG_Loader_Class.h"

class mainController
{

public:

	OpenGL_Manager_Class* gl_obj;
	GUI_Manager_Class* gui_manager_obj;
	Window_Manager_Class* window_manager_obj;
	matrixClass* matrix_obj;

	SVG_Loader_Class* svg_loader_obj;

	HWND base_hWnd;

	// opengl var
	GLuint sampler_Name;

	GLuint VAO_name;
	GLuint VBO_name; // vert

	GLuint VS_OBJ;
	GLuint FS_OBJ;
	GLuint PRG_OBJ;
	GLint UNF_mvpMatrix;

	// vert
	int NUM_VERT = 0;
	GLfloat linesVert[10000][4];
	GLfloat actVert[10000][4];
	GLfloat tailVert[5][10000][4];
	GLfloat brake[10000];
	GLfloat velocity[10000][2];

	mainController();
	~mainController();


	void setup_openGL();
	void setup_sampler();
	void setup_shader();
	void setup_VAO();
	void setup_FBO();

	void animation_loop();
	void makeItRandom();
	void draw();
};

