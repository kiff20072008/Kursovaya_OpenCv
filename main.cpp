#include "Arkanoid.h"
#include "Definitions.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Arkanoid obj(WIDTH, HEIGHT);
	QGraphicsView* view = new QGraphicsView(&obj);
	view->show();
	obj.start();
	view->close();
	return a.exec();
}
