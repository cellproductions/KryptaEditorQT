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
        if (playerProperties->rowCount() < 4)
            playerProperties->setRowCount(4);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        playerProperties->setVerticalHeaderItem(0, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        playerProperties->setVerticalHeaderItem(1, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        playerProperties->setVerticalHeaderItem(2, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        playerProperties->setVerticalHeaderItem(3, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        playerProperties->setItem(0, 0, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        playerProperties->setItem(1, 0, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        playerProperties->setItem(2, 0, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        playerProperties->setItem(3, 0, __qtablewidgetitem16);
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
        ___qtablewidgetitem10->setText(QApplication::translate("PrjSettingsDialog", "Speed", 0));
        QTableWidgetItem *___qtablewidgetitem11 = playerProperties->verticalHeaderItem(3);
        ___qtablewidgetitem11->setText(QApplication::translate("PrjSettingsDialog", "View distance", 0));

        const bool __sortingEnabled1 = playerProperties->isSortingEnabled();
        playerProperties->setSortingEnabled(false);
        QTableWidgetItem *___qtablewidgetitem12 = playerProperties->item(0, 0);
        ___qtablewidgetitem12->setText(QApplication::translate("PrjSettingsDialog", "Spawn layer:", 0));
        QTableWidgetItem *___qtablewidgetitem13 = playerProperties->item(1, 0);
        ___qtablewidgetitem13->setText(QApplication::translate("PrjSettingsDialog", "Spawn tile:", 0));
        QTableWidgetItem *___qtablewidgetitem14 = playerProperties->item(2, 0);
        ___qtablewidgetitem14->setText(QApplication::translate("PrjSettingsDialog", "Speed:", 0));
        QTableWidgetItem *___qtablewidgetitem15 = playerProperties->item(3, 0);
        ___qtablewidgetitem15->setText(QApplication::translate("PrjSettingsDialog", "View distance:", 0));
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
