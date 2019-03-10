#pragma once
#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <rfftw.h>
#include "colorMappers.h"

class WTFluids: public QGLWidget 
{
	Q_OBJECT
	
public:
	WTFluids(QWidget *parent = 0);
	float huebias, saturationbias, vbias;


public slots:
	void setXRotation(int angle);
	void do_one_simulation_step(void);
	

signals:
	void xRotationChanged(int angle);

protected:
	void initializeGL();
	void paintGL();
	//void resizeGL(int width, int height);
	//void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

private:
	QPoint lastPos;
	QColor qtPurple;
	QTimer *cycleTimer;

	std::string IConvertS(int number);
	void initialized(int n);
	void FFT(int direction, void* vx);
	int clamp(float x);
	void solve(int n, fftw_real* vx, fftw_real* vy, fftw_real* vx0, fftw_real* vy0, fftw_real visc, fftw_real dt);
	void diffuse_matter(int n, fftw_real *vx, fftw_real *vy, fftw_real *rho, fftw_real *rho0, fftw_real dt);
	void set_forces(void);
	void direction_to_color(float x, float y, int method);
	void visualize(void);
	void set_colormap(float vy);
};


#endif
