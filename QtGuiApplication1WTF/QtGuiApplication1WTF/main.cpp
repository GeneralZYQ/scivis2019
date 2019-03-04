#include "QtGuiApplication1WTF.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QtGuiApplication1WTF w;
	w.show();
	return a.exec();
}
