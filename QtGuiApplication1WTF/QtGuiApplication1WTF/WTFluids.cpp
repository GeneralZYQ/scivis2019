//#include "fftw3.h"
//#pragma comment(lib, "FFTW.lib")
//#include <fftw3.h>


#include <QtGui>
#include <QtOpenGL>
#include "WTFluids.h"
#include <sstream>




	//--- SIMULATION PARAMETERS ------------------------------------------------------------------------
	const int DIM = 50;				//size of simulation grid
	double dt = 0.4;				//simulation time step
	float visc = 0.001;				//fluid viscosity
	fftw_real *vx, *vy;             //(vx,vy)   = velocity field at the current moment
	fftw_real *vx0, *vy0;           //(vx0,vy0) = velocity field at the previous moment
	fftw_real *fx, *fy;	            //(fx,fy)   = user-controlled simulation forces, steered with the mouse 
	fftw_real *rho, *rho0;			//smoke density at the current (rho) and previous (rho0) moment 
	rfftwnd_plan plan_rc, plan_cr;  //simulation domain discretization

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
	initialized(50);
	cycleTimer = new QTimer(this);
	connect(cycleTimer, SIGNAL(timeout()), this, SLOT(do_one_simulation_step()));
	cycleTimer->start(400);

	emit xRotationChanged(400);
}


void WTFluids::setXRotation(int angle)
{
	
	updateGL();
}

void WTFluids::initialized(int n)
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


	for (size_t i = 0; i < 256; i++)
	{
		float value = (float)i / 256.0;


	}


	for (i = 0; i < n * n; i++)                      //Initialize data structures to 0
	{
		vx[i] = vy[i] = vx0[i] = vy0[i] = fx[i] = fy[i] = rho[i] = rho0[i] = 0.0f;
	}
}

void WTFluids::FFT(int direction, void* vx)
{
	if (direction == 1) rfftwnd_one_real_to_complex(plan_rc, (fftw_real*)vx, (fftw_complex*)vx);
	else             rfftwnd_one_complex_to_real(plan_cr, (fftw_complex*)vx, (fftw_real*)vx);
}

int WTFluids::clamp(float x)
{
	return ((x) >= 0.0 ? ((int)(x)) : (-((int)(1 - (x)))));
}


void WTFluids::solve(int n, fftw_real* vx, fftw_real* vy, fftw_real* vx0, fftw_real* vy0, fftw_real visc, fftw_real dt)
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

void WTFluids::diffuse_matter(int n, fftw_real *vx, fftw_real *vy, fftw_real *rho, fftw_real *rho0, fftw_real dt)
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

void WTFluids::set_forces(void)
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

void WTFluids::direction_to_color(float x, float y, int method)
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

void WTFluids::set_colormap(float vy)
{
	float R, G, B;

	if (scalar_col == COLOR_BLACKWHITE)
		//R = G = B = vy;
		grayscale(vy, &R, &G, &B);
	else if (scalar_col == COLOR_RAINBOW)
		rainbow(vy, huebias, saturationbias, vbias, &R, &G, &B);
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

void WTFluids::visualize(void)
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
			}
		}
		glEnd();
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



void WTFluids::do_one_simulation_step(void)
{

	if (!frozen)
	{
		set_forces();
		solve(DIM, vx, vy, vx0, vy0, visc, dt);
		diffuse_matter(DIM, vx, vy, rho, rho0, dt);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		visualize();
		//gridDivergence();
		glFlush();
		QGLWidget::swapBuffers();
	}
}


void WTFluids::mouseMoveEvent(QMouseEvent *event)
{
	int xi, yi, X, Y; double  dx, dy, len;
	static int lmx = 0, lmy = 0;				//remembers last mouse location

	int mx = event->x() - lastPos.x();
	int my = event->y() - lastPos.y();

	// Compute the array index that corresponds to the cursor location 
	xi = (int)clamp((double)(DIM + 1) * ((double)mx / (double)winWidth));
	yi = (int)clamp((double)(DIM + 1) * ((double)(winHeight - my) / (double)winHeight));

	X = xi; Y = yi;

	if (X > (DIM - 1))  X = DIM - 1; if (Y > (DIM - 1))  Y = DIM - 1;
	if (X < 0) X = 0; if (Y < 0) Y = 0;

	emit xRotationChanged(mx);

	// Add force at the cursor location 
	my = winHeight - my;
	dx = mx - lmx; dy = my - lmy;
	len = sqrt(dx * dx + dy * dy);
	if (len != 0.0) { dx *= 0.1 / len; dy *= 0.1 / len; }
	fx[Y * DIM + X] += dx;
	fy[Y * DIM + X] += dy;
	rho[Y * DIM + X] = 10.0f;
	lmx = mx; lmy = my;


	

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


/*

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
*/