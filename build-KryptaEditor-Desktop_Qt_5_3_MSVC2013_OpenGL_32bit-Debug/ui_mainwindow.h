/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "glpanel.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *miFileNew;
    QAction *miFileExit;
    QAction *miFileOpen;
    QAction *actionPreferences;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QHBoxLayout *mainLayout;
    QFrame *frameTool;
    Kryed::GLPanel *glWidget;
    QVBoxLayout *utilLayout;
    QGroupBox *entityGroup;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *entityLayout;
    QLabel *lEntity;
    QLabel *lEntityName;
    QPushButton *bBrowseEntities;
    QGroupBox *envGroup;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *envLayout;
    QLabel *lEnv;
    QLabel *lEnvName;
    QPushButton *bBrowseEnv;
    QGroupBox *layerGroup;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *layerLayout;
    QTableWidget *layerProperties;
    QPushButton *bLayerMan;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QToolBar *toolMain;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(800, 600);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(800, 600));
        miFileNew = new QAction(MainWindow);
        miFileNew->setObjectName(QStringLiteral("miFileNew"));
        miFileExit = new QAction(MainWindow);
        miFileExit->setObjectName(QStringLiteral("miFileExit"));
        miFileOpen = new QAction(MainWindow);
        miFileOpen->setObjectName(QStringLiteral("miFileOpen"));
        actionPreferences = new QAction(MainWindow);
        actionPreferences->setObjectName(QStringLiteral("actionPreferences"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        mainLayout = new QHBoxLayout();
        mainLayout->setSpacing(3);
        mainLayout->setObjectName(QStringLiteral("mainLayout"));
        mainLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        frameTool = new QFrame(centralWidget);
        frameTool->setObjectName(QStringLiteral("frameTool"));
        frameTool->setFrameShape(QFrame::StyledPanel);
        frameTool->setFrameShadow(QFrame::Plain);

        mainLayout->addWidget(frameTool);

        glWidget = new Kryed::GLPanel(centralWidget);
        glWidget->setObjectName(QStringLiteral("glWidget"));
        sizePolicy.setHeightForWidth(glWidget->sizePolicy().hasHeightForWidth());
        glWidget->setSizePolicy(sizePolicy);

        mainLayout->addWidget(glWidget);

        utilLayout = new QVBoxLayout();
        utilLayout->setSpacing(6);
        utilLayout->setObjectName(QStringLiteral("utilLayout"));
        entityGroup = new QGroupBox(centralWidget);
        entityGroup->setObjectName(QStringLiteral("entityGroup"));
        verticalLayout_2 = new QVBoxLayout(entityGroup);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        entityLayout = new QVBoxLayout();
        entityLayout->setSpacing(3);
        entityLayout->setObjectName(QStringLiteral("entityLayout"));
        lEntity = new QLabel(entityGroup);
        lEntity->setObjectName(QStringLiteral("lEntity"));
        lEntity->setScaledContents(true);
        lEntity->setAlignment(Qt::AlignCenter);

        entityLayout->addWidget(lEntity);

        lEntityName = new QLabel(entityGroup);
        lEntityName->setObjectName(QStringLiteral("lEntityName"));
        lEntityName->setAlignment(Qt::AlignCenter);

        entityLayout->addWidget(lEntityName);

        bBrowseEntities = new QPushButton(entityGroup);
        bBrowseEntities->setObjectName(QStringLiteral("bBrowseEntities"));

        entityLayout->addWidget(bBrowseEntities);

        entityLayout->setStretch(0, 80);
        entityLayout->setStretch(1, 10);
        entityLayout->setStretch(2, 10);

        verticalLayout_2->addLayout(entityLayout);


        utilLayout->addWidget(entityGroup);

        envGroup = new QGroupBox(centralWidget);
        envGroup->setObjectName(QStringLiteral("envGroup"));
        verticalLayout_3 = new QVBoxLayout(envGroup);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        envLayout = new QVBoxLayout();
        envLayout->setSpacing(3);
        envLayout->setObjectName(QStringLiteral("envLayout"));
        lEnv = new QLabel(envGroup);
        lEnv->setObjectName(QStringLiteral("lEnv"));
        lEnv->setScaledContents(true);
        lEnv->setAlignment(Qt::AlignCenter);

        envLayout->addWidget(lEnv);

        lEnvName = new QLabel(envGroup);
        lEnvName->setObjectName(QStringLiteral("lEnvName"));
        lEnvName->setAlignment(Qt::AlignCenter);

        envLayout->addWidget(lEnvName);

        bBrowseEnv = new QPushButton(envGroup);
        bBrowseEnv->setObjectName(QStringLiteral("bBrowseEnv"));

        envLayout->addWidget(bBrowseEnv);

        envLayout->setStretch(0, 80);
        envLayout->setStretch(1, 10);
        envLayout->setStretch(2, 10);

        verticalLayout_3->addLayout(envLayout);


        utilLayout->addWidget(envGroup);

        layerGroup = new QGroupBox(centralWidget);
        layerGroup->setObjectName(QStringLiteral("layerGroup"));
        verticalLayout_5 = new QVBoxLayout(layerGroup);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        layerLayout = new QVBoxLayout();
        layerLayout->setSpacing(3);
        layerLayout->setObjectName(QStringLiteral("layerLayout"));
        layerProperties = new QTableWidget(layerGroup);
        if (layerProperties->columnCount() < 2)
            layerProperties->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        layerProperties->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        layerProperties->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        if (layerProperties->rowCount() < 3)
            layerProperties->setRowCount(3);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        layerProperties->setVerticalHeaderItem(0, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        layerProperties->setVerticalHeaderItem(1, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        layerProperties->setVerticalHeaderItem(2, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        __qtablewidgetitem5->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
        layerProperties->setItem(0, 0, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        __qtablewidgetitem6->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
        layerProperties->setItem(1, 0, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        __qtablewidgetitem7->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
        layerProperties->setItem(2, 0, __qtablewidgetitem7);
        layerProperties->setObjectName(QStringLiteral("layerProperties"));
        layerProperties->setShowGrid(true);
        layerProperties->setWordWrap(false);
        layerProperties->setCornerButtonEnabled(false);
        layerProperties->setRowCount(3);
        layerProperties->setColumnCount(2);
        layerProperties->horizontalHeader()->setVisible(false);
        layerProperties->horizontalHeader()->setStretchLastSection(true);
        layerProperties->verticalHeader()->setVisible(false);
        layerProperties->verticalHeader()->setStretchLastSection(false);

        layerLayout->addWidget(layerProperties);

        bLayerMan = new QPushButton(layerGroup);
        bLayerMan->setObjectName(QStringLiteral("bLayerMan"));

        layerLayout->addWidget(bLayerMan);


        verticalLayout_5->addLayout(layerLayout);


        utilLayout->addWidget(layerGroup);

        utilLayout->setStretch(0, 33);
        utilLayout->setStretch(1, 33);
        utilLayout->setStretch(2, 33);

        mainLayout->addLayout(utilLayout);

        mainLayout->setStretch(0, 15);
        mainLayout->setStretch(1, 100);
        mainLayout->setStretch(2, 20);

        horizontalLayout->addLayout(mainLayout);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 800, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        MainWindow->setMenuBar(menuBar);
        toolMain = new QToolBar(MainWindow);
        toolMain->setObjectName(QStringLiteral("toolMain"));
        toolMain->setMovable(false);
        MainWindow->addToolBar(Qt::TopToolBarArea, toolMain);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuFile->addAction(miFileNew);
        menuFile->addAction(miFileOpen);
        menuFile->addAction(miFileExit);
        menuEdit->addAction(actionPreferences);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Krypta Map Editor", 0));
        miFileNew->setText(QApplication::translate("MainWindow", "New", 0));
        miFileExit->setText(QApplication::translate("MainWindow", "Exit", 0));
        miFileOpen->setText(QApplication::translate("MainWindow", "Open", 0));
        actionPreferences->setText(QApplication::translate("MainWindow", "Preferences", 0));
        entityGroup->setTitle(QApplication::translate("MainWindow", "Entity", 0));
        lEntity->setText(QString());
        lEntityName->setText(QApplication::translate("MainWindow", "None", 0));
        bBrowseEntities->setText(QApplication::translate("MainWindow", "Browse entities", 0));
        envGroup->setTitle(QApplication::translate("MainWindow", "Environment", 0));
        lEnv->setText(QString());
        lEnvName->setText(QApplication::translate("MainWindow", "None", 0));
        bBrowseEnv->setText(QApplication::translate("MainWindow", "Browse env", 0));
        layerGroup->setTitle(QApplication::translate("MainWindow", "Layer", 0));
        QTableWidgetItem *___qtablewidgetitem = layerProperties->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MainWindow", "Property", 0));
        QTableWidgetItem *___qtablewidgetitem1 = layerProperties->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("MainWindow", "Value", 0));
        QTableWidgetItem *___qtablewidgetitem2 = layerProperties->verticalHeaderItem(0);
        ___qtablewidgetitem2->setText(QApplication::translate("MainWindow", "Descr", 0));
        QTableWidgetItem *___qtablewidgetitem3 = layerProperties->verticalHeaderItem(1);
        ___qtablewidgetitem3->setText(QApplication::translate("MainWindow", "Width", 0));
        QTableWidgetItem *___qtablewidgetitem4 = layerProperties->verticalHeaderItem(2);
        ___qtablewidgetitem4->setText(QApplication::translate("MainWindow", "Height", 0));

        const bool __sortingEnabled = layerProperties->isSortingEnabled();
        layerProperties->setSortingEnabled(false);
        QTableWidgetItem *___qtablewidgetitem5 = layerProperties->item(0, 0);
        ___qtablewidgetitem5->setText(QApplication::translate("MainWindow", "Description:", 0));
        QTableWidgetItem *___qtablewidgetitem6 = layerProperties->item(1, 0);
        ___qtablewidgetitem6->setText(QApplication::translate("MainWindow", "Width:", 0));
        QTableWidgetItem *___qtablewidgetitem7 = layerProperties->item(2, 0);
        ___qtablewidgetitem7->setText(QApplication::translate("MainWindow", "Height:", 0));
        layerProperties->setSortingEnabled(__sortingEnabled);

        bLayerMan->setText(QApplication::translate("MainWindow", "Layer manager", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", 0));
        toolMain->setWindowTitle(QApplication::translate("MainWindow", "Krypta Map Editor", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
