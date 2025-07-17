#include <QApplication>
#include "mainwindow.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    QCoreApplication::setApplicationName(QString("Bombtrajectory"));
    QCoreApplication::setApplicationVersion("0.1");
    QCoreApplication::setOrganizationName("momesana software inc.");
    QCoreApplication::setOrganizationDomain("momesana.com");

    MainWindow mw;
    mw.show();
    return app.exec();
}
