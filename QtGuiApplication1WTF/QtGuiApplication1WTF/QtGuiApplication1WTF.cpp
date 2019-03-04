#include "QtGuiApplication1WTF.h"
#include <sstream>
#include "hsvargb.h"
#include <math.h>
#include <gl/GLU.h>
#include <gl/GL.h>
#include "WTFluids.h"
#include "colorMappers.h"

float huebias, saturationbias, vbias;
int colorsCount;
int colorType; // 1: rainbow; 2:grayscale; 3: yellowtoBlue; 4: heatmapping
QWidget *blocks;

QtGuiApplication1WTF::QtGuiApplication1WTF(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	colorsCount = 256;
	colorType = 1;

	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(showActiveMembers()));

	connect(ui.radioButton, SIGNAL(clicked()), this, SLOT(rainbowtoggled()));
	connect(ui.grayscaleRadio, SIGNAL(clicked()), this, SLOT(grayscaletoggled()));
	connect(ui.blueToYellowRadio, SIGNAL(clicked()), this, SLOT(bluetoyellowtoggled()));
	connect(ui.heatMapRadio, SIGNAL(clicked()), this, SLOT(heatmaptoggled()));

	connect(ui.radioButton_256, SIGNAL(clicked()), this, SLOT(color256toggled()));
	connect(ui.radioButton_128, SIGNAL(clicked()), this, SLOT(color128toggled()));
	connect(ui.radioButton_64, SIGNAL(clicked()), this, SLOT(color64toggled()));
	connect(ui.radioButton_32, SIGNAL(clicked()), this, SLOT(color32toggled()));

	connect(ui.horizontalSlider_H, SIGNAL(valueChanged(int)), this, SLOT(setHValue(int)));
	connect(ui.horizontalSlider_S, SIGNAL(valueChanged(int)), this, SLOT(setSValue(int)));
	connect(ui.horizontalSlider_V, SIGNAL(valueChanged(int)), this, SLOT(setVValue(int)));
	ui.horizontalSlider_H->setValue(50);
	ui.horizontalSlider_S->setValue(50);
	ui.horizontalSlider_V->setValue(50);

	blocks = new QWidget[256];

	drawBar();

	ui.radioButton_256->setChecked(true);
	ui.radioButton->setChecked(true);

	fluids = new WTFluids(this);
	fluids->setGeometry(10, 33, 531, 511);
	fluids->show();
	connect(fluids, SIGNAL(xRotationChanged(int)), this, SLOT(fluidsDragged(int)));
}


void QtGuiApplication1WTF::showActiveMembers()
{
	ui.label->setText("hello");

	huebias = 0.1;
	saturationbias = -0.1;

	drawBar();
}

void QtGuiApplication1WTF::rainbowtoggled() {
	ui.label->setText("rainbow");
	colorType = 1;
	drawBar();
}

void QtGuiApplication1WTF::grayscaletoggled() {
	ui.label->setText("grayscale");
	colorType = 2;
	drawBar();
}

void QtGuiApplication1WTF::bluetoyellowtoggled() {
	ui.label->setText("blueyellow");
	colorType = 3;
	drawBar();
}

void QtGuiApplication1WTF::heatmaptoggled() {
	ui.label->setText("heatmapp");
	colorType = 4;
	drawBar();
}

void QtGuiApplication1WTF::color256toggled() {
	colorsCount = 256;
	drawBar();
}

void QtGuiApplication1WTF::fluidsDragged(int posx) {
	QString qstr = QString::fromStdString(FConvertS(posx));
	ui.label->setText(qstr);
}

void QtGuiApplication1WTF::color128toggled() {
	colorsCount = 128;
	drawBar();
}

void QtGuiApplication1WTF::color64toggled() {
	colorsCount = 64;
	drawBar();
}

void QtGuiApplication1WTF::color32toggled() {
	colorsCount = 32;
	drawBar();
}


void QtGuiApplication1WTF::drawBar() {


	float blockHeight = ceilf( 470.0 / colorsCount);
	
	for (size_t i = 0; i < colorsCount; i++)
	{
		float r, g, b = 0;

		if (colorType == 1)
		{
			rainbow((float)i / (float)colorsCount, huebias,saturationbias, vbias, &r, &g, &b);
		}
		else if (colorType == 2)
		{
			 grayscale((float)i / (float)colorsCount, &r, &g, &b);
		}
		else if (colorType == 3)
		{
			yellowToBlue((float)i / (float)colorsCount, &r, &g, &b);
		}

		QWidget *testWed = new QWidget(this);
		testWed->setGeometry(550, ceilf(blockHeight * i) + 23, 31, blockHeight);
		QPalette pal(ui.barwidget->palette());

		QString qstr = QString::fromStdString(FConvertS(b));
		ui.label->setText(qstr);

		pal.setColor(QPalette::Background, QColor(r * 255.0, g * 255.0, b * 255.0, 255));
		testWed->setAutoFillBackground(true);
		testWed->setPalette(pal);
		testWed->show();

	}
}


void QtGuiApplication1WTF::setHValue(int h) {

	QString qstr = QString::fromStdString(FConvertS(h));
	ui.label->setText(qstr);

	/*
	

	if (h > 70)
	{
		h = 55;
	}
	else if (h < 49)
	{
		h = 49;
	}
	*/

	if (h < 50)
	{
		h = 50;
	}

	huebias = (float)(h - 50) / 100.0;
	drawBar();
}

void QtGuiApplication1WTF::setSValue(int s) {

	/*
	

	if (s > 55)
	{
		s = 55;
	}
	else if (s < 40)
	{
		s = 40;
	}
	*/
	if (s > 50)
	{
		s = 50;
	}

	saturationbias = float(s - 50) / 100.0;
	drawBar();
}

void QtGuiApplication1WTF::setVValue(int v) {

	/*
	

	if (v > 50) {
		v = 50;
	}
	else if (v < 45)
	{
		v = 45;
	}
	*/

	if (v > 50)
	{
		v = 50;
	}

	vbias = float(v - 50) / 100.0;
	drawBar();
}

std::string QtGuiApplication1WTF::FConvertS(float number)
{
	std::ostringstream buff;
	buff << number;
	return buff.str();
}



