// Usage: Drag with the mouse to add smoke to the fluid. This will also move a "rotor" that disturbs 
//        the velocity field at the mouse location. Press the indicated keys to change options
//-------------------------------------------------------------------------------------------------- 



#include <rfftw.h>              //the numerical simulation FFTW library
#include <GL/glut.h>            //the GLUT graphics library
#include <stdio.h>              //for printing the help text
#include <math.h>

#include "hsvargb.h"
#include "divergenceAxis.h"
#include "isolinesHelper.h"
//#include "WTPartialDerivate.cpp"


//--- SIMULATION PARAMETERS ------------------------------------------------------------------------
const int DIM = 50;				//size of simulation grid
double dt = 0.4;				//simulation time step
float visc = 0.001;				//fluid viscosity
fftw_real *vx, *vy;             //(vx,vy)   = velocity field at the current moment
fftw_real *vx0, *vy0;           //(vx0,vy0) = velocity field at the previous moment
fftw_real *fx, *fy;	            //(fx,fy)   = user-controlled simulation forces, steered with the mouse 
fftw_real *rho, *rho0;			//smoke density at the current (rho) and previous (rho0) moment 
rfftwnd_plan plan_rc, plan_cr;  //simulation domain discretization
float *rainbowColors, *grayColors, *byColors; // ranbow colors, grayscale colors and yellow to blue colors

//--- VISUALIZATION PARAMETERS ---------------------------------------------------------------------
int   winWidth, winHeight;      //size of the graphics window, in pixels
int   color_dir = 0;            //use direction color-coding or not
float vec_scale = 1000;			//scaling of hedgehogs
int   draw_smoke = 0;           //draw the smoke or not
int   draw_vecs = 1;            //draw the vector field or not

const int DRAW_DIV_VELOCITY = 1; // draw the divergence of velocity
const int DRAW_DIV_FORCE = 2;   // draw the divergence of force
int   draw_divergence = 0;      //draw the divergence of grids

const int DRAW_ISOLINE_ONE = 1; // only draw one isoline
const int DRAW_ISOLINE_N = 2; // draw many isolines
int draw_isoline = 0; // dedicate if the app is going to draw isolines or isoline
float isoline_value; // if only draw one isoline, then use this value
float isoline_start_value = 1.0; // if draw many isolines, then start with this value
float isoline_end_value = 3.0; // if draw many isolines, then end with this value
int isoline_n = 3; // if draw many isolines, then the n is this value

const int COLOR_BLACKWHITE = 0;   //different types of color mapping: black-and-white, rainbow, banded
const int COLOR_RAINBOW = 1;
const int COLOR_BANDS = 2;
int   scalar_col = 0;           //method for scalar coloring
int   frozen = 0;               //toggles on/off the animation
int barColorSelected = 0;

int mainWindow, colorbarWindow, textbarWindow;

float hbias, sbias, vbias; // The bias each should between -0.5 to 0.5;
int colorNumbers;


int glOrtho_xmin = 0;
int glOrtho_xmax = 0;
int glOrtho_ymin = 0;
int glOrtho_ymax = 0;

int draw_3D = 0; // if draw height plot 
int wWidth = 500, wHeight = 500;
int xFar = 0.0f, yFar = 0.0f, zFar = 0.0f;
double mMatrix[16];
double datas[400];
int HEIGHT_PLOT_VELOCITY = 1;
int HEIGHT_PLOT_RHO = 2;
int HEIGHT_PLOT_FORCE = 3;
int height_plot_type = 3;

//------ SIMULATION CODE STARTS HERE -----------------------------------------------------------------

//init_simulation: Initialize simulation data structures as a function of the grid size 'n'. 
//                 Although the simulation takes place on a 2D grid, we allocate all data structures as 1D arrays,
//                 for compatibility with the FFTW numerical library.
void init_simulation(int n)
{
	int i; size_t dim;

	dim = n * 2 * (n / 2 + 1) * sizeof(fftw_real);        //Allocate data structures
	vx = (fftw_real*)malloc(dim);
	vy = (fftw_real*)malloc(dim);
	vx0 = (fftw_real*)malloc(dim);
	vy0 = (fftw_real*)malloc(dim);
	dim = n * n * sizeof(fftw_real);
	fx = (fftw_real*)malloc(dim);
	fy = (fftw_real*)malloc(dim);
	rho = (fftw_real*)malloc(dim);
	rho0 = (fftw_real*)malloc(dim);
	plan_rc = rfftw2d_create_plan(n, n, FFTW_REAL_TO_COMPLEX, FFTW_IN_PLACE);
	plan_cr = rfftw2d_create_plan(n, n, FFTW_COMPLEX_TO_REAL, FFTW_IN_PLACE);
	scalar_col = COLOR_RAINBOW;

	rainbowColors = (float *)malloc(256);
	grayColors = (float *)malloc(256);
	byColors = (float *)malloc(256);
	hbias = 0.0;
	sbias = 0.0;
	vbias = 0.0;
	colorNumbers = 256;

	for (size_t i = 0; i < 256; i++)
	{
		float value = (float)i / 256.0;


	}


	for (i = 0; i < n * n; i++)                      //Initialize data structures to 0
	{
		vx[i] = vy[i] = vx0[i] = vy0[i] = fx[i] = fy[i] = rho[i] = rho0[i] = 0.0f;
	}
}


//FFT: Execute the Fast Fourier Transform on the dataset 'vx'.
//     'dirfection' indicates if we do the direct (1) or inverse (-1) Fourier Transform
void FFT(int direction, void* vx)
{
	if (direction == 1) rfftwnd_one_real_to_complex(plan_rc, (fftw_real*)vx, (fftw_complex*)vx);
	else             rfftwnd_one_complex_to_real(plan_cr, (fftw_complex*)vx, (fftw_real*)vx);
}

int clamp(float x)
{
	return ((x) >= 0.0 ? ((int)(x)) : (-((int)(1 - (x)))));
}

//solve: Solve (compute) one step of the fluid flow simulation
void solve(int n, fftw_real* vx, fftw_real* vy, fftw_real* vx0, fftw_real* vy0, fftw_real visc, fftw_real dt)
{
	fftw_real x, y, x0, y0, f, r, U[2], V[2], s, t;
	int i, j, i0, j0, i1, j1;

	for (i = 0; i < n*n; i++)
	{
		vx[i] += dt * vx0[i]; vx0[i] = vx[i]; vy[i] += dt * vy0[i]; vy0[i] = vy[i];
	}

	for (x = 0.5f / n, i = 0; i < n; i++, x += 1.0f / n)
		for (y = 0.5f / n, j = 0; j < n; j++, y += 1.0f / n)
		{
			x0 = n * (x - dt * vx0[i + n * j]) - 0.5f;
			y0 = n * (y - dt * vy0[i + n * j]) - 0.5f;
			i0 = clamp(x0); s = x0 - i0;
			i0 = (n + (i0%n)) % n;
			i1 = (i0 + 1) % n;
			j0 = clamp(y0); t = y0 - j0;
			j0 = (n + (j0%n)) % n;
			j1 = (j0 + 1) % n;
			vx[i + n * j] = (1 - s)*((1 - t)*vx0[i0 + n * j0] + t * vx0[i0 + n * j1]) + s * ((1 - t)*vx0[i1 + n * j0] + t * vx0[i1 + n * j1]);
			vy[i + n * j] = (1 - s)*((1 - t)*vy0[i0 + n * j0] + t * vy0[i0 + n * j1]) + s * ((1 - t)*vy0[i1 + n * j0] + t * vy0[i1 + n * j1]);
		}

	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
		{
			vx0[i + (n + 2)*j] = vx[i + n * j]; vy0[i + (n + 2)*j] = vy[i + n * j];
		}

	FFT(1, vx0);
	FFT(1, vy0);

	for (i = 0; i <= n; i += 2)
	{
		x = 0.5f*i;
		for (j = 0; j < n; j++)
		{
			y = j <= n / 2 ? (fftw_real)j : (fftw_real)j - n;
			r = x * x + y * y;
			if (r == 0.0f) continue;
			f = (fftw_real)exp(-r * dt*visc);
			U[0] = vx0[i + (n + 2)*j]; V[0] = vy0[i + (n + 2)*j];
			U[1] = vx0[i + 1 + (n + 2)*j]; V[1] = vy0[i + 1 + (n + 2)*j];

			vx0[i + (n + 2)*j] = f * ((1 - x * x / r)*U[0] - x * y / r * V[0]);
			vx0[i + 1 + (n + 2)*j] = f * ((1 - x * x / r)*U[1] - x * y / r * V[1]);
			vy0[i + (n + 2)*j] = f * (-y * x / r * U[0] + (1 - y * y / r)*V[0]);
			vy0[i + 1 + (n + 2)*j] = f * (-y * x / r * U[1] + (1 - y * y / r)*V[1]);
		}
	}

	FFT(-1, vx0);
	FFT(-1, vy0);

	f = 1.0 / (n*n);
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
		{
			vx[i + n * j] = f * vx0[i + (n + 2)*j]; vy[i + n * j] = f * vy0[i + (n + 2)*j];
		}
}


// diffuse_matter: This function diffuses matter that has been placed in the velocity field. It's almost identical to the
// velocity diffusion step in the function above. The input matter densities are in rho0 and the result is written into rho.
void diffuse_matter(int n, fftw_real *vx, fftw_real *vy, fftw_real *rho, fftw_real *rho0, fftw_real dt)
{
	fftw_real x, y, x0, y0, s, t;
	int i, j, i0, j0, i1, j1;

	for (x = 0.5f / n, i = 0; i < n; i++, x += 1.0f / n)
		for (y = 0.5f / n, j = 0; j < n; j++, y += 1.0f / n)
		{
			x0 = n * (x - dt * vx[i + n * j]) - 0.5f;
			y0 = n * (y - dt * vy[i + n * j]) - 0.5f;
			i0 = clamp(x0);
			s = x0 - i0;
			i0 = (n + (i0%n)) % n;
			i1 = (i0 + 1) % n;
			j0 = clamp(y0);
			t = y0 - j0;
			j0 = (n + (j0%n)) % n;
			j1 = (j0 + 1) % n;
			rho[i + n * j] = (1 - s)*((1 - t)*rho0[i0 + n * j0] + t * rho0[i0 + n * j1]) + s * ((1 - t)*rho0[i1 + n * j0] + t * rho0[i1 + n * j1]);
		}
}

//set_forces: copy user-controlled forces to the force vectors that are sent to the solver. 
//            Also dampen forces and matter density to get a stable simulation.
void set_forces(void)
{
	int i;
	for (i = 0; i < DIM * DIM; i++)
	{
		rho0[i] = 0.995 * rho[i];
		fx[i] *= 0.85;
		fy[i] *= 0.85;
		vx0[i] = fx[i];
		vy0[i] = fy[i];
	}
}


//------ VISUALIZATION CODE STARTS HERE -----------------------------------------------------------------

// ==== These two functions only for height plot 

void makeRolate()
{
	glMultMatrixd(mMatrix);
}

void resize()
{
	glGetDoublev(GL_MODELVIEW_MATRIX, mMatrix);  //
}


// ====

// Using this function we can get the partial derivative of a cell.
void calculatePartialDerivate(float v1, float v2, float v3, float v4, float width, float height, float *rhox, float *rhoy) {

	float r = 0.5;
	float s = 0.5;

	*rhox = ((v2 - v1) * r) / width + (v3 - v4) * (1 - r) / width;
	*rhoy = (v3 - v2) * s / height + (v4 - v1) * (1 - s) / height;
}


//rainbow: Implements a color palette, mapping the scalar 'value' to a rainbow color RGB
void rainbow(float value, float* R, float* G, float* B)
{
	const float dx = 0.8;
	if (value < 0) value = 0; if (value > 1) value = 1;

	float raw = floorf(value * 256.0);
	float fValue = raw / 256.0;


	for (float i = 1.0; i >= 0.0; i -= 1.0 / (float)colorNumbers)
	{
		if ((fValue - i) > 0)
		{
			fValue = i;
		}
	}

	fValue = (6 - 2 * dx)*fValue + dx;
	*R = max(0.0, (3 - fabs(fValue - 4) - fabs(fValue - 5)) / 2);
	*G = max(0.0, (4 - fabs(fValue - 2) - fabs(fValue - 4)) / 2);
	*B = max(0.0, (3 - fabs(fValue - 1) - fabs(fValue - 2)) / 2);

	float h, s, v;
}

void biasRainbow(float value, float huebias, float saturationbias, float vbias, float* R, float* G, float* B) {


	value = value / 2.0;

	const float dx = 0.8;
	if (value < 0) value = 0; if (value > 1) value = 1;
	float fValue = value;

	int numberth = fValue * colorNumbers;
	fValue = (float)numberth * (1.0 / (float)colorNumbers);

	//printf("the fvalue is %s \n", fValue);

	fValue = (6 - 2 * dx)*fValue + dx;


	float preR = fmax(0.0, (3 - fabs(fValue - 4) - fabs(fValue - 5)) / 2);
	float preG = fmax(0.0, (4 - fabs(fValue - 2) - fabs(fValue - 4)) / 2);
	float preB = fmax(0.0, (3 - fabs(fValue - 1) - fabs(fValue - 2)) / 2);

	float h, s, v;

	rgb2hsv(preR, preG, preB, &h, &s, &v);

	h += huebias;
	s += saturationbias;
	v += vbias;


	float fR, fG, fB;

	hsv2rgb(h, s, v, &fR, &fG, &fB);

	*R = fR;
	*G = fG;
	*B = fB;
}

void grayscale(float value, float *R, float *G, float *B) {

	value = value / 3.0;
	const float dx = 0.8;
	if (value < 0) value = 0.001; if (value > 1) value = 0.999;

	float fValue = value;

	int numberth = fValue * colorNumbers;
	fValue = (float)numberth * (1.0 / (float)colorNumbers);

	// 0.2126R + 0.7152G + 0.0722B
	/*
	*R = 0.2126 * preR + 0.7152 * preG + 0.0722 * preB;
	*G = 0.2126 * preR + 0.7152 * preG + 0.0722 * preB;
	*R = 0.2126 * preR + 0.7152 * preG + 0.0722 * preB;
	*/
	*R = fValue;
	*G = fValue;
	*B = fValue;
}

void yellowToBlue(float value, float *R, float *G, float *B) {

	value = value / 3.0;
	const int color_number = 2;
	static float color[2][3] = { {0,0,1}, {1,1,0} };
	int idx1, idx2;
	float fracBetween = 0;
	if (value < 0) value = 0.001; if (value >= 1) value = 0.999;

	idx1 = floor(value);
	idx2 = idx1 + 1;

	float fValue = value;

	int numberth = fValue * colorNumbers;
	fValue = (float)numberth * (1.0 / (float)colorNumbers);

	fracBetween = fValue;

	*R = ((color[idx2][0] - color[idx1][0])*fracBetween + color[idx1][0]);
	*G = ((color[idx2][1] - color[idx1][1])*fracBetween + color[idx1][1]);
	*B = ((color[idx2][2] - color[idx1][2])*fracBetween + color[idx1][2]);
}

//set_colormap: Sets three different types of colormaps
void set_colormap(float vy)
{
	float R, G, B;

	if (scalar_col == COLOR_BLACKWHITE) {
		//R = G = B = vy;
		grayscale(vy, &R, &G, &B);
	}

	else if (scalar_col == COLOR_RAINBOW) {
		//rainbow(vy, &R, &G, &B);
		biasRainbow(vy, hbias, sbias, vbias, &R, &G, &B);
	}

	else if (scalar_col == COLOR_BANDS)
	{

		/*
		const int NLEVELS = 7;
		vy *= NLEVELS; vy = (int)(vy); vy/= NLEVELS;
		rainbow(vy,&R,&G,&B);
		*/
		yellowToBlue(vy, &R, &G, &B);
	}

	glColor3f(R, G, B);
}


//direction_to_color: Set the current color by mapping a direction vector (x,y), using
//                    the color mapping method 'method'. If method==1, map the vector direction
//                    using a rainbow colormap. If method==0, simply use the white color
void direction_to_color(float x, float y, int method)
{
	float r, g, b, f;
	if (method)
	{
		f = atan2(y, x) / 3.1415927 + 1;
		r = f;
		if (r > 1) r = 2 - r;
		g = f + .66667;
		if (g > 2) g -= 2;
		if (g > 1) g = 2 - g;
		b = f + 2 * .66667;
		if (b > 2) b -= 2;
		if (b > 1) b = 2 - b;
	}
	else
	{
		r = g = b = 1;
	}
	glColor3f(r, g, b);
}

//visualize: This is the main visualization function
void visualize(void)
{

	if (draw_3D == 1)
	{
		// draw the 3d pictures
		int        i, j, idx;
		fftw_real  wn = (fftw_real)winWidth / (fftw_real)(DIM + 1);   // Grid cell width
		fftw_real  hn = (fftw_real)winHeight / (fftw_real)(DIM + 1);  // Grid cell heigh

		int idx0, idx1, idx2, idx3;
		double px0, py0, px1, py1, px2, py2, px3, py3;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBegin(GL_TRIANGLES);


		for (j = 0; j < DIM - 1; j++)            //draw smoke  
		{
			for (i = 0; i < DIM - 1; i++) //DIM - 1
			{
				px0 = wn + (fftw_real)i * wn;
				py0 = hn + (fftw_real)j * hn;
				idx0 = (j * DIM) + i;


				px1 = wn + (fftw_real)i * wn;
				py1 = hn + (fftw_real)(j + 1) * hn;
				idx1 = ((j + 1) * DIM) + i;


				px2 = wn + (fftw_real)(i + 1) * wn;
				py2 = hn + (fftw_real)(j + 1) * hn;
				idx2 = ((j + 1) * DIM) + (i + 1);


				px3 = wn + (fftw_real)(i + 1) * wn;
				py3 = hn + (fftw_real)j * hn;
				idx3 = (j * DIM) + (i + 1);

				float s = 10.0;
				float vs = 4000.0;
				float fs = 500.0;

				if (height_plot_type == HEIGHT_PLOT_RHO)
				{

					glColor3f(0.0, 1.0, 0.0); glVertex3f(px0, py0, s *rho[idx0]);
					glColor3f(0.0, 1.0, 0.0); glVertex3f(px1, py1, s *rho[idx1]);
					glColor3f(0.0, 1.0, 0.0); glVertex3f(px2, py2, s *rho[idx2]);

					glColor3f(0.0, 1.0, 0.0); glVertex3f(px0, py0, s * rho[idx0]);
					glColor3f(0.0, 1.0, 0.0); glVertex3f(px2, py2, s * rho[idx2]);
					glColor3f(0.0, 1.0, 0.0); glVertex3f(px3, py3, s * rho[idx3]);
					/*
					*/
				}
				else if (height_plot_type == HEIGHT_PLOT_VELOCITY)
				{

					static float maxVelo = 0;
					float velo0 = sqrt(pow(vx[idx0], 2) + pow(vy[idx0], 2));
					float velo1 = sqrt(pow(vx[idx1], 2) + pow(vy[idx1], 2));
					float velo2 = sqrt(pow(vx[idx2], 2) + pow(vy[idx2], 2));
					float velo3 = sqrt(pow(vx[idx3], 2) + pow(vy[idx3], 2));

					if (velo0 > maxVelo)
					{
						maxVelo = velo0;
						//printf("the velocity is %f \n", velo0);
					}

					glColor3f(0.0, 1.0, 0.0); glVertex3f(px0, py0, vs * velo0);
					glColor3f(0.0, 1.0, 0.0); glVertex3f(px1, py1, vs * velo1);
					glColor3f(0.0, 1.0, 0.0); glVertex3f(px2, py2, vs * velo2);

					glColor3f(0.0, 1.0, 0.0); glVertex3f(px0, py0, vs * velo0);
					glColor3f(0.0, 1.0, 0.0); glVertex3f(px2, py2, vs * velo2);
					glColor3f(0.0, 1.0, 0.0); glVertex3f(px3, py3, vs * velo3);

				}
				else if(height_plot_type == HEIGHT_PLOT_FORCE)
				{
					static float maxForce = 0;
					float force0 = sqrt(pow(fx[idx0], 2) + pow(fy[idx0], 2));
					float force1 = sqrt(pow(fx[idx1], 2) + pow(fy[idx1], 2));
					float force2 = sqrt(pow(fx[idx2], 2) + pow(fy[idx2], 2));
					float force3 = sqrt(pow(fx[idx3], 2) + pow(fy[idx3], 2));

					if (force0 > maxForce)
					{
						maxForce = force0;
						//printf("the velocity is %f \n", maxForce);
					}

					glColor3f(0.0, 1.0, 0.0); glVertex3f(px0, py0, fs * force0);
					glColor3f(0.0, 1.0, 0.0); glVertex3f(px1, py1, fs * force1);
					glColor3f(0.0, 1.0, 0.0); glVertex3f(px2, py2, fs * force2);

					glColor3f(0.0, 1.0, 0.0); glVertex3f(px0, py0, fs * force0);
					glColor3f(0.0, 1.0, 0.0); glVertex3f(px2, py2, fs * force2);
					glColor3f(0.0, 1.0, 0.0); glVertex3f(px3, py3, fs * force3);
				}
			}
		}

		glEnd();
	}
	else
	{



		int        i, j, idx;
		fftw_real  wn = (fftw_real)winWidth / (fftw_real)(DIM + 1);   // Grid cell width
		fftw_real  hn = (fftw_real)winHeight / (fftw_real)(DIM + 1);  // Grid cell heigh

		if (draw_smoke)
		{
			int idx0, idx1, idx2, idx3;
			double px0, py0, px1, py1, px2, py2, px3, py3;
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glBegin(GL_TRIANGLES);
			for (j = 0; j < DIM - 1; j++)            //draw smoke
			{
				for (i = 0; i < DIM - 1; i++)
				{
					px0 = wn + (fftw_real)i * wn;
					py0 = hn + (fftw_real)j * hn;
					idx0 = (j * DIM) + i;


					px1 = wn + (fftw_real)i * wn;
					py1 = hn + (fftw_real)(j + 1) * hn;
					idx1 = ((j + 1) * DIM) + i;


					px2 = wn + (fftw_real)(i + 1) * wn;
					py2 = hn + (fftw_real)(j + 1) * hn;
					idx2 = ((j + 1) * DIM) + (i + 1);


					px3 = wn + (fftw_real)(i + 1) * wn;
					py3 = hn + (fftw_real)j * hn;
					idx3 = (j * DIM) + (i + 1);


					set_colormap(rho[idx0]);    glVertex2f(px0, py0);
					set_colormap(rho[idx1]);    glVertex2f(px1, py1);
					set_colormap(rho[idx2]);    glVertex2f(px2, py2);


					set_colormap(rho[idx0]);    glVertex2f(px0, py0);
					set_colormap(rho[idx2]);    glVertex2f(px2, py2);
					set_colormap(rho[idx3]);    glVertex2f(px3, py3);

					/*
					set_colormap(rho[idx0]);    glVertex3f(px0, py0, 3.0);
					set_colormap(rho[idx1]);    glVertex3f(px1, py1, 4.0);
					set_colormap(rho[idx2]);    glVertex3f(px2, py2, 1.0);


					set_colormap(rho[idx0]);    glVertex3f(px0, py0, 3.0);
					set_colormap(rho[idx2]);    glVertex3f(px2, py2, 4.0);
					set_colormap(rho[idx3]);    glVertex3f(px3, py3, 11.0);
					*/

				}
			}
			glEnd();
		}
		else
		{
			if (draw_divergence > 0)
			{
				int idx0, idx1, idx2, idx3;
				double px0, py0, px1, py1, px2, py2, px3, py3;

				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glBegin(GL_TRIANGLES);

				for (j = 0; j < DIM - 1; j++)
				{
					for (i = 0; i < DIM - 1; i++)
					{

						px0 = wn + (fftw_real)i * wn;
						py0 = hn + (fftw_real)j * hn;
						idx0 = (j * DIM) + i;

						px1 = wn + (fftw_real)i * wn;
						py1 = hn + (fftw_real)(j + 1) * hn;
						idx1 = ((j + 1) * DIM) + i;

						px2 = wn + (fftw_real)(i + 1) * wn;
						py2 = hn + (fftw_real)(j + 1) * hn;
						idx2 = ((j + 1) * DIM) + (i + 1);

						px3 = wn + (fftw_real)(i + 1) * wn;
						py3 = hn + (fftw_real)j * hn;
						idx3 = (j * DIM) + (i + 1);

						if (draw_divergence == DRAW_DIV_VELOCITY) {
							float div = 0;
							divInCell(vx[idx0], vy[idx0], vx[idx1], vy[idx1], vx[idx2], vy[idx2], vx[idx3], vy[idx3], wn, hn, &div);

							//printf("The div is %f \n", div);
							div = div * 10000;
							div = div + 0.5;

							float fvx = vx[idx0] + vx[idx1] + vx[idx2] + vx[idx3];
							float fvy = vy[idx0] + vy[idx1] + vy[idx2] + vy[idx3];
							float fx1, fy1, fx2, fy2, fx3, fy3;
							getCorrectCoordinates(px0, py0, hn, wn, fvx, fvy, &fx1, &fy1, &fx2, &fy2, &fx3, &fy3);

							set_colormap(div);    glVertex2f(fx1, fy1);
							set_colormap(div);    glVertex2f(fx2, fy2);
							set_colormap(div);    glVertex2f(fx3, fy3);

						}
						else if (draw_divergence == DRAW_DIV_FORCE) {
							float div = 0;
							divInCell(fx[idx0], fy[idx0], fx[idx1], fy[idx1], fx[idx2], fy[idx2], fx[idx3], fy[idx3], wn, hn, &div);

							div = div * 1000;
							div = div + 0.5;

							float fvx = fx[idx0] + fx[idx1] + fx[idx2] + fx[idx3];
							float fvy = fy[idx0] + fy[idx1] + fy[idx2] + fy[idx3];
							float fx1, fy1, fx2, fy2, fx3, fy3;
							getCorrectCoordinates(px0, py0, hn, wn, fvx, fvy, &fx1, &fy1, &fx2, &fy2, &fx3, &fy3);

							set_colormap(div);    glVertex2f(fx1, fy1);
							set_colormap(div);    glVertex2f(fx2, fy2);
							set_colormap(div);    glVertex2f(fx3, fy3);
						}
					}
				}

				glEnd();
			}
		}

		if (draw_vecs)
		{
			glBegin(GL_LINES);				//draw velocities
			for (i = 0; i < DIM; i++)
				for (j = 0; j < DIM; j++)
				{
					idx = (j * DIM) + i;
					direction_to_color(vx[idx], vy[idx], color_dir);
					glVertex2f(wn + (fftw_real)i * wn, hn + (fftw_real)j * hn);
					glVertex2f((wn + (fftw_real)i * wn) + vec_scale * vx[idx], (hn + (fftw_real)j * hn) + vec_scale * vy[idx]);
				}

			glEnd();
		}

		if (draw_isoline > 0)
		{
			int idx0, idx1, idx2, idx3;
			double px0, py0, px1, py1, px2, py2, px3, py3;

			isoline_value = 1.0;

			isoline_start_value = 1.0;
			isoline_end_value = 3.0;
			isoline_n = 3;

			glBegin(GL_LINES);
			for (j = 0; j < DIM - 1; j++)
			{
				for (i = 0; i < DIM - 1; i++)
				{
					px0 = wn + (fftw_real)i * wn;
					py0 = hn + (fftw_real)j * hn;
					idx0 = (j * DIM) + i;

					px1 = wn + (fftw_real)i * wn;
					py1 = hn + (fftw_real)(j + 1) * hn;
					idx1 = ((j + 1) * DIM) + i;

					px2 = wn + (fftw_real)(i + 1) * wn;
					py2 = hn + (fftw_real)(j + 1) * hn;
					idx2 = ((j + 1) * DIM) + (i + 1);

					px3 = wn + (fftw_real)(i + 1) * wn;
					py3 = hn + (fftw_real)j * hn;
					idx3 = (j * DIM) + (i + 1);

					if (draw_isoline == DRAW_ISOLINE_ONE)
					{
						float sepx1, sepx2, sepx3, sepx4, sepy1, sepy2, sepy3, sepy4;

						linePointsInCell(px0, py0, wn, hn, isoline_value, rho[idx0], rho[idx1], rho[idx2], rho[idx3], &sepx1, &sepy1, &sepx2, &sepy2, &sepx3, &sepy3, &sepx4, &sepy4);

						if (sepx1 > 0 && sepy1 > 0 && sepx2 > 0 && sepy2 > 0)
						{

							set_colormap(isoline_value);
							glVertex2f(sepx1, sepy1);
							glVertex2f(sepx2, sepy2);
						}

						if (sepx3 > 0 && sepy3 > 0 && sepx4 > 0 && sepy4 > 0)
						{
							set_colormap(isoline_value);
							glVertex2f(sepx3, sepy3);
							glVertex2f(sepx4, sepy4);
						}
					}
					else if (draw_isoline == DRAW_ISOLINE_N)
					{
						float interval = (isoline_end_value - isoline_start_value) / (float)(isoline_n - 1);

						float *values = (float *)malloc(isoline_n);

						int i = 0;
						while (i < sizeof(values))
						{
							values[i] = isoline_start_value + i * interval;
							i++;
						}

						//printf("Is executing herer! \n");
						multiLinesPointsInCell(px0, py0, wn, hn, rho[idx0], rho[idx1], rho[idx2], rho[idx3], values);
					}
				}
			}
			glEnd();

		}

	}
}




//------ INTERACTION CODE STARTS HERE -----------------------------------------------------------------

//display: Handle window redrawing events. Simply dwinWidthelegates to visualize().
void display(void)
{

	if (draw_3D == 1)
	{
		glClearColor(0, 0, 0.1f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glMatrixMode(GL_MODELVIEW);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		static int excuted = 0;
		if (excuted == 0)
		{
			printf("always rote?");
			glTranslatef(100, yFar, zFar);

			excuted++;
		}
		winHeight = winHeight > 0 ? winHeight : 1;
		float aspect = (float)winWidth / (float)winHeight;
		gluPerspective(45, 1.0, 1.0, 1500.0);
		glTranslatef(-250, -210, -610);
		//resize();
		//makeRolate();
		//glRotatef(45, 0, 0, 1); z
		//glRotatef(15, 1, 0, 0); x
		visualize();



		glFlush();
		glutSwapBuffers();  //
	}
	else
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		visualize();
		//gridDivergence();
		glFlush();
		glutSwapBuffers();
	}


}

void display1(void) {

	glutSetWindow(colorbarWindow);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//printf("the colo is %d \n", scalar_col);

	if (draw_vecs == 0 || draw_divergence == 1)
	{

		if (draw_divergence == 1)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glBegin(GL_QUADS);

			for (size_t i = 0; i < colorNumbers; i++)
			{
				float startx = ((float)i / (float)colorNumbers) * 2 - 1.0;
				float endx = startx + (float)i / (float)colorNumbers;

				set_colormap((float)i * 4.0 / (float)colorNumbers);
				glVertex2f(startx, 1.0);
				set_colormap((float)i * 4.0 / (float)colorNumbers);
				glVertex2f(startx, -1.0);
				set_colormap((float)(i + 1) * 4.0 / (float)colorNumbers);
				glVertex2f(endx, -1.0);
				set_colormap((float)(i + 1) * 4.0 / (float)colorNumbers);
				glVertex2f(endx, 1.0);
			}
		}
		else
		{

		

		if (scalar_col == COLOR_BANDS) { // blue to yellow

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glBegin(GL_QUADS);

			for (size_t i = 0; i < colorNumbers; i++)
			{
				float startx = ((float)i / (float)colorNumbers) * 2 - 1.0;
				float endx = startx + (float)i / (float)colorNumbers;

				set_colormap((float)i * 4.0 / (float)colorNumbers);
				glVertex2f(startx, 1.0);
				set_colormap((float)i * 4.0 / (float)colorNumbers);
				glVertex2f(startx, -1.0);
				set_colormap((float)(i + 1) * 4.0 / (float)colorNumbers);
				glVertex2f(endx, -1.0);
				set_colormap((float)(i + 1) * 4.0 / (float)colorNumbers);
				glVertex2f(endx, 1.0);
			}

			/*
			
			glColor3f(0.0f, 0.0f, 1.0f); // make this vertex blue
			glVertex2f(-1.0, 1.0);
			glColor3f(0.0f, 0.0f, 1.0f); // make this vertex blue
			glVertex2f(-1.0, -1.0);
			glColor3f(1.0f, 1.0f, 0.0f); // make this vertex yellow
			glVertex2f(1.0, -1.0);
			glColor3f(1.0f, 1.0f, 0.0f); // make this vertex yellow
			glVertex2f(1.0, 1.0);
			*/

		}
		else if (scalar_col == COLOR_BLACKWHITE) {

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glBegin(GL_QUADS);

			for (size_t i = 0; i < colorNumbers; i++)
			{
				float startx = ((float)i / (float)colorNumbers) * 2 - 1.0;
				float endx = startx + (float)i / (float)colorNumbers;

				set_colormap((float)i * 4.0 / (float)colorNumbers);
				glVertex2f(startx, 1.0);
				set_colormap((float)i * 4.0 / (float)colorNumbers);
				glVertex2f(startx, -1.0);
				set_colormap((float)(i + 1) * 4.0 / (float)colorNumbers);
				glVertex2f(endx, -1.0);
				set_colormap((float)(i + 1) * 4.0 / (float)colorNumbers);
				glVertex2f(endx, 1.0);
			}

			/*
			
			
			glColor3f(1.0f, 1.0f, 1.0f); // make this vertex 
			glVertex2f(-1.0, 1.0);
			glColor3f(1.0f, 1.0f, 1.0f); // make this vertex 
			glVertex2f(-1.0, -1.0);

			glColor3f(0.0f, 0.0f, 0.0f); // make this vertex 
			glVertex2f(1.0, -1.0);
			glColor3f(0.0f, 0.0f, 0.0f); // make this vertex 
			glVertex2f(1.0, 1.0);
			*/

		}
		else {

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glBegin(GL_QUADS);

			for (size_t i = 0; i < colorNumbers; i++)
			{
				float startx = ((float)i / (float)colorNumbers) * 2 - 1.0;
				float endx = startx + (float)i / (float)colorNumbers;

				set_colormap((float)i * 2.0 / (float)colorNumbers);
				glVertex2f(startx, 1.0);
				set_colormap((float)i * 2.0 / (float)colorNumbers);
				glVertex2f(startx, -1.0);
				set_colormap((float)(i + 1) * 2.0 / (float)colorNumbers);
				glVertex2f(endx, -1.0);
				set_colormap((float)(i + 1) * 2.0 / (float)colorNumbers);
				glVertex2f(endx, 1.0);
			}

			/*
			

			
			glColor3f(0.0f, 0.0f, 0.8f); // make this vertex 
			glVertex2f(-1.0, 1.0);
			glColor3f(0.0f, 0.0f, 0.8f); // make this vertex 
			glVertex2f(-1.0, -1.0);

			glColor3f(0.0f, 0.68f, 1.0f); // make this vertex 
			glVertex2f(-0.6, -1.0);
			glColor3f(0.0f, 0.68f, 1.0f); // make this vertex 
			glVertex2f(-0.6, 1.0);


			glColor3f(0.0f, 0.68f, 1.0f); // make this vertex 
			glVertex2f(-0.6, 1.0);
			glColor3f(0.0f, 0.68f, 1.0f); // make this vertex 
			glVertex2f(-0.6, -1.0);

			glColor3f(0.0f, 1.0f, 0.44f); // make this vertex 
			glVertex2f(-0.2, -1.0);
			glColor3f(0.0f, 1.0f, 0.44f); // make this vertex 
			glVertex2f(-0.2, 1.0);


			glColor3f(0.0f, 1.0f, 0.44f); // make this vertex 
			glVertex2f(-0.2, 1.0);
			glColor3f(0.0f, 1.0f, 0.44f); // make this vertex 
			glVertex2f(-0.2, -1.0);

			glColor3f(0.44f, 1.0f, 0.0f); // make this vertex 
			glVertex2f(0.2, -1.0);
			glColor3f(0.44f, 1.0f, 0.0f); // make this vertex 
			glVertex2f(0.2, 1.0);


			glColor3f(0.44f, 1.0f, 0.0f); // make this vertex 
			glVertex2f(0.2, -1.0);
			glColor3f(0.44f, 1.0f, 0.0f); // make this vertex 
			glVertex2f(0.2, 1.0);

			glColor3f(1.0f, 0.68f, 0.0f); // make this vertex 
			glVertex2f(0.6, 1.0);
			glColor3f(1.0f, 0.68f, 0.0f); // make this vertex 
			glVertex2f(0.6, -1.0);



			glColor3f(1.0f, 0.68f, 0.0f); // make this vertex 
			glVertex2f(0.6, 1.0);
			glColor3f(1.0f, 0.68f, 0.0f); // make this vertex 
			glVertex2f(0.6, -1.0);

			glColor3f(0.8f, 0.0f, 0.0f); // make this vertex 
			glVertex2f(1.0, -1.0);
			glColor3f(0.8f, 0.0f, 0.0f); // make this vertex 
			glVertex2f(1.0, 1.0);
			*/
		}
	}
	}

	glEnd();

	glutSwapBuffers();
}

void displayText(void) {


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 500.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(2, 2, 2, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glScalef(.005, .007, .005);
	glRotatef(0, 0, 1, 0);
	glRotatef(0, 0, 0, 1);
	glRotatef(0, 1, 0, 0);
	glTranslatef(-400, 0, 0);

	glColor3f(1, 1, 1);

	if (barColorSelected == 0)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, '0');
		glutStrokeCharacter(GLUT_STROKE_ROMAN, ' ');
		glutStrokeCharacter(GLUT_STROKE_ROMAN, ' ');
		glutStrokeCharacter(GLUT_STROKE_ROMAN, ' ');
		glutStrokeCharacter(GLUT_STROKE_ROMAN, ' ');
		glutStrokeCharacter(GLUT_STROKE_ROMAN, ' ');
		glutStrokeCharacter(GLUT_STROKE_ROMAN, ' ');
		glutStrokeCharacter(GLUT_STROKE_ROMAN, '1');
	}
	else
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, '0');

		float va = ((float)barColorSelected / 500.0);
		char buf[200];
		gcvt(va, 6, buf);

		for (size_t i = 0; i < 5; i++)
		{
			char cc = buf[i];
			glutStrokeCharacter(GLUT_STROKE_ROMAN, cc);

		}

		glutStrokeCharacter(GLUT_STROKE_ROMAN, '1');

	}




	glutSwapBuffers();
}

//reshape: Handle window resizing (reshaping) events
void reshape(int w, int h)
{
	glViewport(0.0f, 0.0f, (GLfloat)w, (GLfloat)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
	winWidth = w; winHeight = h;
}

//keyboard: Handle key presses
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 't': dt -= 0.001; break;
	case 'T': dt += 0.001; break;
	case 'c': color_dir = 1 - color_dir; break;
	case 'S': vec_scale *= 1.2; break;
	case 's': vec_scale *= 0.8; break;
	case 'V': visc *= 5; break;
	case 'vy': visc *= 0.2; break;
	case 'x': draw_smoke = 1 - draw_smoke;
		if (draw_smoke == 0) draw_vecs = 1; break;
	case 'y': draw_vecs = 1 - draw_vecs;
		if (draw_vecs == 0) draw_smoke = 1; break;
	case 'm': 
	
		scalar_col++; if (scalar_col > COLOR_BANDS) scalar_col = COLOR_BLACKWHITE; break;
	case 'a': frozen = 1 - frozen; break;
	case 'd': draw_divergence++; if (draw_divergence > 2) draw_divergence = 0; break;
	case 'i':draw_isoline++; if (draw_isoline > 2) draw_isoline = 0; break;
	case 'h': {
		if (draw_3D == 1)
		{
			height_plot_type = height_plot_type + 1;
			if (height_plot_type > 3)
			{
				height_plot_type = 1;
			}
		}
		break;
	}
	case '1': {
		
		if (draw_isoline == DRAW_ISOLINE_ONE)
		{
			isoline_value = 1.0;
		}

		hbias -= 0.01;
		if (hbias < -0.3)
		{
			hbias = 0;
		}
		break;
	} 
	case '2': {

		if (draw_isoline == DRAW_ISOLINE_ONE)
		{
			isoline_value = 2.0;
		}

		hbias += 0.01;
		if (hbias > 0.3)
		{
			hbias = 0;
		}
		break;
	}
	case '3': {

		if (draw_isoline == DRAW_ISOLINE_ONE)
		{
			isoline_value = 3.0;
		}

		sbias -= 0.01;
		if (sbias < -0.3)
		{
			sbias = 0;
		}
		break;
	}
	case '4': {

		if (draw_isoline == DRAW_ISOLINE_ONE)
		{
			isoline_value = 4.0;
		}

		sbias += 0.01;
		if (sbias > 0.3)
		{
			sbias = 0;
		}
		break;
	}
	case '5': {

		if (draw_isoline == DRAW_ISOLINE_ONE)
		{
			isoline_value = 5.0;
		}

		vbias -= 0.01;
		if (vbias < -0.3)
		{
			vbias = 0;
		}
		break;
	}
	case '6': {

		if (draw_isoline == DRAW_ISOLINE_ONE)
		{
			isoline_value = 6.0;
		}

		vbias += 0.01;
		if (vbias > 0.3)
		{
			vbias = 0;
		}
		break;
	} 
	case '7': {

		if (draw_isoline == DRAW_ISOLINE_ONE)
		{
			isoline_value = 7.0;
		}

		colorNumbers = 256;
		break;
	}
	case '8': {

		if (draw_isoline == DRAW_ISOLINE_ONE)
		{
			isoline_value = 8.0;
		}

		colorNumbers = 128;
		break;
	}
	case '9': {

		if (draw_isoline == DRAW_ISOLINE_ONE)
		{
			isoline_value = 9.0;
		}

		colorNumbers = 64;
		break;
	} 
	case '0': {
		colorNumbers = 32;
		break;
	}
			  
	case 'q': exit(0);
	}
}



// drag: When the user drags with the mouse, add a force that corresponds to the direction of the mouse
//       cursor movement. Also inject some new matter into the field at the mouse location.
void drag(int mx, int my)
{
	int xi, yi, X, Y; double  dx, dy, len;
	static int lmx = 0, lmy = 0;				//remembers last mouse location

	// Compute the array index that corresponds to the cursor location 
	xi = (int)clamp((double)(DIM + 1) * ((double)mx / (double)winWidth));
	yi = (int)clamp((double)(DIM + 1) * ((double)(winHeight - my) / (double)winHeight));

	X = xi; Y = yi;

	if (X > (DIM - 1))  X = DIM - 1; if (Y > (DIM - 1))  Y = DIM - 1;
	if (X < 0) X = 0; if (Y < 0) Y = 0;

	// Add force at the cursor location 
	my = winHeight - my;
	dx = mx - lmx; dy = my - lmy;
	len = sqrt(dx * dx + dy * dy);
	if (len != 0.0) { dx *= 0.1 / len; dy *= 0.1 / len; }
	fx[Y * DIM + X] += dx;
	fy[Y * DIM + X] += dy;
	rho[Y * DIM + X] = 10.0f;
	lmx = mx; lmy = my;
	barColorSelected = 0;
}

void barClicked(int button, int state, int x, int y) {

	barColorSelected = x;
	//printf("The bar selecte is %d", barColorSelected);
}

//do_one_simulation_step: Do one complete cycle of the simulation:
//      - set_forces:       
//      - solve:            read forces from the user
//      - diffuse_matter:   compute a new set of velocities
//      - gluPostRedisplay: draw a new visualization frame
void do_one_simulation_step(void)
{
	if (!frozen)
	{
		set_forces();
		solve(DIM, vx, vy, vx0, vy0, visc, dt);
		diffuse_matter(DIM, vx, vy, rho, rho0, dt);



		if (draw_3D == 1)
		{
			glutSetWindow(mainWindow);
			display();
		}
		else
		{
			glutPostRedisplay();
			glutSetWindow(mainWindow);
			display();
			glutSetWindow(barColorSelected);
			display1();
			glutSetWindow(textbarWindow);
			displayText();
		}

	}
}


void glInit()
{
	glShadeModel(GL_FLAT);//SMOOTH//GL_FLAT
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearDepth(1.0f);

	//glEnable(GL_NORMALIZE);

	glEnable(GL_DEPTH_TEST);
	glAlphaFunc(GL_GREATER, 0);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);

	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); // Make round points, not square points
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);  // Antialias the lines
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);


	//glClearColor(1.0,1.0,1.0,1.0);  //
	glMatrixMode(GL_MODELVIEW); //

   //glEnable(GL_COLOR_MATERIAL);
   //glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);


	GLfloat ambient[] = { 0.1, 0.6, 0.1, 1.0 }; // light source color rgb
	GLfloat position[] = { 250.0, 450.0, 130.0, 1.0 }; // light source coordinations x,y,z
	GLfloat mat_diffuse[] = { 0.1, 0.6, 0.1, 1.0 }; // light source diffuse color r,g,b
	GLfloat mat_specular[] = { 0.3, 0.3, 0.2, 1.0 }; // light strong 
	GLfloat mat_shininess[] = { 50.0 };




	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	/*
	*/

	glRotated(-30, 1, 0, 0);
}

void resizeEvent(int w, int h)
{

	winWidth = w; winHeight = h;
	wWidth = w;
	wHeight = h;
	zFar = 0.0f;
	xFar = 0.0f;
	yFar = 0.0f;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	h = h > 0 ? h : 1;
	float aspect = (float)w / (float)h;
	gluPerspective(45, aspect, 1.0, 1500.0);
	glTranslatef(-250, -250, -610.0f); //  start coordinations (x,y,z)
	resize();

	glutPostRedisplay();
}


//main: The main program
int main(int argc, char **argv)
{
	printf("Fluid Flow Simulation and Visualization\n");
	printf("=======================================\n");
	printf("Click and drag the mouse to steer the flow!\n");
	printf("T/t:   increase/decrease simulation timestep\n");
	printf("S/s:   increase/decrease hedgehog scaling\n");
	printf("c:     toggle direction coloring on/off\n");
	printf("V/vy:   increase decrease fluid viscosity\n");
	printf("x:     toggle drawing matter on/off\n");
	printf("y:     toggle drawing hedgehogs on/off\n");
	printf("m:     toggle color mapping\n");
	printf("a:     toggle the animation on/off\n");
	printf("b:     toggle the divergence on/off\n");
	printf("i:     toggle the isolines on/off \n");
	printf("q:     quit\n\n");

	if (draw_3D == 1)
	{
		printf("Hello world!");

		for (size_t i = 0; i < 400; i++)
		{
			double raHeight = -40.0 + rand() % 100;
			datas[i] = raHeight;
		}
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);  //

		glutInitWindowSize(wWidth, wHeight);         //
		glutInitWindowPosition(0, 0);  //
		mainWindow = glutCreateWindow("Real-time smoke simulation and visualization");
		glutReshapeFunc(resizeEvent);
		glutDisplayFunc(display);


		glutIdleFunc(do_one_simulation_step);
		glutKeyboardFunc(keyboard);
		glutMotionFunc(drag);

		glInit();
		init_simulation(DIM);	//initialize the simulation data structures	

		glutMainLoop();
		return 0;
	}
	else
	{
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
		glutInitWindowSize(500, 600);
		mainWindow = glutCreateWindow(" smoking");
		glutDisplayFunc(display);
		glutReshapeFunc(reshape);


	    glutIdleFunc(do_one_simulation_step);
		glutKeyboardFunc(keyboard);
		glutMotionFunc(drag);

		colorbarWindow = glutCreateSubWindow(mainWindow, 0, 530, 500, 20);
		glutDisplayFunc(display1);
		glutMouseFunc(barClicked);

		textbarWindow = glutCreateSubWindow(mainWindow, 0, 550, 500, 50);
		glutDisplayFunc(displayText);

		init_simulation(DIM);	//initialize the simulation data structures	
		glutMainLoop();			//calls do_one_simulation_step, keyboard, display, drag, reshape
		return 0;
	}
}
