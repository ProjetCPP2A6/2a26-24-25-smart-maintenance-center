/********************************************************************************
** Form generated from reading UI file 'dialog_statistiques.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_STATISTIQUES_H
#define UI_DIALOG_STATISTIQUES_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog_Statistiques
{
public:
    QLabel *label_stats;
    QPushButton *pushButton_Fermer;

    void setupUi(QDialog *Dialog_Statistiques)
    {
        if (Dialog_Statistiques->objectName().isEmpty())
            Dialog_Statistiques->setObjectName(QString::fromUtf8("Dialog_Statistiques"));
        Dialog_Statistiques->resize(556, 678);
        label_stats = new QLabel(Dialog_Statistiques);
        label_stats->setObjectName(QString::fromUtf8("label_stats"));
        label_stats->setGeometry(QRect(40, 40, 461, 441));
        label_stats->setStyleSheet(QString::fromUtf8(""));
        pushButton_Fermer = new QPushButton(Dialog_Statistiques);
        pushButton_Fermer->setObjectName(QString::fromUtf8("pushButton_Fermer"));
        pushButton_Fermer->setGeometry(QRect(190, 490, 161, 31));
        QFont font;
        font.setFamily(QString::fromUtf8("Montserrat SemiBold"));
        font.setPointSize(11);
        pushButton_Fermer->setFont(font);
        pushButton_Fermer->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: rgba(35,10,152, 200);\n"
"border-radius: 8px;\n"
"color: white;\n"
"}\n"
"QPushButton:hover {\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(117, 201, 187, 200), stop:1 rgba(35,10,152, 200));\n"
"}"));

        retranslateUi(Dialog_Statistiques);

        QMetaObject::connectSlotsByName(Dialog_Statistiques);
    } // setupUi

    void retranslateUi(QDialog *Dialog_Statistiques)
    {
        Dialog_Statistiques->setWindowTitle(QCoreApplication::translate("Dialog_Statistiques", "Dialog", nullptr));
        label_stats->setText(QString());
        pushButton_Fermer->setText(QCoreApplication::translate("Dialog_Statistiques", "Fermer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog_Statistiques: public Ui_Dialog_Statistiques {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_STATISTIQUES_H
