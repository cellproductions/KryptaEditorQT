#include "MainWindow.h"
#include <Utilities/Exception.h>
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
	try
	{
		QApplication a(argc, argv);
		MainWindow w;
		w.init();
		w.show();

		return a.exec();
	}
	catch (const kry::Util::Exception& e)
	{
		qDebug() << e.what();
	}

	return 1;
}
