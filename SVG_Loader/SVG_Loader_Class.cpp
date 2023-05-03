#include "framework.h"
#include "SVG_Loader_Class.h"


SVG_Loader_Class::SVG_Loader_Class()
{
	for (int i = 0; i < MAX_NUM_POINT; i++)
	{
		pVert[i][0] = 0.0;
		pVert[i][1] = 0.0;
		pVert[i][2] = 0.0;
		pVert[i][3] = 1.0;

		scaleVert[i][0] = 0.0;
		scaleVert[i][1] = 0.0;
		scaleVert[i][2] = 0.0;
		scaleVert[i][3] = 1.0;
	}
}


SVG_Loader_Class::~SVG_Loader_Class()
{

}


void SVG_Loader_Class::scale_point_data(double scale)
{
	for (int i = 0; i < NUM_P; i++)
	{
		scaleVert[i][0] = pVert[i][0] * scale;
		scaleVert[i][1] = pVert[i][1] * scale;
	}
}

void SVG_Loader_Class::flip_y()
{
	for (int i = 0; i < NUM_P; i++)
	{
		scaleVert[i][1] = scaleVert[i][1] * -1.0;
	}
}


float* SVG_Loader_Class::get_pVert_pointer()
{
	return &scaleVert[0][0];
}

//////// FIND TAGS <path, <ellipse, <line, <rect, ........ ///////////


void SVG_Loader_Class::load_svg_from_path(const char* path)
{
	// init numP
	NUM_P = 0;

	FILE* fp;
	errno_t error = fopen_s(&fp, path, "r");

	if (error != 0)
	{
		printf("svg can't open. return\n");
		return;
	}


	// read
	while(1)
	{
		// read
		char rBuf[2] = {'\0'};
		fread(&rBuf, sizeof(char), 1, fp);


		if (rBuf[0] == '<')// start row
		{
			// if find "<"
			char tagChar[2] = { '\0' };


			// find "tag name"
			fread(tagChar, sizeof(char), 1, fp);

			// check tag
			if (tagChar[0] == 'p')
			{
				char addChar[5] = { '\0' };
				// path or polygon or polyline
				fread(addChar, sizeof(char), 4, fp);

				if (strcmp(addChar, "ath ") == 0)
				{
					this->find_path(fp);
				}
				else if (strcmp(addChar, "olyg") == 0)
				{
					this->fine_polyline(fp, false);
				}
				else if (strcmp(addChar, "olyl") == 0)
				{
					this->fine_polyline(fp, true);
				}
			}
			else if (tagChar[0] == 'r')
			{
				char addChar[4] = {'\0'};
				// rect

				fread(addChar, sizeof(char), 3, fp);

				if (strcmp(addChar, "ect") == 0)
				{
					this->find_rect(fp);
				}
			}
			else if (tagChar[0] == 'l')
			{
				char addChar[4] = { '\0' };
				// line

				fread(addChar, sizeof(char), 3, fp);

				if (strcmp(addChar, "ine") == 0)
				{
					this->find_line(fp);
				}
			}
			else if(tagChar[0] == 'c')
			{ 
				char addChar[4] = {'\0'};
				// circle

				fread(addChar, sizeof(char), 3, fp);

				if (strcmp(addChar, "irc") == 0)
				{
					this->find_circle(fp);
				}
			}
			else if( tagChar[0] == 'e')
			{ 
				char addChar[4] = {'\0'};
				// ellipse

				fread(addChar, sizeof(char), 3, fp);

				if (strcmp(addChar, "lli") == 0)
				{
					this->find_ellipse(fp);
				}
			}
		}


		// if find file's end
		if ( rBuf[0] == '\0' )
		{
			break;
		}
	}
	printf("NUM_P %d\n", NUM_P);
	fclose(fp);
}




///// PROCESS EACH TAGS ////////////////////////


void SVG_Loader_Class::find_line(FILE* filePtr)
{
	//printf("find line\n");

	int NUM = 0;
	char readBuf[1000] = { '\0' };

	// read row ***************************
	while (1)
	{
		fread(&readBuf[NUM], sizeof(char), 1, filePtr);

		if (readBuf[NUM] == '>')
		{
			break;
		}
		NUM++;
	}

	// find vertex ************************
	char* x1 = strstr(readBuf, "x1");
	char* y1 = strstr(readBuf, "y1");
	char* x2 = strstr(readBuf, "x2");
	char* y2 = strstr(readBuf, "y2");
	// skip x1="
	x1 += 4;
	y1 += 4;
	x2 += 4;
	y2 += 4;

	char x1_str[20] = { '\0' };
	char y1_str[20] = { '\0' };
	char x2_str[20] = { '\0' };
	char y2_str[20] = { '\0' };

	NUM = 0;
	while (1)
	{
		x1_str[NUM] = *x1;
		x1++;

		if (x1_str[NUM] == '"')
		{break;}

		NUM++;
	}
	NUM = 0;
	while (1)
	{
		y1_str[NUM] = *y1;
		y1++;

		if (y1_str[NUM] == '"')
		{break;}

		NUM++;
	}
	NUM = 0;
	while (1)
	{
		x2_str[NUM] = *x2;
		x2++;

		if (x2_str[NUM] == '"')
		{break;}

		NUM++;
	}
	NUM = 0;
	while (1)
	{
		y2_str[NUM] = *y2;
		y2++;

		if (y2_str[NUM] == '"')
		{break;}

		NUM++;
	}
	double x1_val = atof(x1_str);
	double y1_val = atof(y1_str);
	double x2_val = atof(x2_str);
	double y2_val = atof(y2_str);


	// update point(interpolated)
	if (isInterpolate)
	{
		float delta[2];
		delta[0] = (x2_val - x1_val) / (double)LINE_DIV;
		delta[1] = (y2_val - y1_val) / (double)LINE_DIV;

		for (int i = 0; i < LINE_DIV; i++)
		{
			pVert[NUM_P][0] = x1_val + i*delta[0];
			pVert[NUM_P][1] = y1_val + i*delta[1];
			NUM_P++;

			pVert[NUM_P][0] = x1_val + (i+1) * delta[0];
			pVert[NUM_P][1] = y1_val + (i+1) * delta[1];
			NUM_P++;
		}
	}
	else
	{
		pVert[NUM_P][0] = x1_val;
		pVert[NUM_P][1] = y1_val;
		NUM_P++;

		pVert[NUM_P][0] = x2_val;
		pVert[NUM_P][1] = y2_val;
		NUM_P++;
	}
	//printf("line NUM_P : %d\n", NUM_P);
}



void SVG_Loader_Class::find_rect(FILE* filePtr)
{
	//printf("find rect\n");
	int NUM = 0;
	char readBuf[1000] = {'\0'};

	// read row ***************************
	while (1)
	{
		fread(&readBuf[NUM], sizeof(char), 1, filePtr);

		if (readBuf[NUM] == '>')
		{
			break;
		}
		NUM++;
	}

	// find vertex ************************
	char* x = strstr(readBuf, "x");
	char* y = strstr(readBuf, "y");
	char* w = strstr(readBuf, "width");
	char* h = strstr(readBuf, "height");

	// skip pointer
	x += 3; // x="
	y += 3; // y="
	w += 7; // width="
	h += 8; // height="

	char x_str[20] = { '\0' };
	char y_str[20] = { '\0' };
	char w_str[20] = { '\0' };
	char h_str[20] = { '\0' };

	NUM = 0;
	while (1)
	{
		x_str[NUM] = *x;
		x++;

		if( x_str[NUM] == '"')
		{break;}
		
		NUM++;
	}

	NUM = 0;
	while (1)
	{
		y_str[NUM] = *y;
		y++;

		if (y_str[NUM] == '"')
		{break;}

		NUM++;
	}

	NUM = 0;
	while (1)
	{
		w_str[NUM] = *w;
		w++;

		if (w_str[NUM] == '"')
		{break;}

		NUM++;
	}

	NUM = 0;
	while (1)
	{
		h_str[NUM] = *h;
		h++;

		if (h_str[NUM] == '"')
		{break;}

		NUM++;
	}

	// get float
	double x_val = atof(x_str);
	double y_val = atof(y_str);
	double w_val = atof(w_str);
	double h_val = atof(h_str);

	double fourP[5][2]; // point 0 & 4 is same point
	fourP[0][0] = fourP[4][0] = x_val;
	fourP[0][1] = fourP[4][1] = y_val;

	fourP[1][0] = x_val;	fourP[1][1] = y_val + h_val;
	fourP[2][0] = x_val + w_val;	fourP[2][1] = y_val + h_val;
	fourP[3][0] = x_val + w_val;	fourP[3][1] = y_val;

	if (isInterpolate)
	{
		for (int p = 0; p < 4; p++)
		{
			double startP[2];
			double endP[2];
			double delta[2];
			startP[0] = fourP[p][0];	startP[1] = fourP[p][1];
			endP[0] = fourP[p + 1][0];	endP[1] = fourP[p + 1][1];

			delta[0] = (endP[0] - startP[0]) / (double)LINE_DIV;
			delta[1] = (endP[1] - startP[1]) / (double)LINE_DIV;

			for (int i = 0; i < LINE_DIV; i++)
			{
				pVert[NUM_P][0] = startP[0] + i * delta[0];
				pVert[NUM_P][1] = startP[1] + i * delta[1];
				NUM_P++;

				pVert[NUM_P][0] = startP[0] + (i + 1)*delta[0];
				pVert[NUM_P][1] = startP[1] + (i + 1)*delta[1];
				NUM_P++;
			}
		}
	}
	else
	{
		// add point ( 8 points )
		pVert[NUM_P][0] = x_val;
		pVert[NUM_P][1] = y_val;
		NUM_P++;

		pVert[NUM_P][0] = pVert[NUM_P + 1][0] = x_val;
		pVert[NUM_P][1] = pVert[NUM_P + 1][1] = y_val + h_val;
		NUM_P += 2;

		pVert[NUM_P][0] = pVert[NUM_P + 1][0] = x_val + w_val;
		pVert[NUM_P][1] = pVert[NUM_P + 1][1] = y_val + h_val;
		NUM_P += 2;

		pVert[NUM_P][0] = pVert[NUM_P + 1][0] = x_val + w_val;
		pVert[NUM_P][1] = pVert[NUM_P + 1][1] = y_val;
		NUM_P += 2;

		pVert[NUM_P][0] = x_val;
		pVert[NUM_P][1] = y_val;
		NUM_P++;
	}
	//printf("rect NUM_P : %d\n", NUM_P);
}


void SVG_Loader_Class::find_circle(FILE* filePtr)
{
	int NUM = 0;
	char readBuf[1000] = { '\0' };

	// read row ***************************
	while (1)
	{
		fread(&readBuf[NUM], sizeof(char), 1, filePtr);

		if (readBuf[NUM] == '>')
		{
			break;
		}
		NUM++;
	}


	char* cx = strstr(readBuf, "cx=");
	char* cy = strstr(readBuf, "cy=");
	char* r = strstr(readBuf, "r=");

	// skip pointer
	cx += 4; // cx="
	cy += 4; // cy="
	r += 3; // r="

	char cx_str[20] = { '\0' };
	char cy_str[20] = { '\0' };
	char r_str[20] = { '\0' };

	NUM = 0;
	while (1)
	{
		cx_str[NUM] = *cx;
		cx++;

		if (cx_str[NUM] == '"')
		{
			break;
		}

		NUM++;
	}

	NUM = 0;
	while (1)
	{
		cy_str[NUM] = *cy;
		cy++;

		if (cy_str[NUM] == '"')
		{
			break;
		}

		NUM++;
	}

	NUM = 0;
	while (1)
	{
		r_str[NUM] = *r;
		r++;

		if (r_str[NUM] == '"')
		{
			break;
		}

		NUM++;
	}


	double cx_val = atof(cx_str);
	double cy_val = atof(cy_str);
	double r_val = atof(r_str);


	// add point
	for (int i = 0; i < 60; i++)
	{
		double rad_0 = (double)(i * 6) * 0.0174532925;
		double rad_1 = (double)((i+1) * 6) * 0.0174532925;

		pVert[NUM_P][0] = cx_val + cos(rad_0)*r_val;
		pVert[NUM_P][1] = cy_val + sin(rad_0)*r_val;
		NUM_P++;

		pVert[NUM_P][0] = cx_val + cos(rad_1)*r_val;
		pVert[NUM_P][1] = cy_val + sin(rad_1)*r_val;
		NUM_P++;
	}

	//printf("circle NUM_P : %d\n", NUM_P);
}




void SVG_Loader_Class::find_ellipse(FILE* filePtr)
{

	int NUM = 0;
	char readBuf[1000] = { '\0' };

	// read row ***************************
	while (1)
	{
		fread(&readBuf[NUM], sizeof(char), 1, filePtr);

		if (readBuf[NUM] == '>')
		{
			break;
		}
		NUM++;
	}


	char* cx = strstr(readBuf, "cx");
	char* cy = strstr(readBuf, "cy");
	char* rx = strstr(readBuf, "rx");
	char* ry = strstr(readBuf, "ry");

	cx += 4; // cx="
	cy += 4; // cy="
	rx += 4; // rx="
	ry += 4; // ry="

	char cx_str[20] = { '\0' };
	char cy_str[20] = { '\0' };
	char rx_str[20] = { '\0' };
	char ry_str[20] = { '\0' };

	NUM = 0;
	while (1)
	{
		cx_str[NUM] = *cx;
		cx++;

		if (cx_str[NUM] == '"')
		{break;}

		NUM++;
	}
	NUM = 0;
	while (1)
	{
		cy_str[NUM] = *cy;
		cy++;

		if (cy_str[NUM] == '"')
		{
			break;
		}

		NUM++;
	}
	NUM = 0;
	while (1)
	{
		rx_str[NUM] = *rx;
		rx++;

		if (rx_str[NUM] == '"')
		{
			break;
		}

		NUM++;
	}
	NUM = 0;
	while (1)
	{
		ry_str[NUM] = *ry;
		ry++;

		if (ry_str[NUM] == '"')
		{
			break;
		}

		NUM++;
	}


	double cx_val = atof(cx_str);
	double cy_val = atof(cy_str);
	double rx_val = atof(rx_str);
	double ry_val = atof(ry_str);



	// add point
	for (int i = 0; i < 60; i++)
	{
		double rad_0 = (double)(i * 6) * 0.0174532925;
		double rad_1 = (double)((i + 1) * 6) * 0.0174532925;

		pVert[NUM_P][0] = cx_val + cos(rad_0)*rx_val;
		pVert[NUM_P][1] = cy_val + sin(rad_0)*ry_val;
		NUM_P++;

		pVert[NUM_P][0] = cx_val + cos(rad_1)*rx_val;
		pVert[NUM_P][1] = cy_val + sin(rad_1)*ry_val;
		NUM_P++;
	}

	//printf("ellipse NUM_P : %d\n", NUM_P);
}



void SVG_Loader_Class::fine_polyline(FILE* filePtr, bool isOpenPath)
{
	
	int NUM = 0;
	char readBuf[100000] = { '\0' };

	// read row ***************************
	while (1)
	{
		fread(&readBuf[NUM], sizeof(char), 1, filePtr);

		if (readBuf[NUM] == '>')
		{
			break;
		}
		NUM++;
	}


	char* points = strstr(readBuf, "points=");
	points += 8; // points="

	double pt[10000][2];
	char pt_str[20] = {'\0'};
	NUM = 0;
	int num_pt = 0;

	while (1)
	{
		pt_str[NUM] = *points;
		points++;

		if (pt_str[NUM] == ',')
		{
			// x value
			pt[num_pt][0] = atof(pt_str);
			NUM = 0;
			// reset pt_str
			memset(pt_str, 0, 20);
		}
		else if (pt_str[NUM] == ' ')
		{
			// y value
			pt[num_pt][1] = atof(pt_str);
			num_pt++;
			NUM = 0;
			// reset pt_str
			memset(pt_str, 0, 20);
		}
		else if (pt_str[NUM] == '"')
		{
			break;
		}
		else
		{
			NUM++;
		}
	} // while



	// add point
	for (int i = 0; i < num_pt-1; i++)
	{
		pVert[NUM_P][0] = pt[i][0];
		pVert[NUM_P][1] = pt[i][1];
		NUM_P++;

		pVert[NUM_P][0] = pt[i+1][0];
		pVert[NUM_P][1] = pt[i+1][1];
		NUM_P++;
	}


	if (isOpenPath)
	{
		//printf("polyline NUM_P : %d\n", NUM_P);
	}
	else
	{
		// close path
		pVert[NUM_P][0] = pt[num_pt - 1][0];
		pVert[NUM_P][1] = pt[num_pt - 1][1];
		NUM_P++;

		pVert[NUM_P][0] = pt[0][0];
		pVert[NUM_P][1] = pt[0][1];
		NUM_P++;

		//printf("polygon NUM_P : %d\n", NUM_P);
	}
}

/*
void SVG_Loader_Class::find_polygon(FILE* filePtr) // CLOSE PATH
{
	this->fine_polyline(filePtr);
}
*/


///////// Process < path > ////////////////////////////////////////


void SVG_Loader_Class::find_path(FILE* filePtr)
{
	int NUM = 0;
	char readBuf[256000] = { '\0' }; // 256KB


	// read row ***************************
	while (1)
	{
		fread(&readBuf[NUM], sizeof(char), 1, filePtr);

		if (readBuf[NUM] == '>')
		{
			break;
		}
		NUM++;
	}

	// find "d="
	char* Data = strstr(readBuf, "d=");
	Data += 3;

	while (1)
	{
		char tagChar = *Data;
		Data++;


		// tags
		switch (tagChar)
		{
		case 'M':
			//printf("M");
			this->path_M( &Data );
			break;

		case 'm':
			//printf("m");
			this->path_m(&Data);
			break;
		///////////////////////////////
		case 'L':
			//printf("L");
			this->path_L(&Data);
			break;

		case 'l':
			//printf("l");
			this->path_l(&Data);
			break;
			///////////////////////////////
		case 'H':
			//printf("H");
			this->path_H(&Data);
			break;

		case 'h':
			//printf("h");
			this->path_h(&Data);
			break;
			///////////////////////////////
		case 'V':
			//printf("V");
			this->path_V(&Data);
			break;
		case 'v':
			//printf("v");
			this->path_v(&Data);
			break;
			///////////////////////////////
		case 'S':
			this->path_S(&Data);
			//printf("S");
			break;
		case 's':
			this->path_s(&Data);
			//printf("s");
			break;
			///////////////////////////////
		case 'C':
			//printf("C");
			this->path_C(&Data);
			break;
		case 'c':
			//printf("c");
			this->path_c(&Data);
			break;
			///////////////////////////////
		case 'Z':
			//printf("Z\n");
			this->path_Z(&Data);
			break;
		case 'z':
			//printf("z\n");
			this->path_z(&Data);
			break;
			///////////////////////////////
		case 'Q':
			//printf("Q");
			this->path_Q(&Data);
			break;
		case 'q':
			//printf("q");
			this->path_q(&Data);
			break;
			///////////////////////////////
		case 'T':
			//printf("T");
			this->path_T(&Data);
			break;
		case 't':
			//printf("t");
			this->path_t(&Data);
			break;
			///////////////////////////////
		case 'A':
			printf("A");
			break;
		case 'a':
			printf("a");
			break;
			///////////////////////////////
		}


		// end of "d=" part
		if (tagChar == '"')
		{
			//printf("path data end\n");
			break;
		}

	}// while

}



/////////////// path process ///////////////////////////


// update current position
void SVG_Loader_Class::path_M(char** charPtr) // move to
{
	int COUNT = 0;
	double value[1000] = {0.0};

	// find value until next alphabet emerge
	COUNT = this->find_value_until_next_alphabet(charPtr, &value[0]);

	////////////////////////////////////////////////////////////////////
	
	// process value
	CURRENT_POS[0] = value[0]; // moveto(abs) x
	CURRENT_POS[1] = value[1]; // moveto(abs) y



	// if( COUNT > 2 ), it means "line to(abs)"********************
	for (int i = 2; i < COUNT; i += 2)
	{
		if (isFirstPoint)
		{
			firstPoint[0] = CURRENT_POS[0];
			firstPoint[1] = CURRENT_POS[1];
			isFirstPoint = false;
		}


		double startP[2];
		double endP[2];
		double delta[2];
		startP[0] = CURRENT_POS[0];
		startP[1] = CURRENT_POS[1];
		endP[0] = value[i];
		endP[1] = value[i + 1];

		delta[0] = (endP[0] - startP[0]) / (double)LINE_DIV;
		delta[1] = (endP[1] - startP[1]) / (double)LINE_DIV;

		if (isInterpolate)
		{
			for (int k = 0; k < LINE_DIV; k++)
			{
				pVert[NUM_P][0] = startP[0] + k * delta[0];
				pVert[NUM_P][1] = startP[1] + k * delta[1];
				NUM_P++;

				pVert[NUM_P][0] = startP[0] + (k+1) * delta[0];
				pVert[NUM_P][1] = startP[1] + (k+1) * delta[1];
				NUM_P++;
			}
		}
		else
		{
			// add point 1
			pVert[NUM_P][0] = CURRENT_POS[0];
			pVert[NUM_P][1] = CURRENT_POS[1];
			NUM_P++;

			// point 2
			pVert[NUM_P][0] = value[i];
			pVert[NUM_P][1] = value[i + 1];
			NUM_P++;
		}

		// update current pos
		CURRENT_POS[0] = value[i];
		CURRENT_POS[1] = value[i+1];
	}
	// if( COUNT > 2 ), it means "line to"********************
	
	//printf("M");
}



void SVG_Loader_Class::path_m(char** charPtr)
{
	// COUNT is the number of "value", not "xy pair"
	int COUNT = 0;
	double value[1000] = { 0.0 };

	// find value until next alphabet emerge
	COUNT = this->find_value_until_next_alphabet(charPtr, &value[0]);


	////////////// PROCESS VALUE //////////////////////////////

	// update current pos
	CURRENT_POS[0] = CURRENT_POS[0] + value[0]; // moveto(relative) x
	CURRENT_POS[1] = CURRENT_POS[1] + value[1]; // moveto(relative) y



	// if( COUNT > 2 ), it means "line to (relative)"********************
	for (int i = 2; i < COUNT; i += 2)
	{
		if (isFirstPoint)
		{
			firstPoint[0] = CURRENT_POS[0];
			firstPoint[1] = CURRENT_POS[1];
			isFirstPoint = false;
		}


		// relative
		double delta[2];
		delta[0] = value[i] / (double)LINE_DIV;
		delta[1] = value[i+1] / (double)LINE_DIV;


		if (isInterpolate)
		{
			for (int k = 0; k < LINE_DIV; k++)
			{
				pVert[NUM_P][0] = CURRENT_POS[0] + k * delta[0];
				pVert[NUM_P][1] = CURRENT_POS[1] + k * delta[1];
				NUM_P++;

				pVert[NUM_P][0] = CURRENT_POS[0] + (k + 1) * delta[0];
				pVert[NUM_P][1] = CURRENT_POS[1] + (k + 1) * delta[1];
				NUM_P++;
			}
		}
		else
		{
			// add point 1
			pVert[NUM_P][0] = CURRENT_POS[0];
			pVert[NUM_P][1] = CURRENT_POS[1];
			NUM_P++;

			// add point 2
			pVert[NUM_P][0] = CURRENT_POS[0] + value[i];
			pVert[NUM_P][1] = CURRENT_POS[1] + value[i + 1];
			NUM_P++;
		}

		// update current pos
		CURRENT_POS[0] = CURRENT_POS[0] + value[i]; // moveto(relative) x
		CURRENT_POS[1] = CURRENT_POS[1] + value[i+1]; // moveto(relative)y
	}
	// if( COUNT > 2 ), it means "line to"********************

	//printf("m");
}


///////// Line to //////////////////////////////////////

void SVG_Loader_Class::path_L(char** charPtr)
{
	// COUNT is the number of "value", not "xy pair"
	int COUNT = 0;
	double value[1000] = { 0.0 };

	// find value until next alphabet emerge
	COUNT = this->find_value_until_next_alphabet(charPtr, &value[0]);

	// line to ( add abs point )
	for (int i = 0; i < COUNT; i += 2)
	{
		if (isFirstPoint)
		{
			firstPoint[0] = CURRENT_POS[0];
			firstPoint[1] = CURRENT_POS[1];
			isFirstPoint = false;
		}


		double startP[2];
		double endP[2];
		double delta[2];

		startP[0] = CURRENT_POS[0];
		startP[1] = CURRENT_POS[1];
		endP[0] = value[i];
		endP[1] = value[i + 1];
		delta[0] = (endP[0] - startP[0])/(double)LINE_DIV;
		delta[1] = (endP[1] - startP[1])/(double)LINE_DIV;



		if (isInterpolate)
		{
			for (int k = 0; k < LINE_DIV; k++)
			{
				pVert[NUM_P][0] = CURRENT_POS[0] + delta[0] * k;
				pVert[NUM_P][1] = CURRENT_POS[1] + delta[1] * k;
				NUM_P++;

				pVert[NUM_P][0] = CURRENT_POS[0] + delta[0] * (k+1);
				pVert[NUM_P][1] = CURRENT_POS[1] + delta[1] * (k+1);
				NUM_P++;
			}
		}
		else
		{
			pVert[NUM_P][0] = CURRENT_POS[0];
			pVert[NUM_P][1] = CURRENT_POS[1];
			NUM_P++;

			pVert[NUM_P][0] = value[i];
			pVert[NUM_P][1] = value[i + 1];
			NUM_P++;

		}

		// update current pos
		CURRENT_POS[0] = value[i];
		CURRENT_POS[1] = value[i + 1];
	}

	//printf("L");
}


void SVG_Loader_Class::path_l(char** charPtr)
{
	// COUNT is the number of "value", not "xy pair"
	int COUNT = 0;
	double value[1000] = { 0.0 };

	// find value until next alphabet emerge
	COUNT = this->find_value_until_next_alphabet(charPtr, &value[0]);

	// line to ( add relative point )
	for (int i = 0; i < COUNT; i += 2)
	{
		if (isFirstPoint)
		{
			firstPoint[0] = CURRENT_POS[0];
			firstPoint[1] = CURRENT_POS[1];
			isFirstPoint = false;
		}

		// relative
		double delta[2];
		delta[0] = value[i] / (double)LINE_DIV;
		delta[1] = value[i+1] / (double)LINE_DIV;


		if (isInterpolate)
		{
			for (int k = 0; k < LINE_DIV; k++)
			{
				pVert[NUM_P][0] = CURRENT_POS[0] + delta[0] * k;
				pVert[NUM_P][1] = CURRENT_POS[1] + delta[1] * k;
				NUM_P++;

				pVert[NUM_P][0] = CURRENT_POS[0] + delta[0] * (k + 1);
				pVert[NUM_P][1] = CURRENT_POS[1] + delta[1] * (k + 1);
				NUM_P++;
			}
		}
		else
		{
			pVert[NUM_P][0] = CURRENT_POS[0];
			pVert[NUM_P][1] = CURRENT_POS[1];
			NUM_P++;

			pVert[NUM_P][0] = CURRENT_POS[0] + value[i];
			pVert[NUM_P][1] = CURRENT_POS[1] + value[i + 1];
			NUM_P++;
		}

		// update current pos
		CURRENT_POS[0] = CURRENT_POS[0] + value[i];
		CURRENT_POS[1] = CURRENT_POS[1] + value[i + 1];
	}

	//printf("l");
}


////////// horizontal line ////////////////


void SVG_Loader_Class::path_H(char** charPtr)
{
	// COUNT is the number of "value", not "xy pair"
	int COUNT = 0;
	double value[1000] = { 0.0 };

	// find value until next alphabet emerge
	COUNT = this->find_value_until_next_alphabet(charPtr, &value[0]);


	// add horizontal line
	for (int i = 0; i < COUNT; i++)
	{
		if (isFirstPoint)
		{
			firstPoint[0] = CURRENT_POS[0];
			firstPoint[1] = CURRENT_POS[1];
			isFirstPoint = false;
		}

		double startP;
		double endP;
		double delta;

		startP = CURRENT_POS[0];
		endP = value[i];
		delta = (endP - startP) / (double)LINE_DIV;

		if (isInterpolate)
		{
			for (int k = 0; k < LINE_DIV; k++)
			{
				pVert[NUM_P][0] = CURRENT_POS[0] + delta * k;
				pVert[NUM_P][1] = CURRENT_POS[1];
				NUM_P++;

				pVert[NUM_P][0] = CURRENT_POS[0] + delta * (k + 1);
				pVert[NUM_P][1] = CURRENT_POS[1];
				NUM_P++;
			}
		}
		else
		{
			// add point 1
			pVert[NUM_P][0] = CURRENT_POS[0];
			pVert[NUM_P][1] = CURRENT_POS[1];
			NUM_P++;

			// add point 2
			pVert[NUM_P][0] = value[i];
			pVert[NUM_P][1] = CURRENT_POS[1];
			NUM_P++;
		}
		// update current pos
		CURRENT_POS[0] = value[i];
	}

	//printf("H");
}


void SVG_Loader_Class::path_h(char** charPtr)
{
	// COUNT is the number of "value", not "xy pair"
	int COUNT = 0;
	double value[1000] = { 0.0 };

	// find value until next alphabet emerge
	COUNT = this->find_value_until_next_alphabet(charPtr, &value[0]);


	// add horizontal line ( relative )
	for (int i = 0; i < COUNT; i++)
	{
		if (isFirstPoint)
		{
			firstPoint[0] = CURRENT_POS[0];
			firstPoint[1] = CURRENT_POS[1];
			isFirstPoint = false;
		}

		// relative
		double delta = value[i] / (double)LINE_DIV;

		if (isInterpolate)
		{
			for (int k = 0; k < LINE_DIV; k++)
			{
				pVert[NUM_P][0] = CURRENT_POS[0] + delta * k;
				pVert[NUM_P][1] = CURRENT_POS[1];
				NUM_P++;

				pVert[NUM_P][0] = CURRENT_POS[0] + delta * (k + 1);
				pVert[NUM_P][1] = CURRENT_POS[1];
				NUM_P++;
			}
		}
		else
		{
			pVert[NUM_P][0] = CURRENT_POS[0];
			pVert[NUM_P][1] = CURRENT_POS[1];
			NUM_P++;

			pVert[NUM_P][0] = CURRENT_POS[0] + value[i];
			pVert[NUM_P][1] = CURRENT_POS[1];
			NUM_P++;
		}

		// update current pos
		CURRENT_POS[0] = CURRENT_POS[0] + value[i];
	}

	//printf("h");
}


////////// Vertical line /////////////////////

void SVG_Loader_Class::path_V(char** charPtr)
{
	// COUNT is the number of "value", not "xy pair"
	int COUNT = 0;
	double value[1000] = { 0.0 };

	// find value until next alphabet emerge
	COUNT = this->find_value_until_next_alphabet(charPtr, &value[0]);


	// add vertical line
	for (int i = 0; i < COUNT; i++)
	{
		if (isFirstPoint)
		{
			firstPoint[0] = CURRENT_POS[0];
			firstPoint[1] = CURRENT_POS[1];
			isFirstPoint = false;
		}

		double startP = CURRENT_POS[1];
		double endP = value[i];
		double delta = (endP - startP) / (double)LINE_DIV;

		if (isInterpolate)
		{
			for (int k = 0; k < LINE_DIV; k++)
			{
				pVert[NUM_P][0] = CURRENT_POS[0];
				pVert[NUM_P][1] = CURRENT_POS[1] + delta * k;
				NUM_P++;

				pVert[NUM_P][0] = CURRENT_POS[0];
				pVert[NUM_P][1] = CURRENT_POS[1] + delta * (k + 1);
				NUM_P++;
			}
		}
		else
		{
			pVert[NUM_P][0] = CURRENT_POS[0];
			pVert[NUM_P][1] = CURRENT_POS[1];
			NUM_P++;

			pVert[NUM_P][0] = CURRENT_POS[0];
			pVert[NUM_P][1] = value[i];
			NUM_P++;
		}
		// update current pos
		CURRENT_POS[1] = value[i];
	}

	//printf("V");
}


void SVG_Loader_Class::path_v(char** charPtr)
{
	// COUNT is the number of "value", not "xy pair"
	int COUNT = 0;
	double value[1000] = { 0.0 };

	// find value until next alphabet emerge
	COUNT = this->find_value_until_next_alphabet(charPtr, &value[0]);

	// add vertical line ( relative )
	for (int i = 0; i < COUNT; i++)
	{
		if (isFirstPoint)
		{
			firstPoint[0] = CURRENT_POS[0];
			firstPoint[1] = CURRENT_POS[1];
			isFirstPoint = false;
		}

		double delta = value[i] / (double)LINE_DIV;

		if (isInterpolate)
		{
			for (int k = 0; k < LINE_DIV; k++)
			{
				pVert[NUM_P][0] = CURRENT_POS[0];
				pVert[NUM_P][1] = CURRENT_POS[1] + delta * k;
				NUM_P++;

				pVert[NUM_P][0] = CURRENT_POS[0];
				pVert[NUM_P][1] = CURRENT_POS[1] + delta * (k + 1);
				NUM_P++;
			}
		}
		else
		{
			pVert[NUM_P][0] = CURRENT_POS[0];
			pVert[NUM_P][1] = CURRENT_POS[1];
			NUM_P++;

			pVert[NUM_P][0] = CURRENT_POS[0];
			pVert[NUM_P][1] = CURRENT_POS[1] + value[i];
			NUM_P++;
		}
		// update current pos
		CURRENT_POS[1] = CURRENT_POS[1] + value[i];
	}

	//printf("v");
}


///////////// close path ///////////////////////

void SVG_Loader_Class::path_Z(char** charPtr)
{
	this->path_z(charPtr);
}
void SVG_Loader_Class::path_z(char** charPtr) // same as Z
{
	// reset flag
	isFirstPoint = true;
	isPrevHandle = false;

	// close path
	pVert[NUM_P][0] = CURRENT_POS[0];
	pVert[NUM_P][1] = CURRENT_POS[1];
	NUM_P++;

	pVert[NUM_P][0] = firstPoint[0];
	pVert[NUM_P][1] = firstPoint[1];
	NUM_P++;

	// update current pos
	CURRENT_POS[0] = firstPoint[0];
	CURRENT_POS[1] = firstPoint[1];

	// reset first point
	firstPoint[0] = 0.0;
	firstPoint[1] = 0.0;
	prevHandle[0] = 0.0;
	prevHandle[1] = 0.0;

	//printf("z\n");
}


/////////////////// Bezier path //////////////////////////////

void SVG_Loader_Class::path_C(char** charPtr)
{
	int DIV = CURVE_DIV;

	// COUNT is the number of "value", not "xy pair"
	int COUNT = 0;
	double value[1000] = { 0.0 };

	// find value until next alphabet emerge
	COUNT = this->find_value_until_next_alphabet(charPtr, &value[0]);


	// add Bezier curve ( absolute )
	for (int i = 0; i < COUNT; i += 6)
	{
		// startPoint, h1, h2, endPoint
		double s[2];
		double h1[2];
		double h2[2];
		double e[2];

		s[0] = CURRENT_POS[0];
		s[1] = CURRENT_POS[1];

		if (isFirstPoint)
		{
			firstPoint[0] = CURRENT_POS[0];
			firstPoint[1] = CURRENT_POS[1];
			isFirstPoint = false;
		}

		h1[0] = value[i];
		h1[1] = value[i + 1];

		h2[0] = value[i + 2];
		h2[1] = value[i + 3];

		e[0] = value[i + 4];
		e[1] = value[i + 5];


		// update current pos
		CURRENT_POS[0] = e[0];
		CURRENT_POS[1] = e[1];


		// add point
		for (int t = 0; t < DIV; t++)
		{
			// point1
			double coef = (double)t / (double)DIV;

			double A = (1.0 - coef)*(1.0 - coef)*(1.0 - coef);
			double B = 3.0*(1.0 - coef)*(1.0 - coef)*coef;
			double C = 3.0*(1.0 - coef)*coef*coef;
			double D = coef * coef*coef;

			pVert[NUM_P][0] = A * s[0] + B * h1[0] + C * h2[0] + D * e[0];
			pVert[NUM_P][1] = A * s[1] + B * h1[1] + C * h2[1] + D * e[1];
			NUM_P++;

			// point2
			coef = (double)(t+1) / (double)DIV;

			A = (1.0 - coef)*(1.0 - coef)*(1.0 - coef);
			B = 3.0*(1.0 - coef)*(1.0 - coef)*coef;
			C = 3.0*(1.0 - coef)*coef*coef;
			D = coef * coef*coef;

			pVert[NUM_P][0] = A * s[0] + B * h1[0] + C * h2[0] + D * e[0];
			pVert[NUM_P][1] = A * s[1] + B * h1[1] + C * h2[1] + D * e[1];
			NUM_P++;
		}

		// update mirror handle
		if (isPrevHandle == false)
		{
			isPrevHandle = true;
		}
		prevHandle[0] = e[0] + (e[0] - h2[0]);
		prevHandle[1] = e[1] + (e[1] - h2[1]);

	}

	//printf("C\n");
}




void SVG_Loader_Class::path_c(char** charPtr)
{
	int DIV = CURVE_DIV;

	// COUNT is the number of "value", not "xy pair"
	int COUNT = 0;
	double value[1000] = { 0.0 };

	// find value until next alphabet emerge
	COUNT = this->find_value_until_next_alphabet(charPtr, &value[0]);


	// add Bezier curve ( relative )
	for (int i = 0; i < COUNT; i += 6)
	{
		// startPoint, h1, h2, endPoint
		double s[2];
		double h1[2];
		double h2[2];
		double e[2];

		s[0] = CURRENT_POS[0];
		s[1] = CURRENT_POS[1];

		if (isFirstPoint)
		{
			firstPoint[0] = CURRENT_POS[0];
			firstPoint[1] = CURRENT_POS[1];
			isFirstPoint = false;
		}

		h1[0] = s[0] + value[i];
		h1[1] = s[1] + value[i + 1];

		h2[0] = s[0] + value[i + 2];
		h2[1] = s[1] + value[i + 3];

		e[0] = s[0] + value[i + 4];
		e[1] = s[1] + value[i + 5];


		// update current pos
		CURRENT_POS[0] = e[0];
		CURRENT_POS[1] = e[1];


		// add point
		for (int t = 0; t < DIV; t++)
		{
			// point 1
			double coef = (double)t / (double)DIV;
			double A = (1.0 - coef)*(1.0 - coef)*(1.0 - coef);
			double B = 3.0*(1.0 - coef)*(1.0 - coef)*coef;
			double C = 3.0*(1.0 - coef)*coef*coef;
			double D = coef * coef*coef;

			pVert[NUM_P][0] = A * s[0] + B * h1[0] + C * h2[0] + D * e[0];
			pVert[NUM_P][1] = A * s[1] + B * h1[1] + C * h2[1] + D * e[1];
			NUM_P++;

			// point 2
			coef = (double)(t+1) / (double)DIV;
			A = (1.0 - coef)*(1.0 - coef)*(1.0 - coef);
			B = 3.0*(1.0 - coef)*(1.0 - coef)*coef;
			C = 3.0*(1.0 - coef)*coef*coef;
			D = coef * coef*coef;

			pVert[NUM_P][0] = A * s[0] + B * h1[0] + C * h2[0] + D * e[0];
			pVert[NUM_P][1] = A * s[1] + B * h1[1] + C * h2[1] + D * e[1];
			NUM_P++;
		}

		// update mirror handle
		if (isPrevHandle == false)
		{
			isPrevHandle = true;
		}
		prevHandle[0] = e[0] + (e[0] - h2[0]);
		prevHandle[1] = e[1] + (e[1] - h2[1]);

	}

	//printf("c\n");
}


// Bezier path S
void SVG_Loader_Class::path_S(char** charPtr)
{
	int DIV = CURVE_DIV;

	// COUNT is the number of "value", not "xy pair"
	int COUNT = 0;
	double value[1000] = { 0.0 };

	// find value until next alphabet emerge
	COUNT = this->find_value_until_next_alphabet(charPtr, &value[0]);

	// add bezier curve (absolute)
	for (int i = 0; i < COUNT; i += 4)
	{
		if (isFirstPoint)
		{
			firstPoint[0] = CURRENT_POS[0];
			firstPoint[1] = CURRENT_POS[1];
			isFirstPoint = false;
		}

		double s[2];
		double h1[2];
		double h2[2];
		double e[2];
		//s
		s[0] = CURRENT_POS[0];
		s[1] = CURRENT_POS[1];
		//h1
		if (isPrevHandle)
		{
			h1[0] = prevHandle[0];
			h1[1] = prevHandle[1];
		}
		else
		{
			h1[0] = CURRENT_POS[0];
			h1[1] = CURRENT_POS[1];
		}
		// h2
		h2[0] = value[i];
		h2[1] = value[i + 1];
		//e
		e[0] = value[i + 2];
		e[1] = value[i + 3];

		// set pVert
		for (int t = 0; t < DIV; t++)
		{
			// point 1
			double coef = (double)t / (double)DIV;
			double A = (1.0 - coef)*(1.0 - coef)*(1.0 - coef);
			double B = 3.0*(1.0 - coef)*(1.0 - coef)*coef;
			double C = 3.0*(1.0 - coef)*coef*coef;
			double D = coef * coef*coef;

			pVert[NUM_P][0] = A * s[0] + B * h1[0] + C * h2[0] + D * e[0];
			pVert[NUM_P][1] = A * s[1] + B * h1[1] + C * h2[1] + D * e[1];
			NUM_P++;

			// point 2
			coef = (double)(t + 1) / (double)DIV;
			A = (1.0 - coef)*(1.0 - coef)*(1.0 - coef);
			B = 3.0*(1.0 - coef)*(1.0 - coef)*coef;
			C = 3.0*(1.0 - coef)*coef*coef;
			D = coef * coef*coef;

			pVert[NUM_P][0] = A * s[0] + B * h1[0] + C * h2[0] + D * e[0];
			pVert[NUM_P][1] = A * s[1] + B * h1[1] + C * h2[1] + D * e[1];
			NUM_P++;
		}


		// update CURRENT POS
		CURRENT_POS[0] = e[0];
		CURRENT_POS[1] = e[1];
		// update prevHandle
		if (isPrevHandle == false)
		{
			isPrevHandle = true;
		}
		prevHandle[0] = e[0] + (e[0] - h2[0]);
		prevHandle[1] = e[1] + (e[1] - h2[1]);
	}
}


void SVG_Loader_Class::path_s(char** charPtr)
{
	int DIV = CURVE_DIV;

	// COUNT is the number of "value", not "xy pair"
	int COUNT = 0;
	double value[1000] = { 0.0 };

	// find value until next alphabet emerge
	COUNT = this->find_value_until_next_alphabet(charPtr, &value[0]);

	// add bezier curve (relative)
	for (int i = 0; i < COUNT; i += 4)
	{
		if (isFirstPoint)
		{
			firstPoint[0] = CURRENT_POS[0];
			firstPoint[1] = CURRENT_POS[1];
			isFirstPoint = false;
		}

		double s[2];
		double h1[2];
		double h2[2];
		double e[2];
		//s
		s[0] = CURRENT_POS[0];
		s[1] = CURRENT_POS[1];
		//h1
		if (isPrevHandle)
		{
			h1[0] = prevHandle[0];
			h1[1] = prevHandle[1];
		}
		else
		{
			h1[0] = CURRENT_POS[0];
			h1[1] = CURRENT_POS[1];
		}
		// h2
		h2[0] = s[0] + value[i];
		h2[1] = s[1] + value[i + 1];
		//e
		e[0] = s[0] + value[i + 2];
		e[1] = s[1] + value[i + 3];


		// set pVert
		for (int t = 0; t < DIV; t++)
		{
				// point 1
				double coef = (double)t / (double)DIV;
				double A = (1.0 - coef)*(1.0 - coef)*(1.0 - coef);
				double B = 3.0*(1.0 - coef)*(1.0 - coef)*coef;
				double C = 3.0*(1.0 - coef)*coef*coef;
				double D = coef * coef*coef;

				pVert[NUM_P][0] = A * s[0] + B * h1[0] + C * h2[0] + D * e[0];
				pVert[NUM_P][1] = A * s[1] + B * h1[1] + C * h2[1] + D * e[1];
				NUM_P++;

				// point 2
				coef = (double)(t + 1) / (double)DIV;
				A = (1.0 - coef)*(1.0 - coef)*(1.0 - coef);
				B = 3.0*(1.0 - coef)*(1.0 - coef)*coef;
				C = 3.0*(1.0 - coef)*coef*coef;
				D = coef * coef*coef;

				pVert[NUM_P][0] = A * s[0] + B * h1[0] + C * h2[0] + D * e[0];
				pVert[NUM_P][1] = A * s[1] + B * h1[1] + C * h2[1] + D * e[1];
				NUM_P++;
		
		}

		// update current pos
		CURRENT_POS[0] = e[0];
		CURRENT_POS[1] = e[1];
		// update prevHandle
		if (isPrevHandle == false)
		{
			isPrevHandle = true;
		}
		prevHandle[0] = e[0] + (e[0] - h2[0]);
		prevHandle[1] = e[1] + (e[1] - h2[1]);

	}
}



void SVG_Loader_Class::path_Q(char** charPtr)
{
	int DIV = CURVE_DIV;

	// COUNT is the number of "value", not "xy pair"
	int COUNT = 0;
	double value[1000] = { 0.0 };

	// find value until next alphabet emerge
	COUNT = this->find_value_until_next_alphabet(charPtr, &value[0]);

	// add 2 bezier curve
	for (int i = 0; i < COUNT; i+=4)
	{
		if (isFirstPoint)
		{
			firstPoint[0] = CURRENT_POS[0];
			firstPoint[1] = CURRENT_POS[1];
			isFirstPoint = false;
		}

		double s[2];
		double h[2];
		double e[2];

		s[0] = CURRENT_POS[0];
		s[1] = CURRENT_POS[1];
		h[0] = value[i];
		h[1] = value[i + 1];
		e[0] = value[i + 2];
		e[1] = value[i + 3];

		// set pVert
		for (int t = 0; t < DIV; t++)
		{
			double coef = (double)t / (double)DIV;
			double A = (1.0 - coef) * (1.0 - coef);
			double B = 2.0 * (1.0 - coef) * coef;
			double C = coef * coef;

			// point 1
			pVert[NUM_P][0] = A * s[0] + B * h[0] + C * e[0];
			pVert[NUM_P][1] = A * s[1] + B * h[1] + C * e[1];
			NUM_P++;

			coef = (double)(t+1) / (double)DIV;
			A = (1.0 - coef) * (1.0 - coef);
			B = 2.0 * (1.0 - coef) * coef;
			C = coef * coef;

			// point 2
			pVert[NUM_P][0] = A * s[0] + B * h[0] + C * e[0];
			pVert[NUM_P][1] = A * s[1] + B * h[1] + C * e[1];
			NUM_P++;
		}


		// update CURRENT POS
		CURRENT_POS[0] = e[0];
		CURRENT_POS[1] = e[1];
		// update prev handle
		if (isPrevHandle == false)
		{
			isPrevHandle = true;
		}
		prevHandle[0] = e[0] + (e[0] - h[0]);
		prevHandle[1] = e[1] + (e[1] - h[1]);
	}
}


void SVG_Loader_Class::path_q(char** charPtr)
{
	int DIV = CURVE_DIV;

	// COUNT is the number of "value", not "xy pair"
	int COUNT = 0;
	double value[1000] = { 0.0 };

	// find value until next alphabet emerge
	COUNT = this->find_value_until_next_alphabet(charPtr, &value[0]);

	// add 2 bezier curve
	for (int i = 0; i < COUNT; i += 4)
	{
		if (isFirstPoint)
		{
			firstPoint[0] = CURRENT_POS[0];
			firstPoint[1] = CURRENT_POS[1];
			isFirstPoint = false;
		}

		double s[2];
		double h[2];
		double e[2];

		s[0] = CURRENT_POS[0];
		s[1] = CURRENT_POS[1];
		h[0] = s[0] + value[i];
		h[1] = s[1] + value[i + 1];
		e[0] = s[0] + value[i + 2];
		e[1] = s[1] + value[i + 3];

		// set pVert
		for (int t = 0; t < DIV; t++)
		{
			double coef = (double)t / (double)DIV;
			double A = (1.0 - coef) * (1.0 - coef);
			double B = 2.0 * (1.0 - coef) * coef;
			double C = coef * coef;

			// point 1
			pVert[NUM_P][0] = A * s[0] + B * h[0] + C * e[0];
			pVert[NUM_P][1] = A * s[1] + B * h[1] + C * e[1];
			NUM_P++;

			coef = (double)(t + 1) / (double)DIV;
			A = (1.0 - coef) * (1.0 - coef);
			B = 2.0 * (1.0 - coef) * coef;
			C = coef * coef;

			// point 2
			pVert[NUM_P][0] = A * s[0] + B * h[0] + C * e[0];
			pVert[NUM_P][1] = A * s[1] + B * h[1] + C * e[1];
			NUM_P++;
		}

		// update CURRENT POS
		CURRENT_POS[0] = e[0];
		CURRENT_POS[1] = e[1];
		// update prev handle
		if (isPrevHandle == false)
		{
			isPrevHandle = true;
		}
		prevHandle[0] = e[0] + (e[0] - h[0]);
		prevHandle[1] = e[1] + (e[1] - h[1]);
	}
}


void SVG_Loader_Class::path_T(char** charPtr)
{
	int DIV = CURVE_DIV;

	// COUNT is the number of "value", not "xy pair"
	int COUNT = 0;
	double value[1000] = { 0.0 };

	// find value until next alphabet emerge
	COUNT = this->find_value_until_next_alphabet(charPtr, &value[0]);

	// add 2 bezier curve
	for (int i = 0; i < COUNT; i += 2)
	{
		if (isFirstPoint)
		{
			firstPoint[0] = CURRENT_POS[0];
			firstPoint[1] = CURRENT_POS[1];
			isFirstPoint = false;
		}

		double s[2];
		double h[2];
		double e[2];

		s[0] = CURRENT_POS[0];
		s[1] = CURRENT_POS[1];

		if (isPrevHandle)
		{
			h[0] = prevHandle[0];
			h[1] = prevHandle[1];
		}
		else
		{
			h[0] = s[0];
			h[1] = s[1];
		}

		e[0] = value[i];
		e[1] = value[i + 1];

		// set pVert
		for (int t = 0; t < DIV; t++)
		{
			double coef = (double)t / (double)DIV;
			double A = (1.0 - coef) * (1.0 - coef);
			double B = 2.0 * (1.0 - coef) * coef;
			double C = coef * coef;

			// point 1
			pVert[NUM_P][0] = A * s[0] + B * h[0] + C * e[0];
			pVert[NUM_P][1] = A * s[1] + B * h[1] + C * e[1];
			NUM_P++;

			coef = (double)(t + 1) / (double)DIV;
			A = (1.0 - coef) * (1.0 - coef);
			B = 2.0 * (1.0 - coef) * coef;
			C = coef * coef;

			// point 2
			pVert[NUM_P][0] = A * s[0] + B * h[0] + C * e[0];
			pVert[NUM_P][1] = A * s[1] + B * h[1] + C * e[1];
			NUM_P++;
		}

		// update CURRENT POS
		CURRENT_POS[0] = e[0];
		CURRENT_POS[1] = e[1];
		// update prev handle
		if (isPrevHandle == false)
		{
			isPrevHandle = true;
		}
		prevHandle[0] = e[0] + (e[0] - h[0]);
		prevHandle[1] = e[1] + (e[1] - h[1]);
	}
}


void SVG_Loader_Class::path_t(char** charPtr)
{
	int DIV = CURVE_DIV;

	// COUNT is the number of "value", not "xy pair"
	int COUNT = 0;
	double value[1000] = { 0.0 };

	// find value until next alphabet emerge
	COUNT = this->find_value_until_next_alphabet(charPtr, &value[0]);

	// add 2 bezier curve
	for (int i = 0; i < COUNT; i += 2)
	{
		if (isFirstPoint)
		{
			firstPoint[0] = CURRENT_POS[0];
			firstPoint[1] = CURRENT_POS[1];
			isFirstPoint = false;
		}

		double s[2];
		double h[2];
		double e[2];

		s[0] = CURRENT_POS[0];
		s[1] = CURRENT_POS[1];

		if (isPrevHandle)
		{
			h[0] = prevHandle[0];
			h[1] = prevHandle[1];
		}
		else
		{
			h[0] = s[0];
			h[1] = s[1];
		}

		e[0] = s[0] + value[i];
		e[1] = s[1] + value[i + 1];

		// set pVert
		for (int t = 0; t < DIV; t++)
		{
			double coef = (double)t / (double)DIV;
			double A = (1.0 - coef) * (1.0 - coef);
			double B = 2.0 * (1.0 - coef) * coef;
			double C = coef * coef;

			// point 1
			pVert[NUM_P][0] = A * s[0] + B * h[0] + C * e[0];
			pVert[NUM_P][1] = A * s[1] + B * h[1] + C * e[1];
			NUM_P++;

			coef = (double)(t + 1) / (double)DIV;
			A = (1.0 - coef) * (1.0 - coef);
			B = 2.0 * (1.0 - coef) * coef;
			C = coef * coef;

			// point 2
			pVert[NUM_P][0] = A * s[0] + B * h[0] + C * e[0];
			pVert[NUM_P][1] = A * s[1] + B * h[1] + C * e[1];
			NUM_P++;
		}

		// update CURRENT POS
		CURRENT_POS[0] = e[0];
		CURRENT_POS[1] = e[1];
		// update prev handle
		if (isPrevHandle == false)
		{
			isPrevHandle = true;
		}
		prevHandle[0] = e[0] + (e[0] - h[0]);
		prevHandle[1] = e[1] + (e[1] - h[1]);
	}
}








int SVG_Loader_Class::find_value_until_next_alphabet(char** readPtr, double* vPtr)
{
	char* rPtr = *readPtr;
	double* writeDouble = vPtr;
	int counter = 0;

	while (1)
	{
		bool isEnd = false;
		int NUM = 0;
		char readBuf[100] = { '\0' };

		while (1)
		{
			readBuf[NUM] = *rPtr;
			rPtr++;

			if (readBuf[NUM] == ',' || readBuf[NUM] == ' ')
			{
				// find num value
				*writeDouble = atof(readBuf);

				// shift pointer of double value buffer
				writeDouble++;

				// num value Counter, += 1
				counter++;

				break;
			}
			else if (NUM != 0 && readBuf[NUM] == '-')
			{
				// find num value
				*writeDouble = atof(readBuf);

				// shift pointer of double value buffer
				writeDouble++;

				// num value Counter, += 1
				counter++;

				// rewind rPtr
				rPtr--;

				break;
			}
			else if (readBuf[NUM] >= 65 || readBuf[NUM] == '"') // >= 65 is Alphabet
			{
				// find final value
				isEnd = true;
				*writeDouble = atof(readBuf);

				// shift pointer of double value buffer
				writeDouble++;

				// num value Counter, += 1
				counter++;
				break;
			}

			NUM++;
		}// inner while

		if (isEnd)
		{
			break;
		}
	}// outer while

	return counter;
}