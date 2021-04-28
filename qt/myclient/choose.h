#ifndef CHOOSE_H
#define CHOOSE_H

#include <QDialog>
#include <QDialogButtonBox>

namespace Ui {
class Choose;
}

class Choose : public QDialog
{
    Q_OBJECT

public:
    explicit Choose(QWidget *parent = nullptr);
    ~Choose();

private:
    Ui::Choose *ui;
signals:
    void FilePath(const QString &str);
private slots:
    void    okClick();
};

#endif // CHOOSE_H
