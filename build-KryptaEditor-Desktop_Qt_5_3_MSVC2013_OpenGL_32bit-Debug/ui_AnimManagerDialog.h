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
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include "PivotView.h"

QT_BEGIN_NAMESPACE

class Ui_AnimManagerDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QComboBox *cbAnims;
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
        verticalLayout_2 = new QVBoxLayout(AnimManagerDialog);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        cbAnims = new QComboBox(AnimManagerDialog);
        cbAnims->setObjectName(QStringLiteral("cbAnims"));
        cbAnims->setMaxVisibleItems(20);
        cbAnims->setDuplicatesEnabled(true);

        verticalLayout->addWidget(cbAnims);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        gvPivot = new PivotView(AnimManagerDialog);
        gvPivot->setObjectName(QStringLiteral("gvPivot"));

        verticalLayout_4->addWidget(gvPivot);

        lbImages = new QListWidget(AnimManagerDialog);
        lbImages->setObjectName(QStringLiteral("lbImages"));
        lbImages->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        lbImages->setMovement(QListView::Snap);
        lbImages->setResizeMode(QListView::Fixed);
        lbImages->setLayoutMode(QListView::SinglePass);
        lbImages->setViewMode(QListView::IconMode);
        lbImages->setUniformItemSizes(false);

        verticalLayout_4->addWidget(lbImages);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        bChange = new QPushButton(AnimManagerDialog);
        bChange->setObjectName(QStringLiteral("bChange"));

        horizontalLayout_3->addWidget(bChange);

        bAdd = new QPushButton(AnimManagerDialog);
        bAdd->setObjectName(QStringLiteral("bAdd"));

        horizontalLayout_3->addWidget(bAdd);

        bRemove = new QPushButton(AnimManagerDialog);
        bRemove->setObjectName(QStringLiteral("bRemove"));

        horizontalLayout_3->addWidget(bRemove);

        bAnimate = new QPushButton(AnimManagerDialog);
        bAnimate->setObjectName(QStringLiteral("bAnimate"));

        horizontalLayout_3->addWidget(bAnimate);


        verticalLayout_4->addLayout(horizontalLayout_3);

        verticalLayout_4->setStretch(0, 75);
        verticalLayout_4->setStretch(1, 20);
        verticalLayout_4->setStretch(2, 5);

        horizontalLayout_2->addLayout(verticalLayout_4);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        tSheetProps = new QTableWidget(AnimManagerDialog);
        tSheetProps->setObjectName(QStringLiteral("tSheetProps"));

        verticalLayout_3->addWidget(tSheetProps);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        bSetPivot = new QPushButton(AnimManagerDialog);
        bSetPivot->setObjectName(QStringLiteral("bSetPivot"));

        horizontalLayout_4->addWidget(bSetPivot);

        bResetPivot = new QPushButton(AnimManagerDialog);
        bResetPivot->setObjectName(QStringLiteral("bResetPivot"));

        horizontalLayout_4->addWidget(bResetPivot);

        bCancelPivot = new QPushButton(AnimManagerDialog);
        bCancelPivot->setObjectName(QStringLiteral("bCancelPivot"));

        horizontalLayout_4->addWidget(bCancelPivot);


        verticalLayout_3->addLayout(horizontalLayout_4);

        verticalLayout_3->setStretch(0, 95);
        verticalLayout_3->setStretch(1, 5);

        horizontalLayout_2->addLayout(verticalLayout_3);

        horizontalLayout_2->setStretch(0, 60);
        horizontalLayout_2->setStretch(1, 40);

        verticalLayout->addLayout(horizontalLayout_2);

        tabs = new QTabWidget(AnimManagerDialog);
        tabs->setObjectName(QStringLiteral("tabs"));

        verticalLayout->addWidget(tabs);

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

        verticalLayout->setStretch(0, 10);
        verticalLayout->setStretch(1, 55);
        verticalLayout->setStretch(2, 30);
        verticalLayout->setStretch(3, 5);

        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(AnimManagerDialog);

        tabs->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(AnimManagerDialog);
    } // setupUi

    void retranslateUi(QDialog *AnimManagerDialog)
    {
        AnimManagerDialog->setWindowTitle(QApplication::translate("AnimManagerDialog", "Animation Manager", 0));
        bChange->setText(QApplication::translate("AnimManagerDialog", "Change Spritesheet", 0));
        bAdd->setText(QApplication::translate("AnimManagerDialog", "Add Image", 0));
        bRemove->setText(QApplication::translate("AnimManagerDialog", "Remove Image", 0));
        bAnimate->setText(QApplication::translate("AnimManagerDialog", "Animate", 0));
        bSetPivot->setText(QApplication::translate("AnimManagerDialog", "Set Pivot", 0));
        bResetPivot->setText(QApplication::translate("AnimManagerDialog", "Reset Pivot", 0));
        bCancelPivot->setText(QApplication::translate("AnimManagerDialog", "Cancel Pivot", 0));
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
