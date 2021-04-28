#include <QCoreApplication>
#include "myserv.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    myserv serv;
    serv.StartServer();
    return a.exec();
}
