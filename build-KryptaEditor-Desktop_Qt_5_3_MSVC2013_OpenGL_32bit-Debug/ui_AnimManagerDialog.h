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

QT_BEGIN_NAMESPACE

class Ui_AnimManagerDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QComboBox *cbAnims;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_4;
    QListWidget *lbImages;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *bChange;
    QPushButton *bAdd;
    QPushButton *bRemove;
    QTableWidget *tSheetProps;
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


        verticalLayout_4->addLayout(horizontalLayout_3);


        horizontalLayout_2->addLayout(verticalLayout_4);

        tSheetProps = new QTableWidget(AnimManagerDialog);
        if (tSheetProps->columnCount() < 2)
            tSheetProps->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tSheetProps->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tSheetProps->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        tSheetProps->setObjectName(QStringLiteral("tSheetProps"));
        tSheetProps->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tSheetProps->horizontalHeader()->setVisible(false);
        tSheetProps->horizontalHeader()->setStretchLastSection(true);
        tSheetProps->verticalHeader()->setVisible(false);

        horizontalLayout_2->addWidget(tSheetProps);

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
        QTableWidgetItem *___qtablewidgetitem = tSheetProps->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("AnimManagerDialog", "Property", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tSheetProps->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("AnimManagerDialog", "Value", 0));
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
