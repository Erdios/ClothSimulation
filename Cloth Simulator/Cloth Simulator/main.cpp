#include "ClothSimulator.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ClothSimulator w;

    w.resize(1500,1200);
    w.show();
    return a.exec();
}
