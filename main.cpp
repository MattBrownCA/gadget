#include <QApplication>

#include <primarywidget.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    PrimaryWidget p;
    p.show();

    return a.exec();
}
