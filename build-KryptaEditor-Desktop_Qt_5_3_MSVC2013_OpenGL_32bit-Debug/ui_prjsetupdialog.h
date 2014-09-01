/********************************************************************************
** Form generated from reading UI file 'prjsetupdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRJSETUPDIALOG_H
#define UI_PRJSETUPDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_PrjWizard
{
public:
    QFormLayout *formLayout_2;
    QLabel *lPrjName;
    QLineEdit *tbPrjName;
    QLabel *lMapName;
    QLineEdit *tbMapName;
    QLabel *lLayers;
    QListWidget *lbLayers;
    QGridLayout *layerLayout;
    QLabel *lDimensions;
    QSpinBox *sbHeight;
    QSpinBox *sbWidth;
    QPushButton *bUp;
    QPushButton *bDown;
    QPushButton *bDelete;
    QPushButton *bAdd;
    QLineEdit *tbDescr;
    QHBoxLayout *controlLayout;
    QPushButton *bDone;
    QPushButton *bCancel;

    void setupUi(QDialog *PrjWizard)
    {
        if (PrjWizard->objectName().isEmpty())
            PrjWizard->setObjectName(QStringLiteral("PrjWizard"));
        PrjWizard->resize(435, 377);
        PrjWizard->setModal(false);
        formLayout_2 = new QFormLayout(PrjWizard);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        formLayout_2->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        lPrjName = new QLabel(PrjWizard);
        lPrjName->setObjectName(QStringLiteral("lPrjName"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, lPrjName);

        tbPrjName = new QLineEdit(PrjWizard);
        tbPrjName->setObjectName(QStringLiteral("tbPrjName"));

        formLayout_2->setWidget(0, QFormLayout::FieldRole, tbPrjName);

        lMapName = new QLabel(PrjWizard);
        lMapName->setObjectName(QStringLiteral("lMapName"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, lMapName);

        tbMapName = new QLineEdit(PrjWizard);
        tbMapName->setObjectName(QStringLiteral("tbMapName"));

        formLayout_2->setWidget(1, QFormLayout::FieldRole, tbMapName);

        lLayers = new QLabel(PrjWizard);
        lLayers->setObjectName(QStringLiteral("lLayers"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, lLayers);

        lbLayers = new QListWidget(PrjWizard);
        lbLayers->setObjectName(QStringLiteral("lbLayers"));
        lbLayers->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

        formLayout_2->setWidget(2, QFormLayout::FieldRole, lbLayers);

        layerLayout = new QGridLayout();
        layerLayout->setObjectName(QStringLiteral("layerLayout"));
        lDimensions = new QLabel(PrjWizard);
        lDimensions->setObjectName(QStringLiteral("lDimensions"));

        layerLayout->addWidget(lDimensions, 3, 0, 1, 1, Qt::AlignLeft);

        sbHeight = new QSpinBox(PrjWizard);
        sbHeight->setObjectName(QStringLiteral("sbHeight"));
        sbHeight->setMinimum(20);
        sbHeight->setMaximum(100);
        sbHeight->setValue(50);

        layerLayout->addWidget(sbHeight, 3, 2, 1, 1);

        sbWidth = new QSpinBox(PrjWizard);
        sbWidth->setObjectName(QStringLiteral("sbWidth"));
        sbWidth->setMinimum(20);
        sbWidth->setMaximum(100);
        sbWidth->setValue(50);

        layerLayout->addWidget(sbWidth, 3, 1, 1, 1);

        bUp = new QPushButton(PrjWizard);
        bUp->setObjectName(QStringLiteral("bUp"));

        layerLayout->addWidget(bUp, 4, 2, 1, 1);

        bDown = new QPushButton(PrjWizard);
        bDown->setObjectName(QStringLiteral("bDown"));

        layerLayout->addWidget(bDown, 5, 2, 1, 1);

        bDelete = new QPushButton(PrjWizard);
        bDelete->setObjectName(QStringLiteral("bDelete"));

        layerLayout->addWidget(bDelete, 5, 1, 1, 1);

        bAdd = new QPushButton(PrjWizard);
        bAdd->setObjectName(QStringLiteral("bAdd"));

        layerLayout->addWidget(bAdd, 4, 1, 1, 1);

        tbDescr = new QLineEdit(PrjWizard);
        tbDescr->setObjectName(QStringLiteral("tbDescr"));

        layerLayout->addWidget(tbDescr, 4, 0, 1, 1);


        formLayout_2->setLayout(3, QFormLayout::FieldRole, layerLayout);

        controlLayout = new QHBoxLayout();
        controlLayout->setObjectName(QStringLiteral("controlLayout"));
        bDone = new QPushButton(PrjWizard);
        bDone->setObjectName(QStringLiteral("bDone"));

        controlLayout->addWidget(bDone);

        bCancel = new QPushButton(PrjWizard);
        bCancel->setObjectName(QStringLiteral("bCancel"));

        controlLayout->addWidget(bCancel);


        formLayout_2->setLayout(4, QFormLayout::FieldRole, controlLayout);


        retranslateUi(PrjWizard);

        QMetaObject::connectSlotsByName(PrjWizard);
    } // setupUi

    void retranslateUi(QDialog *PrjWizard)
    {
        PrjWizard->setWindowTitle(QApplication::translate("PrjWizard", "Project Setup", 0));
        lPrjName->setText(QApplication::translate("PrjWizard", "Project name:", 0));
        lMapName->setText(QApplication::translate("PrjWizard", "Map name:", 0));
        lLayers->setText(QApplication::translate("PrjWizard", "Layers:", 0));
        lDimensions->setText(QApplication::translate("PrjWizard", "Layer dimensions [w, h]:", 0));
        sbHeight->setSuffix(QApplication::translate("PrjWizard", " tiles", 0));
        sbWidth->setSuffix(QApplication::translate("PrjWizard", " tiles", 0));
        bUp->setText(QApplication::translate("PrjWizard", "Move up", 0));
        bDown->setText(QApplication::translate("PrjWizard", "Move down", 0));
        bDelete->setText(QApplication::translate("PrjWizard", "Delete", 0));
        bAdd->setText(QApplication::translate("PrjWizard", "Add", 0));
#ifndef QT_NO_TOOLTIP
        tbDescr->setToolTip(QApplication::translate("PrjWizard", "Layer description", 0));
#endif // QT_NO_TOOLTIP
        tbDescr->setInputMask(QString());
        tbDescr->setPlaceholderText(QApplication::translate("PrjWizard", "Layer description...", 0));
        bDone->setText(QApplication::translate("PrjWizard", "Done", 0));
        bCancel->setText(QApplication::translate("PrjWizard", "Cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class PrjWizard: public Ui_PrjWizard {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRJSETUPDIALOG_H
