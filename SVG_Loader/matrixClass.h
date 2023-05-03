#pragma once

class matrixClass
{
public:

	float MATRIX[16];
	float INVERSE[16];

	// constructor
	matrixClass();
	// destructor
	~matrixClass();

	// init and get
	void initMatrix();
	float* getMatrix();
	float* getInverseMatrix(float* ptr);
	float* getInverse_of_currentMatrix();

	// copy
	void copy4x4MatrixToPointer(float* ptr);

	// rotation
	void rotate_Xdeg( float degree );
	void rotate_Ydeg( float degree );
	void rotate_Zdeg( float degree );

	// translation
	void translate_XYZ( float tx, float ty, float tz );

	// scaling
	void scale_XYZ( float sx, float sy, float sz );

	//calculate matrix
	void calculateMatrix( float* matrix );
	void calculateVec4( float* vec4 );
	void calculateVec3( float* vec3 );

	// lookat
	void lookAt( float eyeX, float eyeY, float eyeZ, float viewX, float viewY, float viewZ, float headX, float headY, float headZ );

	// perspective
	void perspective( float fovy, float aspectRatio, float nearPlane, float farPlane );

	// orthogonal
	void orthogonal( float leftVal, float rightVal, float bottomVal, float topVal, float nearVal, float farVal);

};

