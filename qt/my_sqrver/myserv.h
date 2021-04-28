#ifndef MYSERV_H
#define MYSERV_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QFile>

class myserv : public QTcpServer {
    Q_OBJECT
public:
    myserv();
    ~myserv();

    QTcpSocket      *socket;
    QByteArray      data;
    QDir            dir;
    QJsonDocument   doc;
    QJsonParseError docError;
    QFile           target;
public slots:
    void StartServer(); // Start my_serv
    void incomingConnection(qintptr socketDescriptor); //Действия при новом подключении socketDescriptor - номер подключения
    void sockReady(); // готовность сокета
    void sockDisc(); // отключение от сокета
    void createFileList();
};

#endif // MYSERV_H
