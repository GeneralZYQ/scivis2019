#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtGuiApplication1WTF.h"


class WTFluids;

class QtGuiApplication1WTF : public QMainWindow
{
	Q_OBJECT

public:
	QtGuiApplication1WTF(QWidget *parent = Q_NULLPTR);

public slots:
	void showActiveMembers();
	void rainbowtoggled();
	void grayscaletoggled();
	void bluetoyellowtoggled();
	void heatmaptoggled();
	void setHValue(int h);
	void setSValue(int s);
	void setVValue(int v);
	void color256toggled();
	void color128toggled();
	void color64toggled();
	void color32toggled();
	void fluidsDragged(int posx);
	void showSmoke();
	void hideSmoke();

	void drawBar();

private:
	Ui::QtGuiApplication1WTFClass ui;
	std::string FConvertS(float number);
	WTFluids *fluids;
};
