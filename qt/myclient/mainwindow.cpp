#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "choose.h"
#include <QJsonArray>
#include <QFileInfo>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
        connect(socket, SIGNAL(readyRead()), this, SLOT(sockReady())); //установка соединений
        connect(socket, SIGNAL(disconnected()), this, SLOT(sockDisc()));
    socket->connectToHost("127.0.0.1", 0165);
    socket->waitForConnected();
    this->createUI(QStringList() << ("id")
                                     << ("File Name")
                                     << ("Date of download")
                                     << ("Link(Double click to download)"));
    socket->write("{\"command\":\"fileList\"}");
    connect(this, SIGNAL(cellDoubleClicked(QTableWidgetItem*)),
            this, SLOT(on_tableWidget_itemDoubleClicked(QTableWidgetItem*)));
}

void MainWindow::createUI(const QStringList &headers) {
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setShowGrid(true);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->hideColumn(0);
}

void MainWindow::createNewElem(QJsonDocument elem) {
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, new QTableWidgetItem(ui->tableWidget->rowCount()));

    QTableWidgetItem* item2 = new QTableWidgetItem(elem.object().value("filename").toString());
    item2->setFlags(item2->flags() & ~(Qt::ItemIsEditable));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, item2);

    QTableWidgetItem* item1 = new QTableWidgetItem(elem.object().value("filedata").toString());
    item1->setFlags(item1->flags() & ~(Qt::ItemIsEditable));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, item1);

    QTableWidgetItem* item = new QTableWidgetItem(elem.object().value("filename").toString());
    item->setFlags(item->flags() & ~(Qt::ItemIsEditable));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3,item);
    ui->tableWidget->resizeColumnsToContents();
}

void MainWindow::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    if (item->column() != 3)
        return;
    QString selectedName = item->text();
    QString text = ui->lineEdit->text();
    if (!QDir(text).exists()) {
        QMessageBox::information(this, "Download Error", "This folder does not exist!");
        return;
    }
    QFile fl(text + "/" + selectedName);
    if (fl.exists()) {
        QMessageBox::information(this, "Download Error", "File already exists!");
        return;
    }
    target.setFileName(text + "/" + selectedName);
    uploadStatus = true;
    QJsonObject send;
    send["command"] = "upload";
    send["filename"] = selectedName;
    QJsonDocument doc(send);
    QByteArray bytes = doc.toJson();
    socket->write(bytes);
    socket->waitForBytesWritten();
    downloadPath = text;
    QFile table(downloadPath + "/DownloadList.txt");
    table.open(QFile::Append | QFile::Text);
    QTextStream out(&table);
    out << selectedName << "  Date of download: " << QDateTime::currentDateTime().toString() << "\n";
    table.close();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sockDisc() {
    socket->deleteLater();
}

void MainWindow::sockReady() {
    if (socket->waitForConnected(500)) {
        Data = socket->readAll();
        doc = QJsonDocument::fromJson(Data,&docError);
        if (docError.errorString() == "no error occurred") {
             if (doc.object().value("command").toString() == "connect") {
                 QMessageBox::information(this, "Status", "We will be connected!");
             }
             else if (doc.object().value("command").toString() == "fileList") {
                 QJsonObject arr = doc.object();
                 QJsonArray jsonay = arr["fileList"].toArray();
                 foreach (const QJsonValue & value, jsonay) {
                     QJsonObject obj = value.toObject();
                     this->createNewElem(QJsonDocument(obj));
                 }
                 return;
             }
             else if (doc.object().value("command").toString() == "download" && doc.object().value("status").toString() == "true") {
                 QFile file(_file);
                 if (!file.open(QFile::ReadOnly)) {
                     QMessageBox::information(this, "Status", "Invalid file!");
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
                 QJsonObject newElem;
                 QFileInfo fi = _file;
                 QString fileName = fi.fileName();
                 newElem["filename"] = fileName;
                 newElem["filedata"] = QDateTime::currentDateTime().toString();
                 this->createNewElem(QJsonDocument(newElem));
             }
             else if (doc.object().value("command").toString() == "download" && doc.object().value("status").toString() == "false") {
                 QMessageBox::information(this, "Download Error", "This file already exists!");
             }
             else {
                 QMessageBox::information(this, "Status", "Connection Error!");
             }
        }
        else {
            if (uploadStatus) {
                if (!target.open(QIODevice::WriteOnly | QIODevice::Append)) {
                    return;
                }
                target.write(Data);
                target.close();
                return;
            }
            else {
                uploadStatus = false;
            }
        }
    }
}

void MainWindow::on_pushButton_2_clicked() {
    Choose *wnd = new Choose(this);
    wnd->show();
    connect(wnd, SIGNAL(FilePath(QString)), this, SLOT(downloadToServ(QString)));
}

void MainWindow::downloadToServ(const QString &str) {
    //    {"command":"name","filename":"name"} - format
    QFile file(str);   //file path
    _file = str;
    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::information(this, "Status", "Invalid file!");
        return;
    }
    QJsonObject send;
    QFileInfo fi = _file;
    QString fileName = fi.fileName();
    send["command"] = "download";
    send["filename"] = fileName;
    QJsonDocument doc(send);
    QByteArray bytes = doc.toJson();
    socket->write(bytes);
    socket->waitForBytesWritten();
    file.close();
}
