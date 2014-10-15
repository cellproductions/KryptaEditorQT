/********************************************************************************
** Form generated from reading UI file 'SaveDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAVEDIALOG_H
#define UI_SAVEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_SaveDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QPushButton *bYes;
    QPushButton *bNo;
    QPushButton *bCancel;

    void setupUi(QDialog *SaveDialog)
    {
        if (SaveDialog->objectName().isEmpty())
            SaveDialog->setObjectName(QStringLiteral("SaveDialog"));
        SaveDialog->resize(259, 81);
        verticalLayout_2 = new QVBoxLayout(SaveDialog);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(SaveDialog);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout->addWidget(label);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        bYes = new QPushButton(SaveDialog);
        bYes->setObjectName(QStringLiteral("bYes"));

        horizontalLayout->addWidget(bYes);

        bNo = new QPushButton(SaveDialog);
        bNo->setObjectName(QStringLiteral("bNo"));

        horizontalLayout->addWidget(bNo);

        bCancel = new QPushButton(SaveDialog);
        bCancel->setObjectName(QStringLiteral("bCancel"));

        horizontalLayout->addWidget(bCancel);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(SaveDialog);

        QMetaObject::connectSlotsByName(SaveDialog);
    } // setupUi

    void retranslateUi(QDialog *SaveDialog)
    {
        SaveDialog->setWindowTitle(QApplication::translate("SaveDialog", "Save", 0));
        label->setText(QApplication::translate("SaveDialog", "Would you like to save your poject?", 0));
        bYes->setText(QApplication::translate("SaveDialog", "Yes", 0));
        bNo->setText(QApplication::translate("SaveDialog", "No", 0));
        bCancel->setText(QApplication::translate("SaveDialog", "Cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class SaveDialog: public Ui_SaveDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAVEDIALOG_H
