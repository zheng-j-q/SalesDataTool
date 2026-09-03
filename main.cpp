#include "stdafx.h"
#include "SalesDataTool.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SalesDataTool window;
    window.show();
    return app.exec();
}
