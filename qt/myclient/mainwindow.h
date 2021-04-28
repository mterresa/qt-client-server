#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QFile>
#include <QTabWidget>
#include <QTableWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QTcpSocket      *socket; // отвечает за передачу информации
    QByteArray      Data;
    QJsonDocument   doc;
    QJsonParseError docError;
    QString         _file;
    bool            uploadStatus = false;
    QFile           target;
    QString         downloadPath;

public slots:
    void    sockReady(); // готовность читать от сервера
    void    sockDisc(); // поведение при отключении

private slots:
    void    on_pushButton_2_clicked();
    void    downloadToServ(const QString &str);
    void    on_tableWidget_itemDoubleClicked(QTableWidgetItem* item);

private:
    Ui::MainWindow *ui;
    void    createUI(const QStringList &headers);
    void    createNewElem(QJsonDocument elem);
signals:
    void    cellDoubleClicked(QTableWidgetItem* item);
};
#endif // MAINWINDOW_H
