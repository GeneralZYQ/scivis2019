//#include "fftw3.h"
//#pragma comment(lib, "libfftw3-3.lib")
//#include <fftw3.h>


#include <QtGui>
#include <QtOpenGL>
#include "WTFluids.h"
#include <sstream>
#include <rfftw.h>

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
const int COLOR_BLACKWHITE = 0;   //different types of color mapping: black-and-white, rainbow, banded
const int COLOR_RAINBOW = 1;
const int COLOR_BANDS = 2;
int   scalar_col = 0;           //method for scalar coloring
int   frozen = 0;               //toggles on/off the animation

WTFluids::WTFluids(QWidget *parent)
	: QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
	
	qtPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);

	//fftw_complex y[5];
}


void WTFluids::setXRotation(int angle)
{
	
	updateGL();
}

void WTFluids::initialized()
{


}


void WTFluids::initializeGL()
{
	qglClearColor(qtPurple.dark());

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_MULTISAMPLE);
	static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void WTFluids::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -10.0);
}

void WTFluids::resizeGL(int width, int height)
{
	int side = qMin(width, height);
	glViewport((width - side) / 2, (height - side) / 2, side, side);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
#ifdef QT_OPENGL_ES_1
	glOrthof(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
#else
	glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
#endif
	glMatrixMode(GL_MODELVIEW);
}

void WTFluids::mouseMoveEvent(QMouseEvent *event)
{
	int dx = event->x() - lastPos.x();
	int dy = event->y() - lastPos.y();

	emit xRotationChanged(dx);

	if (event->buttons() & Qt::LeftButton) {
		
	}
	else if (event->buttons() & Qt::RightButton) {

	}
	lastPos = event->pos();
}

std::string WTFluids::IConvertS(int number)
{
	std::ostringstream buff;
	buff << number;
	return buff.str();
}
