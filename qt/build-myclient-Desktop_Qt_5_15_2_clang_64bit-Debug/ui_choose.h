/********************************************************************************
** Form generated from reading UI file 'choose.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHOOSE_H
#define UI_CHOOSE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Choose
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *Choose)
    {
        if (Choose->objectName().isEmpty())
            Choose->setObjectName(QString::fromUtf8("Choose"));
        Choose->resize(385, 126);
        verticalLayout = new QVBoxLayout(Choose);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(Choose);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(17);
        label->setFont(font);

        horizontalLayout->addWidget(label);

        lineEdit = new QLineEdit(Choose);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        horizontalLayout->addWidget(lineEdit);


        verticalLayout->addLayout(horizontalLayout);

        buttonBox = new QDialogButtonBox(Choose);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(Choose);

        QMetaObject::connectSlotsByName(Choose);
    } // setupUi

    void retranslateUi(QDialog *Choose)
    {
        Choose->setWindowTitle(QCoreApplication::translate("Choose", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("Choose", "Choose file:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Choose: public Ui_Choose {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHOOSE_H
