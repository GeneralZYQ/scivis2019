#pragma once
#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

class WTFluids: public QGLWidget 
{
	Q_OBJECT
	
public:
	WTFluids(QWidget *parent = 0);


public slots:
	void setXRotation(int angle);
	

signals:
	void xRotationChanged(int angle);

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);
	//void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

private:
	QPoint lastPos;
	QColor qtPurple;

	std::string IConvertS(int number);
	void initialized();
};


#endif
