/********************************************************************************
** Form generated from reading UI file 'EventEditDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EVENTEDITDIALOG_H
#define UI_EVENTEDITDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EventEditDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QTreeWidget *eventTree;
    QHBoxLayout *horizontalLayout_2;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QHBoxLayout *horizontalLayout_3;
    QHBoxLayout *horizontalLayout;
    QPushButton *bSave;
    QSpacerItem *horizontalSpacer;
    QPushButton *bCancel;

    void setupUi(QDialog *EventEditDialog)
    {
        if (EventEditDialog->objectName().isEmpty())
            EventEditDialog->setObjectName(QStringLiteral("EventEditDialog"));
        EventEditDialog->resize(360, 400);
        verticalLayout_2 = new QVBoxLayout(EventEditDialog);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        eventTree = new QTreeWidget(EventEditDialog);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem(eventTree);
        __qtreewidgetitem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsEnabled);
        eventTree->setObjectName(QStringLiteral("eventTree"));
        eventTree->header()->setCascadingSectionResizes(false);
        eventTree->header()->setDefaultSectionSize(100);
        eventTree->header()->setStretchLastSection(true);

        verticalLayout->addWidget(eventTree);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        scrollArea = new QScrollArea(EventEditDialog);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 336, 69));
        horizontalLayout_3 = new QHBoxLayout(scrollAreaWidgetContents);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        scrollArea->setWidget(scrollAreaWidgetContents);

        horizontalLayout_2->addWidget(scrollArea);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        bSave = new QPushButton(EventEditDialog);
        bSave->setObjectName(QStringLiteral("bSave"));

        horizontalLayout->addWidget(bSave);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        bCancel = new QPushButton(EventEditDialog);
        bCancel->setObjectName(QStringLiteral("bCancel"));

        horizontalLayout->addWidget(bCancel);


        verticalLayout->addLayout(horizontalLayout);

        verticalLayout->setStretch(0, 80);
        verticalLayout->setStretch(1, 15);
        verticalLayout->setStretch(2, 5);

        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(EventEditDialog);

        QMetaObject::connectSlotsByName(EventEditDialog);
    } // setupUi

    void retranslateUi(QDialog *EventEditDialog)
    {
        EventEditDialog->setWindowTitle(QApplication::translate("EventEditDialog", "Dialog", 0));
        QTreeWidgetItem *___qtreewidgetitem = eventTree->headerItem();
        ___qtreewidgetitem->setText(2, QApplication::translate("EventEditDialog", "Actions", 0));
        ___qtreewidgetitem->setText(1, QApplication::translate("EventEditDialog", "Conditions", 0));
        ___qtreewidgetitem->setText(0, QApplication::translate("EventEditDialog", "Event", 0));

        const bool __sortingEnabled = eventTree->isSortingEnabled();
        eventTree->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem1 = eventTree->topLevelItem(0);
        ___qtreewidgetitem1->setText(0, QApplication::translate("EventEditDialog", "event", 0));
        eventTree->setSortingEnabled(__sortingEnabled);

        bSave->setText(QApplication::translate("EventEditDialog", "Save and Close", 0));
        bCancel->setText(QApplication::translate("EventEditDialog", "Cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class EventEditDialog: public Ui_EventEditDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EVENTEDITDIALOG_H
