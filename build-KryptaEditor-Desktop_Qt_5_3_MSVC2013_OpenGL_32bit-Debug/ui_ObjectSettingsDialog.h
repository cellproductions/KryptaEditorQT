/********************************************************************************
** Form generated from reading UI file 'ObjectSettingsDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OBJECTSETTINGSDIALOG_H
#define UI_OBJECTSETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include "PivotView.h"

QT_BEGIN_NAMESPACE

class Ui_ObjectSettingsDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    PivotView *gvObject;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *bPivotSet;
    QPushButton *bPivotReset;
    QPushButton *bPivotCancel;
    QTabWidget *tabs;
    QHBoxLayout *horizontalLayout;
    QPushButton *bSave;
    QSpacerItem *horizontalSpacer;
    QPushButton *bCancel;

    void setupUi(QDialog *ObjectSettingsDialog)
    {
        if (ObjectSettingsDialog->objectName().isEmpty())
            ObjectSettingsDialog->setObjectName(QStringLiteral("ObjectSettingsDialog"));
        ObjectSettingsDialog->resize(320, 450);
        verticalLayout_2 = new QVBoxLayout(ObjectSettingsDialog);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        gvObject = new PivotView(ObjectSettingsDialog);
        gvObject->setObjectName(QStringLiteral("gvObject"));
        gvObject->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        gvObject->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        verticalLayout->addWidget(gvObject);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        bPivotSet = new QPushButton(ObjectSettingsDialog);
        bPivotSet->setObjectName(QStringLiteral("bPivotSet"));

        horizontalLayout_2->addWidget(bPivotSet);

        bPivotReset = new QPushButton(ObjectSettingsDialog);
        bPivotReset->setObjectName(QStringLiteral("bPivotReset"));

        horizontalLayout_2->addWidget(bPivotReset);

        bPivotCancel = new QPushButton(ObjectSettingsDialog);
        bPivotCancel->setObjectName(QStringLiteral("bPivotCancel"));

        horizontalLayout_2->addWidget(bPivotCancel);


        verticalLayout->addLayout(horizontalLayout_2);

        tabs = new QTabWidget(ObjectSettingsDialog);
        tabs->setObjectName(QStringLiteral("tabs"));

        verticalLayout->addWidget(tabs);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        bSave = new QPushButton(ObjectSettingsDialog);
        bSave->setObjectName(QStringLiteral("bSave"));

        horizontalLayout->addWidget(bSave);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        bCancel = new QPushButton(ObjectSettingsDialog);
        bCancel->setObjectName(QStringLiteral("bCancel"));

        horizontalLayout->addWidget(bCancel);


        verticalLayout->addLayout(horizontalLayout);

        verticalLayout->setStretch(0, 50);
        verticalLayout->setStretch(2, 50);
        verticalLayout->setStretch(3, 5);

        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(ObjectSettingsDialog);

        tabs->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(ObjectSettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *ObjectSettingsDialog)
    {
        ObjectSettingsDialog->setWindowTitle(QApplication::translate("ObjectSettingsDialog", "Object Settings", 0));
        bPivotSet->setText(QApplication::translate("ObjectSettingsDialog", "Set Pivot", 0));
        bPivotReset->setText(QApplication::translate("ObjectSettingsDialog", "Reset Pivot", 0));
        bPivotCancel->setText(QApplication::translate("ObjectSettingsDialog", "Cancel Pivot", 0));
        bSave->setText(QApplication::translate("ObjectSettingsDialog", "Save and Close", 0));
        bCancel->setText(QApplication::translate("ObjectSettingsDialog", "Cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class ObjectSettingsDialog: public Ui_ObjectSettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OBJECTSETTINGSDIALOG_H
