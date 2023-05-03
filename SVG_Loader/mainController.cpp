#include "framework.h"
#include "mainController.h"


mainController* this_obj;

VOID CALLBACK drawTimer(HWND hWnd, UINT msg, UINT idEvent, DWORD dwTime)
{
	this_obj->animation_loop();
	this_obj->draw();
}


mainController::mainController()
{
	srand(time(NULL));

	gl_obj = new OpenGL_Manager_Class();
	window_manager_obj = new Window_Manager_Class();
	window_manager_obj->setMainController(this);

	gui_manager_obj = new GUI_Manager_Class();
	matrix_obj = new matrixClass();


	svg_loader_obj = new SVG_Loader_Class();
	svg_loader_obj->isInterpolate = true;
	svg_loader_obj->CURVE_DIV = 8;
	svg_loader_obj->LINE_DIV = 10;
	//svg_loader_obj->load_svg_from_path("logo.svg");
	//svg_loader_obj->load_svg_from_path("sumo.svg");
	svg_loader_obj->load_svg_from_path("R2.svg");
	svg_loader_obj->scale_point_data(0.1);


	// copy vertex from SVG Loader
	NUM_VERT = svg_loader_obj->NUM_P;
	float* vPtr = svg_loader_obj->get_pVert_pointer();

	for (int i = 0; i < NUM_VERT; i++)
	{
		actVert[i][0] = linesVert[i][0] = *vPtr; vPtr++;
		actVert[i][1] = linesVert[i][1] = *vPtr; vPtr++;
		actVert[i][2] = linesVert[i][2] = *vPtr; vPtr++;
		actVert[i][3] = linesVert[i][3] = *vPtr; vPtr++;

		brake[i] = 0.98 + (rand() % 100)*0.01*0.01;
		velocity[i][0] = 0.0;
		velocity[i][1] = 0.0;

		for (int t = 0; t < 5; t++)
		{
			tailVert[t][i][0] = actVert[i][0];
			tailVert[t][i][1] = actVert[i][1];
			tailVert[t][i][2] = actVert[i][2];
			tailVert[t][i][3] = actVert[i][3];
		}
	}


	this_obj = this;

	//
	this->setup_openGL();
	this->setup_sampler();
	this->setup_shader();
	this->setup_VAO();



	// start timer
	SetTimer(NULL, 100, 31, (TIMERPROC)drawTimer);
	
}

mainController::~mainController()
{


}




void mainController::setup_openGL()
{
	// create dummy window
	HWND dummyWnd = window_manager_obj->createNewWindow(L"dummy", L"dummyTitle", 0);

	// create base window
	base_hWnd = window_manager_obj->create_panel_window(L"mainWindow", L"main window");
	window_manager_obj->set_window_position(base_hWnd, 100, 100);
	window_manager_obj->set_window_size(base_hWnd, 1200, 900);

	// init opengl
	gl_obj->init_OpenGL_functions(dummyWnd, base_hWnd);

	// set opengl status
	gl_obj->set_OpenGL_status();

	// close dummy window
	window_manager_obj->close_window(dummyWnd);
}


void mainController::setup_sampler()
{
	// generate sampler
	glGenSamplers(1, &sampler_Name);

	// setup sampler
	glSamplerParameteri(sampler_Name, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(sampler_Name, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(sampler_Name, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(sampler_Name, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// set sampler to all texture object
	GLint maxTex;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTex);

	for (int i = 0; i < maxTex; i++)
	{
		glBindSampler(i, sampler_Name);
	}
}



void mainController::setup_shader()
{
	gl_obj->loadShaderSource_And_CompileShader("normal_vs.txt", 0, &VS_OBJ);
	gl_obj->loadShaderSource_And_CompileShader("normal_fs.txt", 2, &FS_OBJ);
	gl_obj->createProgram_And_AttachShader(&PRG_OBJ, &VS_OBJ, NULL, &FS_OBJ);

	gl_obj->getUniformLocation(&PRG_OBJ, &UNF_mvpMatrix, "mvpMatrix");
}


void mainController::setup_VAO()
{
	// gen VAO
	glGenVertexArrays(1, &VAO_name);
	// gen Vertex Buffer
	glGenBuffers(1, &VBO_name);

	glBindVertexArray(VAO_name);
	glEnableVertexAttribArray(0); // vertex

	// alloc buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO_name);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 4, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

	// unbind 
	glBindVertexArray(0);
}


void mainController::setup_FBO()
{

}



void mainController::makeItRandom()
{
	for (int i = 0; i < NUM_VERT; i++)
	{
		velocity[i][0] += (rand() % 200 - 100)*0.01 * 0.001;
		velocity[i][1] += (rand() % 200 - 100)*0.01 * 0.001;
	}
}


void mainController::animation_loop()
{
	int ret = GetAsyncKeyState(VK_RIGHT);
	if (ret != 0)
	{
		// key pushed
		this->makeItRandom();
		printf("R");
	}



	for (int i = 0; i < NUM_VERT; i++)
	{
		// set tail
		for (int t = 4; t > 0; t--)
		{
			tailVert[t][i][0] = tailVert[t-1][i][0];
			tailVert[t][i][1] = tailVert[t-1][i][1];
		}


		// random velocity
		int R = rand() % 30000;
		if (R == 0 && i != NUM_VERT-1)
		{
			velocity[i][0] += (rand() % 200 - 100)*0.01*0.01;
			velocity[i][1] += (rand() % 200 - 100)*0.01*0.01;
			velocity[i+1][0] += (rand() % 200 - 100)*0.01*0.01;
			velocity[i+1][1] += (rand() % 200 - 100)*0.01*0.01;
		}

		double PF[2];

		PF[0] = (linesVert[i][0] - actVert[i][0])*0.002;
		PF[1] = (linesVert[i][1] - actVert[i][1])*0.002;

		velocity[i][0] = velocity[i][0] * brake[i] + PF[0];
		velocity[i][1] = velocity[i][1] * brake[i] + PF[1];

		// set tail
		tailVert[0][i][0] = actVert[i][0];
		tailVert[0][i][1] = actVert[i][1];

		// set act vert
		actVert[i][0] += velocity[i][0];
		actVert[i][1] += velocity[i][1];

	}
}



void mainController::draw()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, 1200, 900);

	double viewRatio = 1200.0 / 900.0;

	// set matrix
	matrix_obj->initMatrix();

	matrix_obj->orthogonal(-viewRatio, viewRatio, -1.0, 1.0, -1.0, 1.0);
	matrix_obj->lookAt(0.0, 0.0, 1.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);
	matrix_obj->rotate_Zdeg(180.0);
	matrix_obj->scale_XYZ(-1.0, 1.0, 1.0);
	matrix_obj->translate_XYZ(0.0, 0.0, 0.0);
	glUseProgram(PRG_OBJ);
	glUniformMatrix4fv(UNF_mvpMatrix, 1, GL_FALSE, matrix_obj->getMatrix());

	glLineWidth(2.0);

	// draw vert
	glBindVertexArray(VAO_name);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_name);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*NUM_VERT * 4, actVert, GL_DYNAMIC_DRAW);

	glDrawArrays(GL_POINTS, 0, NUM_VERT);
	glDrawArrays(GL_LINES, 0, NUM_VERT);

	// draw tail
	//glBindBuffer(GL_ARRAY_BUFFER, VBO_name);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*NUM_VERT* 5 * 4, tailVert, GL_DYNAMIC_DRAW);

	//glDrawArrays(GL_POINTS, 0, NUM_VERT * 5);
	//glDrawArrays(GL_LINES, 0, NUM_VERT * 5);
	//
	SwapBuffers(gl_obj->OpenGL_HDC);
}