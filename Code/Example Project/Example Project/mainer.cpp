#include <Qt/qapplication.h>


int main(int argc, char * argv[]) {
	QApplication app(argc, argv);
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);



	return app.exec();
}