/********************************************************************************
** Form generated from reading UI file 'PrjSettingsDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRJSETTINGSDIALOG_H
#define UI_PRJSETTINGSDIALOG_H

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
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PrjSettingsDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabs;
    QWidget *projectTab;
    QVBoxLayout *verticalLayout_3;
    QTableWidget *projectProperties;
    QWidget *playerTab;
    QVBoxLayout *verticalLayout_4;
    QTableWidget *playerProperties;
    QHBoxLayout *horizontalLayout;
    QPushButton *bSave;
    QSpacerItem *horizontalSpacer;
    QPushButton *bCancel;

    void setupUi(QDialog *PrjSettingsDialog)
    {
        if (PrjSettingsDialog->objectName().isEmpty())
            PrjSettingsDialog->setObjectName(QStringLiteral("PrjSettingsDialog"));
        PrjSettingsDialog->resize(320, 360);
        verticalLayout_2 = new QVBoxLayout(PrjSettingsDialog);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tabs = new QTabWidget(PrjSettingsDialog);
        tabs->setObjectName(QStringLiteral("tabs"));
        projectTab = new QWidget();
        projectTab->setObjectName(QStringLiteral("projectTab"));
        verticalLayout_3 = new QVBoxLayout(projectTab);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        projectProperties = new QTableWidget(projectTab);
        if (projectProperties->columnCount() < 2)
            projectProperties->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        projectProperties->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        projectProperties->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        if (projectProperties->rowCount() < 2)
            projectProperties->setRowCount(2);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        projectProperties->setVerticalHeaderItem(0, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        projectProperties->setVerticalHeaderItem(1, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        projectProperties->setItem(0, 0, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        projectProperties->setItem(0, 1, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        projectProperties->setItem(1, 0, __qtablewidgetitem6);
        projectProperties->setObjectName(QStringLiteral("projectProperties"));
        projectProperties->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        projectProperties->horizontalHeader()->setVisible(false);
        projectProperties->horizontalHeader()->setStretchLastSection(true);
        projectProperties->verticalHeader()->setVisible(false);

        verticalLayout_3->addWidget(projectProperties);

        tabs->addTab(projectTab, QString());
        playerTab = new QWidget();
        playerTab->setObjectName(QStringLiteral("playerTab"));
        verticalLayout_4 = new QVBoxLayout(playerTab);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        playerProperties = new QTableWidget(playerTab);
        if (playerProperties->columnCount() < 2)
            playerProperties->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        playerProperties->setHorizontalHeaderItem(0, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        playerProperties->setHorizontalHeaderItem(1, __qtablewidgetitem8);
        if (playerProperties->rowCount() < 15)
            playerProperties->setRowCount(15);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        playerProperties->setVerticalHeaderItem(0, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        playerProperties->setVerticalHeaderItem(1, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        playerProperties->setVerticalHeaderItem(2, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        playerProperties->setVerticalHeaderItem(3, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        playerProperties->setVerticalHeaderItem(4, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        playerProperties->setVerticalHeaderItem(5, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        playerProperties->setVerticalHeaderItem(6, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        playerProperties->setVerticalHeaderItem(7, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        playerProperties->setVerticalHeaderItem(8, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        playerProperties->setVerticalHeaderItem(9, __qtablewidgetitem18);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        playerProperties->setVerticalHeaderItem(10, __qtablewidgetitem19);
        QTableWidgetItem *__qtablewidgetitem20 = new QTableWidgetItem();
        playerProperties->setVerticalHeaderItem(11, __qtablewidgetitem20);
        QTableWidgetItem *__qtablewidgetitem21 = new QTableWidgetItem();
        playerProperties->setVerticalHeaderItem(12, __qtablewidgetitem21);
        QTableWidgetItem *__qtablewidgetitem22 = new QTableWidgetItem();
        playerProperties->setVerticalHeaderItem(13, __qtablewidgetitem22);
        QTableWidgetItem *__qtablewidgetitem23 = new QTableWidgetItem();
        playerProperties->setVerticalHeaderItem(14, __qtablewidgetitem23);
        QTableWidgetItem *__qtablewidgetitem24 = new QTableWidgetItem();
        playerProperties->setItem(0, 0, __qtablewidgetitem24);
        QTableWidgetItem *__qtablewidgetitem25 = new QTableWidgetItem();
        playerProperties->setItem(1, 0, __qtablewidgetitem25);
        QTableWidgetItem *__qtablewidgetitem26 = new QTableWidgetItem();
        playerProperties->setItem(2, 0, __qtablewidgetitem26);
        QTableWidgetItem *__qtablewidgetitem27 = new QTableWidgetItem();
        playerProperties->setItem(3, 0, __qtablewidgetitem27);
        QTableWidgetItem *__qtablewidgetitem28 = new QTableWidgetItem();
        playerProperties->setItem(4, 0, __qtablewidgetitem28);
        QTableWidgetItem *__qtablewidgetitem29 = new QTableWidgetItem();
        playerProperties->setItem(5, 0, __qtablewidgetitem29);
        QTableWidgetItem *__qtablewidgetitem30 = new QTableWidgetItem();
        playerProperties->setItem(6, 0, __qtablewidgetitem30);
        QTableWidgetItem *__qtablewidgetitem31 = new QTableWidgetItem();
        playerProperties->setItem(7, 0, __qtablewidgetitem31);
        QTableWidgetItem *__qtablewidgetitem32 = new QTableWidgetItem();
        playerProperties->setItem(7, 1, __qtablewidgetitem32);
        QTableWidgetItem *__qtablewidgetitem33 = new QTableWidgetItem();
        playerProperties->setItem(8, 0, __qtablewidgetitem33);
        QTableWidgetItem *__qtablewidgetitem34 = new QTableWidgetItem();
        playerProperties->setItem(9, 0, __qtablewidgetitem34);
        QTableWidgetItem *__qtablewidgetitem35 = new QTableWidgetItem();
        playerProperties->setItem(9, 1, __qtablewidgetitem35);
        QTableWidgetItem *__qtablewidgetitem36 = new QTableWidgetItem();
        playerProperties->setItem(10, 0, __qtablewidgetitem36);
        QTableWidgetItem *__qtablewidgetitem37 = new QTableWidgetItem();
        playerProperties->setItem(10, 1, __qtablewidgetitem37);
        QTableWidgetItem *__qtablewidgetitem38 = new QTableWidgetItem();
        playerProperties->setItem(11, 0, __qtablewidgetitem38);
        QTableWidgetItem *__qtablewidgetitem39 = new QTableWidgetItem();
        playerProperties->setItem(11, 1, __qtablewidgetitem39);
        QTableWidgetItem *__qtablewidgetitem40 = new QTableWidgetItem();
        playerProperties->setItem(12, 0, __qtablewidgetitem40);
        QTableWidgetItem *__qtablewidgetitem41 = new QTableWidgetItem();
        playerProperties->setItem(12, 1, __qtablewidgetitem41);
        QTableWidgetItem *__qtablewidgetitem42 = new QTableWidgetItem();
        playerProperties->setItem(13, 0, __qtablewidgetitem42);
        QTableWidgetItem *__qtablewidgetitem43 = new QTableWidgetItem();
        playerProperties->setItem(13, 1, __qtablewidgetitem43);
        QTableWidgetItem *__qtablewidgetitem44 = new QTableWidgetItem();
        playerProperties->setItem(14, 0, __qtablewidgetitem44);
        QTableWidgetItem *__qtablewidgetitem45 = new QTableWidgetItem();
        playerProperties->setItem(14, 1, __qtablewidgetitem45);
        playerProperties->setObjectName(QStringLiteral("playerProperties"));
        playerProperties->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        playerProperties->horizontalHeader()->setVisible(false);
        playerProperties->horizontalHeader()->setStretchLastSection(true);
        playerProperties->verticalHeader()->setVisible(false);

        verticalLayout_4->addWidget(playerProperties);

        tabs->addTab(playerTab, QString());

        verticalLayout->addWidget(tabs);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        bSave = new QPushButton(PrjSettingsDialog);
        bSave->setObjectName(QStringLiteral("bSave"));

        horizontalLayout->addWidget(bSave);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        bCancel = new QPushButton(PrjSettingsDialog);
        bCancel->setObjectName(QStringLiteral("bCancel"));

        horizontalLayout->addWidget(bCancel);


        verticalLayout->addLayout(horizontalLayout);

        verticalLayout->setStretch(0, 95);
        verticalLayout->setStretch(1, 5);

        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(PrjSettingsDialog);

        tabs->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(PrjSettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *PrjSettingsDialog)
    {
        PrjSettingsDialog->setWindowTitle(QApplication::translate("PrjSettingsDialog", "Project Settings", 0));
        QTableWidgetItem *___qtablewidgetitem = projectProperties->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("PrjSettingsDialog", "Property", 0));
        QTableWidgetItem *___qtablewidgetitem1 = projectProperties->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("PrjSettingsDialog", "Value", 0));
        QTableWidgetItem *___qtablewidgetitem2 = projectProperties->verticalHeaderItem(0);
        ___qtablewidgetitem2->setText(QApplication::translate("PrjSettingsDialog", "Project name", 0));
        QTableWidgetItem *___qtablewidgetitem3 = projectProperties->verticalHeaderItem(1);
        ___qtablewidgetitem3->setText(QApplication::translate("PrjSettingsDialog", "Project path", 0));

        const bool __sortingEnabled = projectProperties->isSortingEnabled();
        projectProperties->setSortingEnabled(false);
        QTableWidgetItem *___qtablewidgetitem4 = projectProperties->item(0, 0);
        ___qtablewidgetitem4->setText(QApplication::translate("PrjSettingsDialog", "Project name:", 0));
        QTableWidgetItem *___qtablewidgetitem5 = projectProperties->item(1, 0);
        ___qtablewidgetitem5->setText(QApplication::translate("PrjSettingsDialog", "Project path:", 0));
        projectProperties->setSortingEnabled(__sortingEnabled);

        tabs->setTabText(tabs->indexOf(projectTab), QApplication::translate("PrjSettingsDialog", "Project", 0));
        QTableWidgetItem *___qtablewidgetitem6 = playerProperties->horizontalHeaderItem(0);
        ___qtablewidgetitem6->setText(QApplication::translate("PrjSettingsDialog", "Property", 0));
        QTableWidgetItem *___qtablewidgetitem7 = playerProperties->horizontalHeaderItem(1);
        ___qtablewidgetitem7->setText(QApplication::translate("PrjSettingsDialog", "Value", 0));
        QTableWidgetItem *___qtablewidgetitem8 = playerProperties->verticalHeaderItem(0);
        ___qtablewidgetitem8->setText(QApplication::translate("PrjSettingsDialog", "Start layer", 0));
        QTableWidgetItem *___qtablewidgetitem9 = playerProperties->verticalHeaderItem(1);
        ___qtablewidgetitem9->setText(QApplication::translate("PrjSettingsDialog", "Start tile", 0));
        QTableWidgetItem *___qtablewidgetitem10 = playerProperties->verticalHeaderItem(2);
        ___qtablewidgetitem10->setText(QApplication::translate("PrjSettingsDialog", "Hitbox dimensions", 0));
        QTableWidgetItem *___qtablewidgetitem11 = playerProperties->verticalHeaderItem(3);
        ___qtablewidgetitem11->setText(QApplication::translate("PrjSettingsDialog", "Direction", 0));
        QTableWidgetItem *___qtablewidgetitem12 = playerProperties->verticalHeaderItem(4);
        ___qtablewidgetitem12->setText(QApplication::translate("PrjSettingsDialog", "See in fog", 0));
        QTableWidgetItem *___qtablewidgetitem13 = playerProperties->verticalHeaderItem(5);
        ___qtablewidgetitem13->setText(QApplication::translate("PrjSettingsDialog", "Turn directions", 0));
        QTableWidgetItem *___qtablewidgetitem14 = playerProperties->verticalHeaderItem(6);
        ___qtablewidgetitem14->setText(QApplication::translate("PrjSettingsDialog", "Max heuristic", 0));
        QTableWidgetItem *___qtablewidgetitem15 = playerProperties->verticalHeaderItem(7);
        ___qtablewidgetitem15->setText(QApplication::translate("PrjSettingsDialog", "View distance", 0));
        QTableWidgetItem *___qtablewidgetitem16 = playerProperties->verticalHeaderItem(8);
        ___qtablewidgetitem16->setText(QApplication::translate("PrjSettingsDialog", "Move acceleration", 0));
        QTableWidgetItem *___qtablewidgetitem17 = playerProperties->verticalHeaderItem(9);
        ___qtablewidgetitem17->setText(QApplication::translate("PrjSettingsDialog", "Turn acceleration", 0));
        QTableWidgetItem *___qtablewidgetitem18 = playerProperties->verticalHeaderItem(10);
        ___qtablewidgetitem18->setText(QApplication::translate("PrjSettingsDialog", "Max move speed", 0));
        QTableWidgetItem *___qtablewidgetitem19 = playerProperties->verticalHeaderItem(11);
        ___qtablewidgetitem19->setText(QApplication::translate("PrjSettingsDialog", "Max turn speed", 0));
        QTableWidgetItem *___qtablewidgetitem20 = playerProperties->verticalHeaderItem(12);
        ___qtablewidgetitem20->setText(QApplication::translate("PrjSettingsDialog", "Idle animation", 0));
        QTableWidgetItem *___qtablewidgetitem21 = playerProperties->verticalHeaderItem(13);
        ___qtablewidgetitem21->setText(QApplication::translate("PrjSettingsDialog", "Move animation", 0));
        QTableWidgetItem *___qtablewidgetitem22 = playerProperties->verticalHeaderItem(14);
        ___qtablewidgetitem22->setText(QApplication::translate("PrjSettingsDialog", "Health", 0));

        const bool __sortingEnabled1 = playerProperties->isSortingEnabled();
        playerProperties->setSortingEnabled(false);
        QTableWidgetItem *___qtablewidgetitem23 = playerProperties->item(0, 0);
        ___qtablewidgetitem23->setText(QApplication::translate("PrjSettingsDialog", "Spawn layer:", 0));
        QTableWidgetItem *___qtablewidgetitem24 = playerProperties->item(1, 0);
        ___qtablewidgetitem24->setText(QApplication::translate("PrjSettingsDialog", "Spawn tile:", 0));
        QTableWidgetItem *___qtablewidgetitem25 = playerProperties->item(2, 0);
        ___qtablewidgetitem25->setText(QApplication::translate("PrjSettingsDialog", "Hitbox dimensions:", 0));
        QTableWidgetItem *___qtablewidgetitem26 = playerProperties->item(3, 0);
        ___qtablewidgetitem26->setText(QApplication::translate("PrjSettingsDialog", "Direction:", 0));
        QTableWidgetItem *___qtablewidgetitem27 = playerProperties->item(4, 0);
        ___qtablewidgetitem27->setText(QApplication::translate("PrjSettingsDialog", "See in fog:", 0));
        QTableWidgetItem *___qtablewidgetitem28 = playerProperties->item(5, 0);
        ___qtablewidgetitem28->setText(QApplication::translate("PrjSettingsDialog", "Turn directions:", 0));
        QTableWidgetItem *___qtablewidgetitem29 = playerProperties->item(6, 0);
        ___qtablewidgetitem29->setText(QApplication::translate("PrjSettingsDialog", "Max heuristic:", 0));
        QTableWidgetItem *___qtablewidgetitem30 = playerProperties->item(7, 0);
        ___qtablewidgetitem30->setText(QApplication::translate("PrjSettingsDialog", "View distance:", 0));
        QTableWidgetItem *___qtablewidgetitem31 = playerProperties->item(8, 0);
        ___qtablewidgetitem31->setText(QApplication::translate("PrjSettingsDialog", "Move acceleration:", 0));
        QTableWidgetItem *___qtablewidgetitem32 = playerProperties->item(9, 0);
        ___qtablewidgetitem32->setText(QApplication::translate("PrjSettingsDialog", "Turn acceleration:", 0));
        QTableWidgetItem *___qtablewidgetitem33 = playerProperties->item(10, 0);
        ___qtablewidgetitem33->setText(QApplication::translate("PrjSettingsDialog", "Max move speed:", 0));
        QTableWidgetItem *___qtablewidgetitem34 = playerProperties->item(11, 0);
        ___qtablewidgetitem34->setText(QApplication::translate("PrjSettingsDialog", "Max turn speed:", 0));
        QTableWidgetItem *___qtablewidgetitem35 = playerProperties->item(12, 0);
        ___qtablewidgetitem35->setText(QApplication::translate("PrjSettingsDialog", "Idle animation:", 0));
        QTableWidgetItem *___qtablewidgetitem36 = playerProperties->item(13, 0);
        ___qtablewidgetitem36->setText(QApplication::translate("PrjSettingsDialog", "Move animation:", 0));
        QTableWidgetItem *___qtablewidgetitem37 = playerProperties->item(14, 0);
        ___qtablewidgetitem37->setText(QApplication::translate("PrjSettingsDialog", "Health:", 0));
        playerProperties->setSortingEnabled(__sortingEnabled1);

        tabs->setTabText(tabs->indexOf(playerTab), QApplication::translate("PrjSettingsDialog", "Player", 0));
        bSave->setText(QApplication::translate("PrjSettingsDialog", "Save and Close", 0));
        bCancel->setText(QApplication::translate("PrjSettingsDialog", "Cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class PrjSettingsDialog: public Ui_PrjSettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRJSETTINGSDIALOG_H
