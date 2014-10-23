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
        if (projectProperties->rowCount() < 9)
            projectProperties->setRowCount(9);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        projectProperties->setVerticalHeaderItem(0, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        projectProperties->setVerticalHeaderItem(1, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        projectProperties->setVerticalHeaderItem(2, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        projectProperties->setVerticalHeaderItem(3, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        projectProperties->setVerticalHeaderItem(4, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        projectProperties->setVerticalHeaderItem(5, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        projectProperties->setVerticalHeaderItem(6, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        projectProperties->setVerticalHeaderItem(7, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        projectProperties->setVerticalHeaderItem(8, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        projectProperties->setItem(0, 0, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        projectProperties->setItem(0, 1, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        projectProperties->setItem(1, 0, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        projectProperties->setItem(2, 0, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        projectProperties->setItem(3, 0, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        projectProperties->setItem(4, 0, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        projectProperties->setItem(5, 0, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        projectProperties->setItem(6, 0, __qtablewidgetitem18);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        projectProperties->setItem(7, 0, __qtablewidgetitem19);
        QTableWidgetItem *__qtablewidgetitem20 = new QTableWidgetItem();
        projectProperties->setItem(8, 0, __qtablewidgetitem20);
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
        QTableWidgetItem *__qtablewidgetitem21 = new QTableWidgetItem();
        playerProperties->setHorizontalHeaderItem(0, __qtablewidgetitem21);
        QTableWidgetItem *__qtablewidgetitem22 = new QTableWidgetItem();
        playerProperties->setHorizontalHeaderItem(1, __qtablewidgetitem22);
        if (playerProperties->rowCount() < 18)
            playerProperties->setRowCount(18);
        QTableWidgetItem *__qtablewidgetitem23 = new QTableWidgetItem();
        playerProperties->setVerticalHeaderItem(0, __qtablewidgetitem23);
        QTableWidgetItem *__qtablewidgetitem24 = new QTableWidgetItem();
        playerProperties->setVerticalHeaderItem(1, __qtablewidgetitem24);
        QTableWidgetItem *__qtablewidgetitem25 = new QTableWidgetItem();
        playerProperties->setVerticalHeaderItem(2, __qtablewidgetitem25);
        QTableWidgetItem *__qtablewidgetitem26 = new QTableWidgetItem();
        playerProperties->setVerticalHeaderItem(3, __qtablewidgetitem26);
        QTableWidgetItem *__qtablewidgetitem27 = new QTableWidgetItem();
        playerProperties->setVerticalHeaderItem(4, __qtablewidgetitem27);
        QTableWidgetItem *__qtablewidgetitem28 = new QTableWidgetItem();
        playerProperties->setVerticalHeaderItem(5, __qtablewidgetitem28);
        QTableWidgetItem *__qtablewidgetitem29 = new QTableWidgetItem();
        playerProperties->setVerticalHeaderItem(6, __qtablewidgetitem29);
        QTableWidgetItem *__qtablewidgetitem30 = new QTableWidgetItem();
        playerProperties->setVerticalHeaderItem(7, __qtablewidgetitem30);
        QTableWidgetItem *__qtablewidgetitem31 = new QTableWidgetItem();
        playerProperties->setVerticalHeaderItem(8, __qtablewidgetitem31);
        QTableWidgetItem *__qtablewidgetitem32 = new QTableWidgetItem();
        playerProperties->setVerticalHeaderItem(9, __qtablewidgetitem32);
        QTableWidgetItem *__qtablewidgetitem33 = new QTableWidgetItem();
        playerProperties->setVerticalHeaderItem(10, __qtablewidgetitem33);
        QTableWidgetItem *__qtablewidgetitem34 = new QTableWidgetItem();
        playerProperties->setVerticalHeaderItem(11, __qtablewidgetitem34);
        QTableWidgetItem *__qtablewidgetitem35 = new QTableWidgetItem();
        playerProperties->setVerticalHeaderItem(12, __qtablewidgetitem35);
        QTableWidgetItem *__qtablewidgetitem36 = new QTableWidgetItem();
        playerProperties->setVerticalHeaderItem(13, __qtablewidgetitem36);
        QTableWidgetItem *__qtablewidgetitem37 = new QTableWidgetItem();
        playerProperties->setVerticalHeaderItem(14, __qtablewidgetitem37);
        QTableWidgetItem *__qtablewidgetitem38 = new QTableWidgetItem();
        playerProperties->setVerticalHeaderItem(15, __qtablewidgetitem38);
        QTableWidgetItem *__qtablewidgetitem39 = new QTableWidgetItem();
        playerProperties->setVerticalHeaderItem(16, __qtablewidgetitem39);
        QTableWidgetItem *__qtablewidgetitem40 = new QTableWidgetItem();
        playerProperties->setVerticalHeaderItem(17, __qtablewidgetitem40);
        QTableWidgetItem *__qtablewidgetitem41 = new QTableWidgetItem();
        playerProperties->setItem(0, 0, __qtablewidgetitem41);
        QTableWidgetItem *__qtablewidgetitem42 = new QTableWidgetItem();
        playerProperties->setItem(1, 0, __qtablewidgetitem42);
        QTableWidgetItem *__qtablewidgetitem43 = new QTableWidgetItem();
        playerProperties->setItem(2, 0, __qtablewidgetitem43);
        QTableWidgetItem *__qtablewidgetitem44 = new QTableWidgetItem();
        playerProperties->setItem(3, 0, __qtablewidgetitem44);
        QTableWidgetItem *__qtablewidgetitem45 = new QTableWidgetItem();
        playerProperties->setItem(4, 0, __qtablewidgetitem45);
        QTableWidgetItem *__qtablewidgetitem46 = new QTableWidgetItem();
        playerProperties->setItem(5, 0, __qtablewidgetitem46);
        QTableWidgetItem *__qtablewidgetitem47 = new QTableWidgetItem();
        playerProperties->setItem(6, 0, __qtablewidgetitem47);
        QTableWidgetItem *__qtablewidgetitem48 = new QTableWidgetItem();
        playerProperties->setItem(7, 0, __qtablewidgetitem48);
        QTableWidgetItem *__qtablewidgetitem49 = new QTableWidgetItem();
        playerProperties->setItem(7, 1, __qtablewidgetitem49);
        QTableWidgetItem *__qtablewidgetitem50 = new QTableWidgetItem();
        playerProperties->setItem(8, 0, __qtablewidgetitem50);
        QTableWidgetItem *__qtablewidgetitem51 = new QTableWidgetItem();
        playerProperties->setItem(9, 0, __qtablewidgetitem51);
        QTableWidgetItem *__qtablewidgetitem52 = new QTableWidgetItem();
        playerProperties->setItem(9, 1, __qtablewidgetitem52);
        QTableWidgetItem *__qtablewidgetitem53 = new QTableWidgetItem();
        playerProperties->setItem(10, 0, __qtablewidgetitem53);
        QTableWidgetItem *__qtablewidgetitem54 = new QTableWidgetItem();
        playerProperties->setItem(10, 1, __qtablewidgetitem54);
        QTableWidgetItem *__qtablewidgetitem55 = new QTableWidgetItem();
        playerProperties->setItem(11, 0, __qtablewidgetitem55);
        QTableWidgetItem *__qtablewidgetitem56 = new QTableWidgetItem();
        playerProperties->setItem(11, 1, __qtablewidgetitem56);
        QTableWidgetItem *__qtablewidgetitem57 = new QTableWidgetItem();
        playerProperties->setItem(12, 0, __qtablewidgetitem57);
        QTableWidgetItem *__qtablewidgetitem58 = new QTableWidgetItem();
        playerProperties->setItem(12, 1, __qtablewidgetitem58);
        QTableWidgetItem *__qtablewidgetitem59 = new QTableWidgetItem();
        playerProperties->setItem(13, 0, __qtablewidgetitem59);
        QTableWidgetItem *__qtablewidgetitem60 = new QTableWidgetItem();
        playerProperties->setItem(13, 1, __qtablewidgetitem60);
        QTableWidgetItem *__qtablewidgetitem61 = new QTableWidgetItem();
        playerProperties->setItem(14, 0, __qtablewidgetitem61);
        QTableWidgetItem *__qtablewidgetitem62 = new QTableWidgetItem();
        playerProperties->setItem(15, 0, __qtablewidgetitem62);
        QTableWidgetItem *__qtablewidgetitem63 = new QTableWidgetItem();
        playerProperties->setItem(15, 1, __qtablewidgetitem63);
        QTableWidgetItem *__qtablewidgetitem64 = new QTableWidgetItem();
        playerProperties->setItem(16, 0, __qtablewidgetitem64);
        QTableWidgetItem *__qtablewidgetitem65 = new QTableWidgetItem();
        playerProperties->setItem(17, 0, __qtablewidgetitem65);
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
        QTableWidgetItem *___qtablewidgetitem4 = projectProperties->verticalHeaderItem(2);
        ___qtablewidgetitem4->setText(QApplication::translate("PrjSettingsDialog", "Icon", 0));
        QTableWidgetItem *___qtablewidgetitem5 = projectProperties->verticalHeaderItem(3);
        ___qtablewidgetitem5->setText(QApplication::translate("PrjSettingsDialog", "FoW", 0));
        QTableWidgetItem *___qtablewidgetitem6 = projectProperties->verticalHeaderItem(4);
        ___qtablewidgetitem6->setText(QApplication::translate("PrjSettingsDialog", "RFoW", 0));
        QTableWidgetItem *___qtablewidgetitem7 = projectProperties->verticalHeaderItem(5);
        ___qtablewidgetitem7->setText(QApplication::translate("PrjSettingsDialog", "Fog tint", 0));
        QTableWidgetItem *___qtablewidgetitem8 = projectProperties->verticalHeaderItem(6);
        ___qtablewidgetitem8->setText(QApplication::translate("PrjSettingsDialog", "Floor fade time", 0));
        QTableWidgetItem *___qtablewidgetitem9 = projectProperties->verticalHeaderItem(7);
        ___qtablewidgetitem9->setText(QApplication::translate("PrjSettingsDialog", "Loading sound", 0));
        QTableWidgetItem *___qtablewidgetitem10 = projectProperties->verticalHeaderItem(8);
        ___qtablewidgetitem10->setText(QApplication::translate("PrjSettingsDialog", "Loading image", 0));

        const bool __sortingEnabled = projectProperties->isSortingEnabled();
        projectProperties->setSortingEnabled(false);
        QTableWidgetItem *___qtablewidgetitem11 = projectProperties->item(0, 0);
        ___qtablewidgetitem11->setText(QApplication::translate("PrjSettingsDialog", "Project name:", 0));
        QTableWidgetItem *___qtablewidgetitem12 = projectProperties->item(1, 0);
        ___qtablewidgetitem12->setText(QApplication::translate("PrjSettingsDialog", "Project path:", 0));
        QTableWidgetItem *___qtablewidgetitem13 = projectProperties->item(2, 0);
        ___qtablewidgetitem13->setText(QApplication::translate("PrjSettingsDialog", "Icon:", 0));
        QTableWidgetItem *___qtablewidgetitem14 = projectProperties->item(3, 0);
        ___qtablewidgetitem14->setText(QApplication::translate("PrjSettingsDialog", "Fog of War:", 0));
        QTableWidgetItem *___qtablewidgetitem15 = projectProperties->item(4, 0);
        ___qtablewidgetitem15->setText(QApplication::translate("PrjSettingsDialog", "Reveal Fog of War:", 0));
        QTableWidgetItem *___qtablewidgetitem16 = projectProperties->item(5, 0);
        ___qtablewidgetitem16->setText(QApplication::translate("PrjSettingsDialog", "Fog Tint:", 0));
        QTableWidgetItem *___qtablewidgetitem17 = projectProperties->item(6, 0);
        ___qtablewidgetitem17->setText(QApplication::translate("PrjSettingsDialog", "Floor Fade Time:", 0));
        QTableWidgetItem *___qtablewidgetitem18 = projectProperties->item(7, 0);
        ___qtablewidgetitem18->setText(QApplication::translate("PrjSettingsDialog", "Loading Sound:", 0));
        QTableWidgetItem *___qtablewidgetitem19 = projectProperties->item(8, 0);
        ___qtablewidgetitem19->setText(QApplication::translate("PrjSettingsDialog", "Loading Image:", 0));
        projectProperties->setSortingEnabled(__sortingEnabled);

        tabs->setTabText(tabs->indexOf(projectTab), QApplication::translate("PrjSettingsDialog", "Project", 0));
        QTableWidgetItem *___qtablewidgetitem20 = playerProperties->horizontalHeaderItem(0);
        ___qtablewidgetitem20->setText(QApplication::translate("PrjSettingsDialog", "Property", 0));
        QTableWidgetItem *___qtablewidgetitem21 = playerProperties->horizontalHeaderItem(1);
        ___qtablewidgetitem21->setText(QApplication::translate("PrjSettingsDialog", "Value", 0));
        QTableWidgetItem *___qtablewidgetitem22 = playerProperties->verticalHeaderItem(0);
        ___qtablewidgetitem22->setText(QApplication::translate("PrjSettingsDialog", "Start floor", 0));
        QTableWidgetItem *___qtablewidgetitem23 = playerProperties->verticalHeaderItem(1);
        ___qtablewidgetitem23->setText(QApplication::translate("PrjSettingsDialog", "Start tile", 0));
        QTableWidgetItem *___qtablewidgetitem24 = playerProperties->verticalHeaderItem(2);
        ___qtablewidgetitem24->setText(QApplication::translate("PrjSettingsDialog", "Hitbox dimensions", 0));
        QTableWidgetItem *___qtablewidgetitem25 = playerProperties->verticalHeaderItem(3);
        ___qtablewidgetitem25->setText(QApplication::translate("PrjSettingsDialog", "Directions", 0));
        QTableWidgetItem *___qtablewidgetitem26 = playerProperties->verticalHeaderItem(4);
        ___qtablewidgetitem26->setText(QApplication::translate("PrjSettingsDialog", "See in fog", 0));
        QTableWidgetItem *___qtablewidgetitem27 = playerProperties->verticalHeaderItem(5);
        ___qtablewidgetitem27->setText(QApplication::translate("PrjSettingsDialog", "Turn directions", 0));
        QTableWidgetItem *___qtablewidgetitem28 = playerProperties->verticalHeaderItem(6);
        ___qtablewidgetitem28->setText(QApplication::translate("PrjSettingsDialog", "Max heuristic", 0));
        QTableWidgetItem *___qtablewidgetitem29 = playerProperties->verticalHeaderItem(7);
        ___qtablewidgetitem29->setText(QApplication::translate("PrjSettingsDialog", "View distance", 0));
        QTableWidgetItem *___qtablewidgetitem30 = playerProperties->verticalHeaderItem(8);
        ___qtablewidgetitem30->setText(QApplication::translate("PrjSettingsDialog", "Move acceleration", 0));
        QTableWidgetItem *___qtablewidgetitem31 = playerProperties->verticalHeaderItem(9);
        ___qtablewidgetitem31->setText(QApplication::translate("PrjSettingsDialog", "Turn acceleration", 0));
        QTableWidgetItem *___qtablewidgetitem32 = playerProperties->verticalHeaderItem(10);
        ___qtablewidgetitem32->setText(QApplication::translate("PrjSettingsDialog", "Max move speed", 0));
        QTableWidgetItem *___qtablewidgetitem33 = playerProperties->verticalHeaderItem(11);
        ___qtablewidgetitem33->setText(QApplication::translate("PrjSettingsDialog", "Max turn speed", 0));
        QTableWidgetItem *___qtablewidgetitem34 = playerProperties->verticalHeaderItem(12);
        ___qtablewidgetitem34->setText(QApplication::translate("PrjSettingsDialog", "Idle animation", 0));
        QTableWidgetItem *___qtablewidgetitem35 = playerProperties->verticalHeaderItem(13);
        ___qtablewidgetitem35->setText(QApplication::translate("PrjSettingsDialog", "Move animation", 0));
        QTableWidgetItem *___qtablewidgetitem36 = playerProperties->verticalHeaderItem(14);
        ___qtablewidgetitem36->setText(QApplication::translate("PrjSettingsDialog", "Death animation", 0));
        QTableWidgetItem *___qtablewidgetitem37 = playerProperties->verticalHeaderItem(15);
        ___qtablewidgetitem37->setText(QApplication::translate("PrjSettingsDialog", "Health", 0));
        QTableWidgetItem *___qtablewidgetitem38 = playerProperties->verticalHeaderItem(16);
        ___qtablewidgetitem38->setText(QApplication::translate("PrjSettingsDialog", "Direction", 0));
        QTableWidgetItem *___qtablewidgetitem39 = playerProperties->verticalHeaderItem(17);
        ___qtablewidgetitem39->setText(QApplication::translate("PrjSettingsDialog", "Group", 0));

        const bool __sortingEnabled1 = playerProperties->isSortingEnabled();
        playerProperties->setSortingEnabled(false);
        QTableWidgetItem *___qtablewidgetitem40 = playerProperties->item(0, 0);
        ___qtablewidgetitem40->setText(QApplication::translate("PrjSettingsDialog", "Spawn floor:", 0));
        QTableWidgetItem *___qtablewidgetitem41 = playerProperties->item(1, 0);
        ___qtablewidgetitem41->setText(QApplication::translate("PrjSettingsDialog", "Spawn tile:", 0));
        QTableWidgetItem *___qtablewidgetitem42 = playerProperties->item(2, 0);
        ___qtablewidgetitem42->setText(QApplication::translate("PrjSettingsDialog", "Hitbox dimensions:", 0));
        QTableWidgetItem *___qtablewidgetitem43 = playerProperties->item(3, 0);
        ___qtablewidgetitem43->setText(QApplication::translate("PrjSettingsDialog", "Directions:", 0));
        QTableWidgetItem *___qtablewidgetitem44 = playerProperties->item(4, 0);
        ___qtablewidgetitem44->setText(QApplication::translate("PrjSettingsDialog", "See in fog:", 0));
        QTableWidgetItem *___qtablewidgetitem45 = playerProperties->item(5, 0);
        ___qtablewidgetitem45->setText(QApplication::translate("PrjSettingsDialog", "Turn directions:", 0));
        QTableWidgetItem *___qtablewidgetitem46 = playerProperties->item(6, 0);
        ___qtablewidgetitem46->setText(QApplication::translate("PrjSettingsDialog", "Max heuristic:", 0));
        QTableWidgetItem *___qtablewidgetitem47 = playerProperties->item(7, 0);
        ___qtablewidgetitem47->setText(QApplication::translate("PrjSettingsDialog", "View distance:", 0));
        QTableWidgetItem *___qtablewidgetitem48 = playerProperties->item(8, 0);
        ___qtablewidgetitem48->setText(QApplication::translate("PrjSettingsDialog", "Move acceleration:", 0));
        QTableWidgetItem *___qtablewidgetitem49 = playerProperties->item(9, 0);
        ___qtablewidgetitem49->setText(QApplication::translate("PrjSettingsDialog", "Turn acceleration:", 0));
        QTableWidgetItem *___qtablewidgetitem50 = playerProperties->item(10, 0);
        ___qtablewidgetitem50->setText(QApplication::translate("PrjSettingsDialog", "Max move speed:", 0));
        QTableWidgetItem *___qtablewidgetitem51 = playerProperties->item(11, 0);
        ___qtablewidgetitem51->setText(QApplication::translate("PrjSettingsDialog", "Max turn speed:", 0));
        QTableWidgetItem *___qtablewidgetitem52 = playerProperties->item(12, 0);
        ___qtablewidgetitem52->setText(QApplication::translate("PrjSettingsDialog", "Idle animation:", 0));
        QTableWidgetItem *___qtablewidgetitem53 = playerProperties->item(13, 0);
        ___qtablewidgetitem53->setText(QApplication::translate("PrjSettingsDialog", "Move animation:", 0));
        QTableWidgetItem *___qtablewidgetitem54 = playerProperties->item(14, 0);
        ___qtablewidgetitem54->setText(QApplication::translate("PrjSettingsDialog", "Death animation:", 0));
        QTableWidgetItem *___qtablewidgetitem55 = playerProperties->item(15, 0);
        ___qtablewidgetitem55->setText(QApplication::translate("PrjSettingsDialog", "Health:", 0));
        QTableWidgetItem *___qtablewidgetitem56 = playerProperties->item(16, 0);
        ___qtablewidgetitem56->setText(QApplication::translate("PrjSettingsDialog", "Direction:", 0));
        QTableWidgetItem *___qtablewidgetitem57 = playerProperties->item(17, 0);
        ___qtablewidgetitem57->setText(QApplication::translate("PrjSettingsDialog", "Group:", 0));
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
