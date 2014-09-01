/********************************************************************************
** Form generated from reading UI file 'layerbrowserdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LAYERBROWSERDIALOG_H
#define UI_LAYERBROWSERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_LayerBrowserDialog
{
public:
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QListWidget *listWidget;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;

    void setupUi(QDialog *LayerBrowserDialog)
    {
        if (LayerBrowserDialog->objectName().isEmpty())
            LayerBrowserDialog->setObjectName(QStringLiteral("LayerBrowserDialog"));
        LayerBrowserDialog->resize(640, 480);
        verticalLayout_3 = new QVBoxLayout(LayerBrowserDialog);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        listWidget = new QListWidget(LayerBrowserDialog);
        listWidget->setObjectName(QStringLiteral("listWidget"));

        horizontalLayout_2->addWidget(listWidget);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));

        horizontalLayout_2->addLayout(verticalLayout_2);

        horizontalLayout_2->setStretch(0, 50);
        horizontalLayout_2->setStretch(1, 50);

        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButton_2 = new QPushButton(LayerBrowserDialog);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout->addWidget(pushButton_2);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton = new QPushButton(LayerBrowserDialog);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout->addWidget(pushButton);

        horizontalLayout->setStretch(0, 20);
        horizontalLayout->setStretch(1, 60);
        horizontalLayout->setStretch(2, 20);

        verticalLayout->addLayout(horizontalLayout);

        verticalLayout->setStretch(0, 95);
        verticalLayout->setStretch(1, 5);

        verticalLayout_3->addLayout(verticalLayout);


        retranslateUi(LayerBrowserDialog);

        QMetaObject::connectSlotsByName(LayerBrowserDialog);
    } // setupUi

    void retranslateUi(QDialog *LayerBrowserDialog)
    {
        LayerBrowserDialog->setWindowTitle(QApplication::translate("LayerBrowserDialog", "Layer Browser", 0));
        pushButton_2->setText(QApplication::translate("LayerBrowserDialog", "Select and Close", 0));
        pushButton->setText(QApplication::translate("LayerBrowserDialog", "Close", 0));
    } // retranslateUi

};

namespace Ui {
    class LayerBrowserDialog: public Ui_LayerBrowserDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LAYERBROWSERDIALOG_H
