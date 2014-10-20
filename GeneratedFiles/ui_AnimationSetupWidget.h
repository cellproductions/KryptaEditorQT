/********************************************************************************
** Form generated from reading UI file 'AnimationSetupWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ANIMATIONSETUPWIDGET_H
#define UI_ANIMATIONSETUPWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "PivotView.h"

QT_BEGIN_NAMESPACE

class Ui_AnimationSetupWidget
{
public:
    QVBoxLayout *verticalLayout;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_4;
    PivotView *gvPivot;
    QListWidget *lbImages;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *bChange;
    QPushButton *bAdd;
    QPushButton *bRemove;
    QPushButton *bAnimate;
    QVBoxLayout *verticalLayout_3;
    QTableWidget *tSheetProps;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *bSetPivot;
    QPushButton *bResetPivot;
    QPushButton *bCancelPivot;
    QTabWidget *tabs;

    void setupUi(QWidget *AnimationSetupWidget)
    {
        if (AnimationSetupWidget->objectName().isEmpty())
            AnimationSetupWidget->setObjectName(QStringLiteral("AnimationSetupWidget"));
        AnimationSetupWidget->resize(880, 700);
        verticalLayout = new QVBoxLayout(AnimationSetupWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        gvPivot = new PivotView(AnimationSetupWidget);
        gvPivot->setObjectName(QStringLiteral("gvPivot"));

        verticalLayout_4->addWidget(gvPivot);

        lbImages = new QListWidget(AnimationSetupWidget);
        lbImages->setObjectName(QStringLiteral("lbImages"));
        lbImages->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        lbImages->setEditTriggers(QAbstractItemView::AllEditTriggers);
        lbImages->setProperty("showDropIndicator", QVariant(false));
        lbImages->setDragDropMode(QAbstractItemView::NoDragDrop);
        lbImages->setMovement(QListView::Static);
        lbImages->setResizeMode(QListView::Fixed);
        lbImages->setLayoutMode(QListView::SinglePass);
        lbImages->setViewMode(QListView::IconMode);
        lbImages->setUniformItemSizes(false);

        verticalLayout_4->addWidget(lbImages);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        bChange = new QPushButton(AnimationSetupWidget);
        bChange->setObjectName(QStringLiteral("bChange"));

        horizontalLayout_3->addWidget(bChange);

        bAdd = new QPushButton(AnimationSetupWidget);
        bAdd->setObjectName(QStringLiteral("bAdd"));

        horizontalLayout_3->addWidget(bAdd);

        bRemove = new QPushButton(AnimationSetupWidget);
        bRemove->setObjectName(QStringLiteral("bRemove"));
        bRemove->setEnabled(false);

        horizontalLayout_3->addWidget(bRemove);

        bAnimate = new QPushButton(AnimationSetupWidget);
        bAnimate->setObjectName(QStringLiteral("bAnimate"));

        horizontalLayout_3->addWidget(bAnimate);


        verticalLayout_4->addLayout(horizontalLayout_3);

        verticalLayout_4->setStretch(0, 75);
        verticalLayout_4->setStretch(1, 20);
        verticalLayout_4->setStretch(2, 5);

        horizontalLayout_2->addLayout(verticalLayout_4);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        tSheetProps = new QTableWidget(AnimationSetupWidget);
        if (tSheetProps->columnCount() < 2)
            tSheetProps->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tSheetProps->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tSheetProps->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        tSheetProps->setObjectName(QStringLiteral("tSheetProps"));
        tSheetProps->horizontalHeader()->setVisible(false);
        tSheetProps->horizontalHeader()->setStretchLastSection(true);
        tSheetProps->verticalHeader()->setVisible(false);

        verticalLayout_3->addWidget(tSheetProps);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        bSetPivot = new QPushButton(AnimationSetupWidget);
        bSetPivot->setObjectName(QStringLiteral("bSetPivot"));

        horizontalLayout_4->addWidget(bSetPivot);

        bResetPivot = new QPushButton(AnimationSetupWidget);
        bResetPivot->setObjectName(QStringLiteral("bResetPivot"));

        horizontalLayout_4->addWidget(bResetPivot);

        bCancelPivot = new QPushButton(AnimationSetupWidget);
        bCancelPivot->setObjectName(QStringLiteral("bCancelPivot"));

        horizontalLayout_4->addWidget(bCancelPivot);


        verticalLayout_3->addLayout(horizontalLayout_4);

        verticalLayout_3->setStretch(0, 95);
        verticalLayout_3->setStretch(1, 5);

        horizontalLayout_2->addLayout(verticalLayout_3);

        horizontalLayout_2->setStretch(0, 60);
        horizontalLayout_2->setStretch(1, 40);

        verticalLayout_5->addLayout(horizontalLayout_2);

        tabs = new QTabWidget(AnimationSetupWidget);
        tabs->setObjectName(QStringLiteral("tabs"));

        verticalLayout_5->addWidget(tabs);

        verticalLayout_5->setStretch(0, 70);
        verticalLayout_5->setStretch(1, 30);

        verticalLayout->addLayout(verticalLayout_5);


        retranslateUi(AnimationSetupWidget);

        tabs->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(AnimationSetupWidget);
    } // setupUi

    void retranslateUi(QWidget *AnimationSetupWidget)
    {
        AnimationSetupWidget->setWindowTitle(QApplication::translate("AnimationSetupWidget", "Animation Setup", 0));
        bChange->setText(QApplication::translate("AnimationSetupWidget", "Change Spritesheet", 0));
        bAdd->setText(QApplication::translate("AnimationSetupWidget", "Add Image", 0));
        bRemove->setText(QApplication::translate("AnimationSetupWidget", "Remove Image", 0));
        bAnimate->setText(QApplication::translate("AnimationSetupWidget", "Animate", 0));
        QTableWidgetItem *___qtablewidgetitem = tSheetProps->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("AnimationSetupWidget", "Property", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tSheetProps->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("AnimationSetupWidget", "Value", 0));
        bSetPivot->setText(QApplication::translate("AnimationSetupWidget", "Set Pivot", 0));
        bResetPivot->setText(QApplication::translate("AnimationSetupWidget", "Reset Pivot", 0));
        bCancelPivot->setText(QApplication::translate("AnimationSetupWidget", "Cancel Pivot", 0));
    } // retranslateUi

};

namespace Ui {
    class AnimationSetupWidget: public Ui_AnimationSetupWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ANIMATIONSETUPWIDGET_H
