#include "choose.h"
#include "ui_choose.h"
#include <QPushButton>

Choose::Choose(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Choose)
{
    ui->setupUi(this);
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), SLOT(okClick()));
    connect(ui->buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), SLOT(close()));
}

void    Choose::okClick() {
    emit FilePath(ui->lineEdit->text());
    close();
}

Choose::~Choose()
{
    delete ui;
}
