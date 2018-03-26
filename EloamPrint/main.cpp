#include "eloamprint.h"
#include <QtWidgets/QApplication>
#include <stdlib.h>  
//#ifdef NDEBUG
//#else
//	#include "vld.h"
//#endif
int main(int argc, char *argv[])
{

	QApplication a(argc, argv);
	EloamPrint w;
	w.show();
	return a.exec();
}
