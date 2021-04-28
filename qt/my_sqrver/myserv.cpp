#include "myserv.h"
#include <QDir>
#include <QJsonArray>

myserv::myserv() {};

myserv::~myserv() {};

void myserv::StartServer() {
    if (this->listen(QHostAddress::Any, 0165)) {
        qDebug() << "Listening";
        if (!dir.exists("srorage") ) {
            dir.mkdir("storage");
            dir.cd("storage");
        }
        else {
            dir.cd("storage");
        }
    }
    else {
        qDebug() << "Not listening";
    }
}

void myserv::incomingConnection(qintptr socketDescriptor) {
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);

    connect(socket,SIGNAL(readyRead()),this,SLOT(sockReady()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(sockDisc()));
    qDebug() << socketDescriptor << "Client connected";
}

void myserv::sockReady() {
    data = socket->readAll();

    doc = QJsonDocument::fromJson(data, &docError);
//    {"command":"name","filename":"name"} - format
    if (docError.errorString() == "no error occurred")  {
        qDebug() << "-----";
        if (doc.object().value("command").toString() == "download") {
            QDir n_dir;
            n_dir.cd("storage");
            if (n_dir.exists(doc.object().value("filename").toString())) {
                socket->write("{\"command\":\"download\",\"status\":\"false\"}");
                return;
            }
            target.setFileName(dir.absolutePath() + "/" + doc.object().value("filename").toString());
            socket->write("{\"command\":\"download\",\"status\":\"true\"}");
        }
        else if (doc.object().value("command").toString() == "upload") {
             QFile file(dir.absolutePath() + "/" + doc.object().value("filename").toString());
             if (!file.open(QFile::ReadOnly)) {
                 return;
             }
             QByteArray q = file.readAll();
             file.close();
             qint64 x = 0;
             while (x < q.size()) {
                 qint64 y = socket->write(q);
                 x += y;
             }
             socket->waitForBytesWritten();
        }
        else if (doc.object().value("command").toString() == "fileList") {
            this->createFileList();
            return;
        }
        else {
        }
    }
    else {
        if (!target.open(QIODevice::WriteOnly | QIODevice::Append)) {
            qDebug() << "Can't open file for written";
            return;
        }
        target.write(data);
        target.close();
    }
}

void    myserv::createFileList() {
    QJsonArray arr;
    QDir n_dir;
    n_dir.cd("storage");
    QFileInfoList list = n_dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        if (fileInfo.fileName() == "." | fileInfo.fileName() == "..")
            continue;
        QJsonObject send;
        send["filename"] = fileInfo.fileName();
        send["filedata"] = fileInfo.lastModified().toString();
        arr.append(send);
    }
    QJsonObject fileList;
    fileList["command"] = "fileList";
    fileList["fileList"] = arr;
    QJsonDocument doc(fileList);
    QByteArray bytes = doc.toJson();
    socket->write(bytes);
    socket->waitForBytesWritten();
}

void myserv::sockDisc() {
    qDebug() << "Disconnected!";
    socket->deleteLater();
}
