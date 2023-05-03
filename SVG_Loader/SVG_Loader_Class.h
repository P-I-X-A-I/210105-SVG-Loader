#pragma once

#include <math.h>

#define MAX_NUM_POINT 2000000

class SVG_Loader_Class
{
public:

	bool isInterpolate = true;


	double CURRENT_POS[2] = {0.0};
	bool isFirstPoint = true;
	double firstPoint[2] = {0.0};

	bool isPrevHandle = false;
	double prevHandle[2] = { 0.0 };

	int CURVE_DIV = 10;
	int LINE_DIV = 10;

	int NUM_P = 0;
	float pVert[MAX_NUM_POINT][4];
	float scaleVert[MAX_NUM_POINT][4];


	SVG_Loader_Class();
	~SVG_Loader_Class();

	void load_svg_from_path(const char* path);
	void scale_point_data(double scale);
	void flip_y();
	float* get_pVert_pointer();

private:
	void find_line(FILE* filePtr);
	void find_path(FILE* filePtr);
	void find_rect(FILE* filePtr);
	//void find_polygon(FILE* filePtr);
	void find_circle(FILE* filePtr);
	void find_ellipse(FILE* filePtr);
	void fine_polyline(FILE* filePtr, bool isOpenPath);


	// move to
	void path_M(char** charPtr);
	void path_m(char** charPtr);

	// line to
	void path_L(char** charPtr);
	void path_l(char** charPtr);

	// horizontal line
	void path_H(char** charPtr);
	void path_h(char** charPtr);

	// vertical line
	void path_V(char** charPtr);
	void path_v(char** charPtr);

	// close path
	void path_Z(char** charPtr);
	void path_z(char** charPtr);// same as Z

	// 3 bezier line
	void path_C(char** charPtr);
	void path_c(char** charPtr);

	// 3 bezier line
	void path_S(char** charPtr);
	void path_s(char** charPtr);

	// 2 bezier line
	void path_Q(char** charPtr);
	void path_q(char** charPtr);
	void path_T(char** charPtr);
	void path_t(char** charPtr);


	int find_value_until_next_alphabet(char** readPtr, double* vPtr);
};

