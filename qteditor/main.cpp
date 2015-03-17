#include "EditorMain.h"
#include <QtWidgets/QApplication>
#include "Editor/QC_Application.h"

int main(int argc, char *argv[])
{
	QCApplication a(argc, argv);

	EditorMain mainWindow;

	mainWindow.onStart();
	mainWindow.show();

	return a.exec();
}
