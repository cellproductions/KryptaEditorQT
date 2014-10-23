/********************************************************************************
** Form generated from reading UI file 'ItemManagerDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ITEMMANAGERDIALOG_H
#define UI_ITEMMANAGERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include "csdialog.h"

QT_BEGIN_NAMESPACE

class Ui_ItemManagerDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QListWidget *lbItems;
    QTableWidget *tItemProps;
    QHBoxLayout *horizontalLayout;
    QPushButton *bDone;
    QSpacerItem *horizontalSpacer;
    QPushButton *bAdd;
    QPushButton *bDelete;

    void setupUi(CSDialog *ItemManagerDialog)
    {
        if (ItemManagerDialog->objectName().isEmpty())
            ItemManagerDialog->setObjectName(QStringLiteral("ItemManagerDialog"));
        ItemManagerDialog->resize(502, 361);
        verticalLayout_2 = new QVBoxLayout(ItemManagerDialog);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        lbItems = new QListWidget(ItemManagerDialog);
        lbItems->setObjectName(QStringLiteral("lbItems"));
        lbItems->setProperty("showDropIndicator", QVariant(false));
        lbItems->setMovement(QListView::Static);
        lbItems->setViewMode(QListView::IconMode);

        verticalLayout->addWidget(lbItems);

        tItemProps = new QTableWidget(ItemManagerDialog);
        if (tItemProps->columnCount() < 2)
            tItemProps->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tItemProps->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tItemProps->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        tItemProps->setObjectName(QStringLiteral("tItemProps"));
        tItemProps->horizontalHeader()->setVisible(false);
        tItemProps->horizontalHeader()->setStretchLastSection(true);
        tItemProps->verticalHeader()->setVisible(false);

        verticalLayout->addWidget(tItemProps);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        bDone = new QPushButton(ItemManagerDialog);
        bDone->setObjectName(QStringLiteral("bDone"));

        horizontalLayout->addWidget(bDone);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        bAdd = new QPushButton(ItemManagerDialog);
        bAdd->setObjectName(QStringLiteral("bAdd"));

        horizontalLayout->addWidget(bAdd);

        bDelete = new QPushButton(ItemManagerDialog);
        bDelete->setObjectName(QStringLiteral("bDelete"));
        bDelete->setEnabled(false);

        horizontalLayout->addWidget(bDelete);


        verticalLayout->addLayout(horizontalLayout);

        verticalLayout->setStretch(0, 75);
        verticalLayout->setStretch(1, 30);
        verticalLayout->setStretch(2, 5);

        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(ItemManagerDialog);

        QMetaObject::connectSlotsByName(ItemManagerDialog);
    } // setupUi

    void retranslateUi(CSDialog *ItemManagerDialog)
    {
        ItemManagerDialog->setWindowTitle(QApplication::translate("ItemManagerDialog", "Item Manager", 0));
        QTableWidgetItem *___qtablewidgetitem = tItemProps->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("ItemManagerDialog", "Property", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tItemProps->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("ItemManagerDialog", "Value", 0));
        bDone->setText(QApplication::translate("ItemManagerDialog", "Done", 0));
        bAdd->setText(QApplication::translate("ItemManagerDialog", "Add Item", 0));
        bDelete->setText(QApplication::translate("ItemManagerDialog", "Delete Item", 0));
    } // retranslateUi

};

namespace Ui {
    class ItemManagerDialog: public Ui_ItemManagerDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ITEMMANAGERDIALOG_H
