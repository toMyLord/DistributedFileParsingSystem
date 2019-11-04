/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTextBrowser>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QTextBrowser *ParsingInfo;
    QLabel *label;
    QLabel *label_2;
    QTextBrowser *WorkstationInfo;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(582, 282);
        ParsingInfo = new QTextBrowser(Dialog);
        ParsingInfo->setObjectName(QString::fromUtf8("ParsingInfo"));
        ParsingInfo->setGeometry(QRect(20, 51, 256, 201));
        label = new QLabel(Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 10, 181, 31));
        label_2 = new QLabel(Dialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(300, 10, 101, 31));
        WorkstationInfo = new QTextBrowser(Dialog);
        WorkstationInfo->setObjectName(QString::fromUtf8("WorkstationInfo"));
        WorkstationInfo->setGeometry(QRect(300, 51, 251, 201));

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", nullptr));
        label->setText(QApplication::translate("Dialog", "\346\226\207\344\273\266\350\247\243\346\236\220\346\234\215\345\212\241\345\231\250:", nullptr));
        label_2->setText(QApplication::translate("Dialog", "\345\267\245\344\275\234\347\253\231:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
