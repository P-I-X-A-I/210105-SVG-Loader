#include "OpenGL_Manager_Class.h"


OpenGL_Manager_Class::OpenGL_Manager_Class()
{
	// constructor
	printf("OpenGL Manager Class init\n");
}


OpenGL_Manager_Class::~OpenGL_Manager_Class()
{
	// destructor
}

bool OpenGL_Manager_Class::init_OpenGL_functions(HWND dummyWnd, HWND glWnd)
{
	printf("\n*** INIT OPENGL ***\n");
	// get device context
	HDC dummyDC = GetDC(dummyWnd);

	// dummy PFD
	PIXELFORMATDESCRIPTOR dummy_pfd = {
	sizeof(PIXELFORMATDESCRIPTOR), // size of structure
	PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, // some flag
	PFD_TYPE_RGBA,		// always RGBA
	32,					// color depth
	0, 0,				// R ( not used )
	0, 0,				// G ( not used )
	0, 0,				// B ( not used )
	0, 0,				// A ( not used )
	0,					// Accum bit ( not used )
	0,					// Accum R ( not used )
	0,					// Accum G ( not used )
	0,					// Accum B ( not used )
	0,					// Accum A ( not used )
	16,					// Depth bit ( if you set other than 16, maybe fail on some system.
	8,					// Stencil bit
	0,					// Aux buffer
	PFD_MAIN_PLANE,
	0,					// bReversed ( not used )
	0,					// bwLayerMask ( not used )
	0,					// dwVisibleMask ( not used )
	0					// dwDamageMask ( not used )
	};

	int dummy_pFormat = ChoosePixelFormat(dummyDC, &dummy_pfd);
	if (dummy_pFormat == 0)
	{
		printf("Getting dummy pixel format fails... return\n");
		return false;
	}
	else
	{
		printf("num of pixel format %d\n", dummy_pFormat);
	}


	//set dummy pixel format to dummy device context.
	SetPixelFormat(dummyDC, dummy_pFormat, &dummy_pfd);

	// create dummy OpenGL context
	HGLRC dummy_glContext = wglCreateContext(dummyDC);

	if (dummy_glContext == NULL)
	{
		printf("Create dummy OpenGL context fails... return\n");
		return false;
	}


	// make context current
	wglMakeCurrent(dummyDC, dummy_glContext);

	// get OpenGL function pointer through glew
	GLenum error = glewInit();

	if (error == GLEW_OK)
	{
		printf("glewInit SUCCESS.\n");
	}
	else
	{
		printf("glewInit fail... return\n");
		return false;
	}

	// destroy dummy things
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(dummy_glContext);


	//*************** Create actual OpenGL context

	// get device context
	OpenGL_HDC = GetDC(glWnd);

	PIXELFORMATDESCRIPTOR void_pfd;

	// check if OpengGL 3.x or higher are available
	if (!WGLEW_ARB_create_context && !WGLEW_ARB_pixel_format)
	{
		printf("OpenGL 3.x or higher are not available on this system... return\n");
		return false;
	}


	// Let WGL function choose pixel format
	const int pAttr[] = {
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_EXT, GL_TRUE,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_SAMPLE_BUFFERS_ARB, 1, // should be 1
		WGL_SAMPLES_ARB, 4,		// 2, 4, 8
		WGL_COLOR_BITS_ARB, 32,
		WGL_DEPTH_BITS_ARB, 16,		// if you set other than 16, maybe fail on some system.
		WGL_STENCIL_BITS_ARB, 8,
		0,
	};

	// set opengl version
	static const int cAttr[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 6,
		//WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,  // shouldn't be used this feature.
		WGL_CONTEXT_PROFILE_MASK_ARB,	WGL_CONTEXT_CORE_PROFILE_BIT_ARB, // it support only new (not fixed pipeline) opengl features. 
		0,
	};

	int pFormat;
	UINT numFormat;

	bool yn = wglChoosePixelFormatARB(
		//hWnd_DC,	// device context of main window
		OpenGL_HDC, // device context of openGL view
		pAttr,			// attribute
		NULL,			// float attribute
		1,				// the number of format which will be returned.
		&pFormat,		// container for returned format
		&numFormat		// the actual number of formats which is returned.
	);

	printf("%d pixel fomat are returned.\n", numFormat);


	// set pixel format to context
	SetPixelFormat(OpenGL_HDC, pFormat, &void_pfd);


	// create opengl context
	OpenGL_RC = wglCreateContextAttribsARB(
		OpenGL_HDC,
		NULL,
		cAttr
	);

	if (OpenGL_RC == NULL)
	{
		printf("creating OpenGL context fail... return\n");
		return false;
	}



	// make OpenGL context current
	wglMakeCurrent(OpenGL_HDC, OpenGL_RC);

	// check context status
	printf("VERSION : %s\n", glGetString(GL_VERSION));
	printf("RENDERER : %s\n", glGetString(GL_RENDERER));
	printf("GLSL : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	int tempVal;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &tempVal);
	printf("max_vertex_attribs %d\n", tempVal);

	glGetIntegerv(GL_MAX_VARYING_VECTORS, &tempVal);
	printf("max_varying_vectors %d\n", tempVal);

	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &tempVal);
	printf("max_vertex_uniform_vectors %d\n", tempVal);

	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &tempVal);
	printf("max_fragment_uniform_vectors %d\n", tempVal);

	glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &tempVal);
	printf("max_vertex_texture_image_units %d\n", tempVal);

	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &tempVal);
	printf("max_texture_image_units %d\n", tempVal);

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &tempVal);
	printf("max_texture_size %d\n", tempVal);

	GLint tempSize[2];
	glGetIntegerv(GL_MAX_VIEWPORT_DIMS, tempSize);
	printf("max_viewport_size %d %d\n", tempSize[0], tempSize[1]);


	// test buffer clear
	glClearColor(1.0, 1.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SwapBuffers(OpenGL_HDC);

	return true;
}


void OpenGL_Manager_Class::set_OpenGL_status()
{
	wglSwapIntervalEXT(1);

	//multisample
	//glDisable(GL_MULTISAMPLE);	
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

	// blend
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// cullface
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Dither
	glDisable(GL_DITHER); // meaningless anymore

	// polygon offset
	// call glPolygonOffset() if needed
	//glEnable(GL_POLYGON_OFFSET_FILL);
	//glEnable(GL_POLYGON_OFFSET_LINE);
	//glEnable(GL_POLYGON_OFFSET_POINT);


	// polygon smooth
	glDisable(GL_POLYGON_SMOOTH); // it causes black line on polygon's edge. so disabled is recommended.


	// specify pointsize in shader
	glEnable(GL_PROGRAM_POINT_SIZE);


}


//+++++++++++++++ Shader Function +++++++++++++++++//

bool OpenGL_Manager_Class::loadShaderSource_And_CompileShader(const char* name, int type, GLuint* toObj)
{
	// C++ file
	std::ifstream sFile;

	// file open
	sFile.open(name);

	if (!sFile)
	{
		printf("%s can't be opened... return\n", name);
		return false;
	}

	// set file's position to end
	sFile.seekg(0, std::ios::end);


	// get file length
	unsigned long fLength = sFile.tellg();
	if (fLength == 0)
	{
		printf("this file contains nothing... return\n");
		sFile.close();
		return false;
	}
	else
	{
		printf("file length:%d\n", fLength);
	}


	// reset file position
	sFile.seekg(std::ios::beg);


	// create array for shader source
	GLchar* sourceArray = new char[fLength + 1];

	// set terminate zero
	sourceArray[fLength] = 0;


	// read source string
	int INDEX = 0;
	while (sFile.good())
	{
		sourceArray[INDEX] = sFile.get();

		if (!sFile.eof())
		{
			INDEX++;
		}
	}

	// set terminate zero
	sourceArray[INDEX] = 0;


	// close file
	sFile.close();



	// create shader object
	switch (type)
	{
	case 0: // vertex shader
		*toObj = glCreateShader(GL_VERTEX_SHADER);
		break;
	case 1:
		*toObj = glCreateShader(GL_GEOMETRY_SHADER);
		break;
	case 2:
		*toObj = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	case 3:
		*toObj = glCreateShader(GL_COMPUTE_SHADER);
		break;
	default:
		printf("shader type should be 0, 1, 2 or 3... return\n");
		return false;
		break;
	}


	// supply shader source to shader object
	glShaderSource(*toObj, 1, (const GLchar**)&sourceArray, NULL);

	// compile shader
	glCompileShader(*toObj);


	// error check
	GLint logLength;
	glGetShaderiv(*toObj, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 1)
	{
		GLchar* log = (GLchar*)malloc(logLength);
		glGetShaderInfoLog(*toObj, logLength, &logLength, log);
		printf("%s compile error %d :\n%s\n", name, logLength, log);
		free(log);
	}
	else
	{
		printf("%s compile success!\n", name);
	}


	// delete source array

	delete[] sourceArray;


	return true;
}



bool OpenGL_Manager_Class::loadShaderSource_And_CompileShader_fromPtr(char* srcPtr, int type, GLuint* toObj)
{
	// create shader object
	switch (type)
	{
	case 0: // vertex shader
		*toObj = glCreateShader(GL_VERTEX_SHADER);
		break;
	case 1:
		*toObj = glCreateShader(GL_GEOMETRY_SHADER);
		break;
	case 2:
		*toObj = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	case 3:
		*toObj = glCreateShader(GL_COMPUTE_SHADER);
		break;
	default:
		printf("shader type should be 0, 1, 2 or 3... return\n");
		return false;
		break;
	}


	// supply shader source to shader object
	glShaderSource(*toObj, 1, (const GLchar**)&srcPtr, NULL);

	// compile shader
	glCompileShader(*toObj);


	// error check
	GLint logLength;
	glGetShaderiv(*toObj, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 1)
	{
		GLchar* log = (GLchar*)malloc(logLength);
		glGetShaderInfoLog(*toObj, logLength, &logLength, log);
		printf("compile error %d :\n%s\n", logLength, log);
		free(log);
	}
	else
	{
		printf("compile success!\n");
	}
	return true;
}



bool OpenGL_Manager_Class::createProgram_And_AttachShader(GLuint* prgPtr, GLuint* vsPtr, GLuint* gsPtr, GLuint* fsPtr)
{
	// create program object
	*prgPtr = glCreateProgram();

	// attach shader
	if (vsPtr != NULL)
	{
		glAttachShader(*prgPtr, *vsPtr);
	}
	if (gsPtr != NULL)
	{
		glAttachShader(*prgPtr, *gsPtr);
	}
	if (fsPtr != NULL)
	{
		glAttachShader(*prgPtr, *fsPtr);
	}


	// link program
	glLinkProgram(*prgPtr);


	// check status
	GLint status;
	glGetProgramiv(*prgPtr, GL_LINK_STATUS, &status);
	if (status == GL_TRUE)
	{
		printf("program object link success!\n");
	}
	else
	{
		printf("program link error log :\n");
		GLint logLength;
		glGetProgramiv(*prgPtr, GL_INFO_LOG_LENGTH, &logLength);

		GLchar* log = (GLchar*)malloc(logLength);
		glGetProgramInfoLog(*prgPtr, logLength, &logLength, log);
		printf("%s\n", log);
		free(log);

		return false;
	}


	return true;
}


bool OpenGL_Manager_Class::createComputeProgram_And_AttachShader(GLuint* prgPtr, GLuint* csPtr)
{
	// create program
	*prgPtr = glCreateProgram();

	// attach shader
	if (csPtr != NULL)
	{
		glAttachShader(*prgPtr, *csPtr);
	}
	else
	{
		printf("NULL is passed at compute shader...\n");
	}


	// link program
	glLinkProgram(*prgPtr);


	// check status
	GLint status;
	glGetProgramiv(*prgPtr, GL_LINK_STATUS, &status);
	if (status == GL_TRUE)
	{
		printf("compute program object link success!\n");
	}
	else
	{
		printf("compute program link error log :\n");
		GLint logLength;
		glGetProgramiv(*prgPtr, GL_INFO_LOG_LENGTH, &logLength);

		GLchar* log = (GLchar*)malloc(logLength);
		glGetProgramInfoLog(*prgPtr, logLength, &logLength, log);
		printf("%s\n", log);
		free(log);

		return false;
	}

	return true;
}



void OpenGL_Manager_Class::getUniformLocation(GLuint* prgObj, GLint* uniform, const char* name)
{
	*uniform = glGetUniformLocation(*prgObj, name);
	printf("PRG:%d, %s, %d\n", *prgObj, name, *uniform);

}

