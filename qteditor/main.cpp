#include "EditorMain.h"
#include <QtWidgets/QApplication>
#include "Editor/QC_Application.h"

int main(int argc, char *argv[])
{
	ACApplication a(argc, argv);

	return a.run();
}
