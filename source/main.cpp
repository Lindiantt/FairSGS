#include "mainwindow.h"
#include <QApplication>

MainWindow *w;

/*void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    int i;
    i=1;
}*/

int main(int argc, char *argv[])
{
    //qInstallMessageHandler(myMessageOutput);
    QApplication a(argc, argv);
    w=new MainWindow;
    w->show();

    return a.exec();
}
