/********************************************************************************
** Form generated from reading UI file 'ConfigDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIGDIALOG_H
#define UI_CONFIGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ConfigDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QTableWidget *settings;
    QHBoxLayout *horizontalLayout;
    QPushButton *bSave;
    QSpacerItem *horizontalSpacer;
    QPushButton *bCancel;

    void setupUi(QDialog *ConfigDialog)
    {
        if (ConfigDialog->objectName().isEmpty())
            ConfigDialog->setObjectName(QStringLiteral("ConfigDialog"));
        ConfigDialog->resize(320, 340);
        verticalLayout_2 = new QVBoxLayout(ConfigDialog);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        settings = new QTableWidget(ConfigDialog);
        if (settings->columnCount() < 2)
            settings->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        settings->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        settings->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        settings->setObjectName(QStringLiteral("settings"));
        settings->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        settings->setProperty("showDropIndicator", QVariant(false));
        settings->setSelectionMode(QAbstractItemView::NoSelection);
        settings->setGridStyle(Qt::SolidLine);
        settings->setWordWrap(false);
        settings->setCornerButtonEnabled(false);
        settings->horizontalHeader()->setVisible(false);
        settings->horizontalHeader()->setStretchLastSection(true);
        settings->verticalHeader()->setVisible(false);
        settings->verticalHeader()->setStretchLastSection(false);

        verticalLayout->addWidget(settings);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        bSave = new QPushButton(ConfigDialog);
        bSave->setObjectName(QStringLiteral("bSave"));

        horizontalLayout->addWidget(bSave);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        bCancel = new QPushButton(ConfigDialog);
        bCancel->setObjectName(QStringLiteral("bCancel"));

        horizontalLayout->addWidget(bCancel);


        verticalLayout->addLayout(horizontalLayout);

        verticalLayout->setStretch(0, 95);
        verticalLayout->setStretch(1, 5);

        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(ConfigDialog);

        QMetaObject::connectSlotsByName(ConfigDialog);
    } // setupUi

    void retranslateUi(QDialog *ConfigDialog)
    {
        ConfigDialog->setWindowTitle(QApplication::translate("ConfigDialog", "Properties", 0));
        QTableWidgetItem *___qtablewidgetitem = settings->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("ConfigDialog", "Property", 0));
        QTableWidgetItem *___qtablewidgetitem1 = settings->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("ConfigDialog", "Value", 0));
        bSave->setText(QApplication::translate("ConfigDialog", "Save and Close", 0));
        bCancel->setText(QApplication::translate("ConfigDialog", "Cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class ConfigDialog: public Ui_ConfigDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGDIALOG_H
