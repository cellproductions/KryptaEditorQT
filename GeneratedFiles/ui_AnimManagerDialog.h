/********************************************************************************
** Form generated from reading UI file 'AnimManagerDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ANIMMANAGERDIALOG_H
#define UI_ANIMMANAGERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "animationsetupwidget.h"

QT_BEGIN_NAMESPACE

class Ui_AnimManagerDialog
{
public:
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout;
    QComboBox *cbAnims;
    QTabWidget *dirTabs;
    QWidget *tab_1;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout_4;
    AnimationSetupWidget *northAnim;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_6;
    QVBoxLayout *verticalLayout_5;
    AnimationSetupWidget *northeastAnim;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_8;
    QVBoxLayout *verticalLayout_7;
    AnimationSetupWidget *eastAnim;
    QWidget *tab_4;
    QVBoxLayout *verticalLayout_10;
    QVBoxLayout *verticalLayout_9;
    AnimationSetupWidget *southeastAnim;
    QWidget *tab_5;
    QVBoxLayout *verticalLayout_12;
    QVBoxLayout *verticalLayout_11;
    AnimationSetupWidget *southAnim;
    QWidget *tab_6;
    QVBoxLayout *verticalLayout_14;
    QVBoxLayout *verticalLayout_13;
    AnimationSetupWidget *southwestAnim;
    QWidget *tab_7;
    QVBoxLayout *verticalLayout_16;
    QVBoxLayout *verticalLayout_15;
    AnimationSetupWidget *westAnim;
    QWidget *tab_8;
    QVBoxLayout *verticalLayout_18;
    QVBoxLayout *verticalLayout_17;
    AnimationSetupWidget *northwestAnim;
    QHBoxLayout *horizontalLayout;
    QPushButton *bDone;
    QSpacerItem *horizontalSpacer;
    QPushButton *bDeleteAnim;
    QPushButton *bAddAnim;
    QPushButton *bImportAnim;
    QPushButton *bExportAnim;

    void setupUi(QDialog *AnimManagerDialog)
    {
        if (AnimManagerDialog->objectName().isEmpty())
            AnimManagerDialog->setObjectName(QStringLiteral("AnimManagerDialog"));
        AnimManagerDialog->resize(880, 700);
        verticalLayout_3 = new QVBoxLayout(AnimManagerDialog);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        cbAnims = new QComboBox(AnimManagerDialog);
        cbAnims->setObjectName(QStringLiteral("cbAnims"));
        cbAnims->setMaxVisibleItems(20);
        cbAnims->setDuplicatesEnabled(true);

        verticalLayout->addWidget(cbAnims);

        dirTabs = new QTabWidget(AnimManagerDialog);
        dirTabs->setObjectName(QStringLiteral("dirTabs"));
        tab_1 = new QWidget();
        tab_1->setObjectName(QStringLiteral("tab_1"));
        verticalLayout_2 = new QVBoxLayout(tab_1);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        northAnim = new AnimationSetupWidget(tab_1);
        northAnim->setObjectName(QStringLiteral("northAnim"));

        verticalLayout_4->addWidget(northAnim);


        verticalLayout_2->addLayout(verticalLayout_4);

        dirTabs->addTab(tab_1, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        verticalLayout_6 = new QVBoxLayout(tab_2);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        northeastAnim = new AnimationSetupWidget(tab_2);
        northeastAnim->setObjectName(QStringLiteral("northeastAnim"));

        verticalLayout_5->addWidget(northeastAnim);


        verticalLayout_6->addLayout(verticalLayout_5);

        dirTabs->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        verticalLayout_8 = new QVBoxLayout(tab_3);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        eastAnim = new AnimationSetupWidget(tab_3);
        eastAnim->setObjectName(QStringLiteral("eastAnim"));

        verticalLayout_7->addWidget(eastAnim);


        verticalLayout_8->addLayout(verticalLayout_7);

        dirTabs->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        verticalLayout_10 = new QVBoxLayout(tab_4);
        verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        southeastAnim = new AnimationSetupWidget(tab_4);
        southeastAnim->setObjectName(QStringLiteral("southeastAnim"));

        verticalLayout_9->addWidget(southeastAnim);


        verticalLayout_10->addLayout(verticalLayout_9);

        dirTabs->addTab(tab_4, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QStringLiteral("tab_5"));
        verticalLayout_12 = new QVBoxLayout(tab_5);
        verticalLayout_12->setObjectName(QStringLiteral("verticalLayout_12"));
        verticalLayout_11 = new QVBoxLayout();
        verticalLayout_11->setObjectName(QStringLiteral("verticalLayout_11"));
        southAnim = new AnimationSetupWidget(tab_5);
        southAnim->setObjectName(QStringLiteral("southAnim"));

        verticalLayout_11->addWidget(southAnim);


        verticalLayout_12->addLayout(verticalLayout_11);

        dirTabs->addTab(tab_5, QString());
        tab_6 = new QWidget();
        tab_6->setObjectName(QStringLiteral("tab_6"));
        verticalLayout_14 = new QVBoxLayout(tab_6);
        verticalLayout_14->setObjectName(QStringLiteral("verticalLayout_14"));
        verticalLayout_13 = new QVBoxLayout();
        verticalLayout_13->setObjectName(QStringLiteral("verticalLayout_13"));
        southwestAnim = new AnimationSetupWidget(tab_6);
        southwestAnim->setObjectName(QStringLiteral("southwestAnim"));

        verticalLayout_13->addWidget(southwestAnim);


        verticalLayout_14->addLayout(verticalLayout_13);

        dirTabs->addTab(tab_6, QString());
        tab_7 = new QWidget();
        tab_7->setObjectName(QStringLiteral("tab_7"));
        verticalLayout_16 = new QVBoxLayout(tab_7);
        verticalLayout_16->setObjectName(QStringLiteral("verticalLayout_16"));
        verticalLayout_15 = new QVBoxLayout();
        verticalLayout_15->setObjectName(QStringLiteral("verticalLayout_15"));
        westAnim = new AnimationSetupWidget(tab_7);
        westAnim->setObjectName(QStringLiteral("westAnim"));

        verticalLayout_15->addWidget(westAnim);


        verticalLayout_16->addLayout(verticalLayout_15);

        dirTabs->addTab(tab_7, QString());
        tab_8 = new QWidget();
        tab_8->setObjectName(QStringLiteral("tab_8"));
        verticalLayout_18 = new QVBoxLayout(tab_8);
        verticalLayout_18->setObjectName(QStringLiteral("verticalLayout_18"));
        verticalLayout_17 = new QVBoxLayout();
        verticalLayout_17->setObjectName(QStringLiteral("verticalLayout_17"));
        northwestAnim = new AnimationSetupWidget(tab_8);
        northwestAnim->setObjectName(QStringLiteral("northwestAnim"));

        verticalLayout_17->addWidget(northwestAnim);


        verticalLayout_18->addLayout(verticalLayout_17);

        dirTabs->addTab(tab_8, QString());

        verticalLayout->addWidget(dirTabs);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        bDone = new QPushButton(AnimManagerDialog);
        bDone->setObjectName(QStringLiteral("bDone"));

        horizontalLayout->addWidget(bDone);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        bDeleteAnim = new QPushButton(AnimManagerDialog);
        bDeleteAnim->setObjectName(QStringLiteral("bDeleteAnim"));

        horizontalLayout->addWidget(bDeleteAnim);

        bAddAnim = new QPushButton(AnimManagerDialog);
        bAddAnim->setObjectName(QStringLiteral("bAddAnim"));

        horizontalLayout->addWidget(bAddAnim);

        bImportAnim = new QPushButton(AnimManagerDialog);
        bImportAnim->setObjectName(QStringLiteral("bImportAnim"));

        horizontalLayout->addWidget(bImportAnim);

        bExportAnim = new QPushButton(AnimManagerDialog);
        bExportAnim->setObjectName(QStringLiteral("bExportAnim"));

        horizontalLayout->addWidget(bExportAnim);

        horizontalLayout->setStretch(0, 10);
        horizontalLayout->setStretch(1, 30);
        horizontalLayout->setStretch(2, 15);
        horizontalLayout->setStretch(3, 15);
        horizontalLayout->setStretch(4, 15);
        horizontalLayout->setStretch(5, 15);

        verticalLayout->addLayout(horizontalLayout);

        verticalLayout->setStretch(0, 5);
        verticalLayout->setStretch(1, 90);
        verticalLayout->setStretch(2, 5);

        verticalLayout_3->addLayout(verticalLayout);


        retranslateUi(AnimManagerDialog);

        dirTabs->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(AnimManagerDialog);
    } // setupUi

    void retranslateUi(QDialog *AnimManagerDialog)
    {
        AnimManagerDialog->setWindowTitle(QApplication::translate("AnimManagerDialog", "Animation Manager", 0));
        dirTabs->setTabText(dirTabs->indexOf(tab_1), QApplication::translate("AnimManagerDialog", "North", 0));
        dirTabs->setTabText(dirTabs->indexOf(tab_2), QApplication::translate("AnimManagerDialog", "North East", 0));
        dirTabs->setTabText(dirTabs->indexOf(tab_3), QApplication::translate("AnimManagerDialog", "East", 0));
        dirTabs->setTabText(dirTabs->indexOf(tab_4), QApplication::translate("AnimManagerDialog", "South East", 0));
        dirTabs->setTabText(dirTabs->indexOf(tab_5), QApplication::translate("AnimManagerDialog", "South", 0));
        dirTabs->setTabText(dirTabs->indexOf(tab_6), QApplication::translate("AnimManagerDialog", "South West", 0));
        dirTabs->setTabText(dirTabs->indexOf(tab_7), QApplication::translate("AnimManagerDialog", "West", 0));
        dirTabs->setTabText(dirTabs->indexOf(tab_8), QApplication::translate("AnimManagerDialog", "North West", 0));
        bDone->setText(QApplication::translate("AnimManagerDialog", "Done", 0));
        bDeleteAnim->setText(QApplication::translate("AnimManagerDialog", "Delete Animation", 0));
        bAddAnim->setText(QApplication::translate("AnimManagerDialog", "Add New Animation", 0));
        bImportAnim->setText(QApplication::translate("AnimManagerDialog", "Import Animation", 0));
        bExportAnim->setText(QApplication::translate("AnimManagerDialog", "Export Animation", 0));
    } // retranslateUi

};

namespace Ui {
    class AnimManagerDialog: public Ui_AnimManagerDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ANIMMANAGERDIALOG_H
