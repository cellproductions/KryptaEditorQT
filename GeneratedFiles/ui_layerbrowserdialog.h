/********************************************************************************
** Form generated from reading UI file 'layerbrowserdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LAYERBROWSERDIALOG_H
#define UI_LAYERBROWSERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_LayerBrowserDialog
{
public:
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QListWidget *lbLayers;
    QVBoxLayout *verticalLayout_2;
    QLineEdit *tbDescription;
    QLabel *label;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_4;
    QSpinBox *sbWidth;
    QPushButton *bAdd;
    QPushButton *bDelete;
    QPushButton *bChange;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout_5;
    QSpinBox *sbHeight;
    QPushButton *bUp;
    QPushButton *bDown;
    QSpacerItem *verticalSpacer_3;
    QHBoxLayout *horizontalLayout;
    QPushButton *bSelect;
    QSpacerItem *horizontalSpacer;
    QPushButton *bClose;

    void setupUi(QDialog *LayerBrowserDialog)
    {
        if (LayerBrowserDialog->objectName().isEmpty())
            LayerBrowserDialog->setObjectName(QStringLiteral("LayerBrowserDialog"));
        LayerBrowserDialog->resize(340, 280);
        verticalLayout_3 = new QVBoxLayout(LayerBrowserDialog);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        lbLayers = new QListWidget(LayerBrowserDialog);
        lbLayers->setObjectName(QStringLiteral("lbLayers"));

        horizontalLayout_2->addWidget(lbLayers);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        tbDescription = new QLineEdit(LayerBrowserDialog);
        tbDescription->setObjectName(QStringLiteral("tbDescription"));

        verticalLayout_2->addWidget(tbDescription);

        label = new QLabel(LayerBrowserDialog);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout_2->addWidget(label);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(1);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        sbWidth = new QSpinBox(LayerBrowserDialog);
        sbWidth->setObjectName(QStringLiteral("sbWidth"));
        sbWidth->setMinimum(10);
        sbWidth->setMaximum(100);
        sbWidth->setValue(50);

        verticalLayout_4->addWidget(sbWidth);

        bAdd = new QPushButton(LayerBrowserDialog);
        bAdd->setObjectName(QStringLiteral("bAdd"));

        verticalLayout_4->addWidget(bAdd);

        bDelete = new QPushButton(LayerBrowserDialog);
        bDelete->setObjectName(QStringLiteral("bDelete"));

        verticalLayout_4->addWidget(bDelete);

        bChange = new QPushButton(LayerBrowserDialog);
        bChange->setObjectName(QStringLiteral("bChange"));

        verticalLayout_4->addWidget(bChange);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer);

        verticalLayout_4->setStretch(0, 35);
        verticalLayout_4->setStretch(1, 30);
        verticalLayout_4->setStretch(2, 30);

        horizontalLayout_3->addLayout(verticalLayout_4);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        sbHeight = new QSpinBox(LayerBrowserDialog);
        sbHeight->setObjectName(QStringLiteral("sbHeight"));
        sbHeight->setMinimum(10);
        sbHeight->setMaximum(100);
        sbHeight->setValue(50);

        verticalLayout_5->addWidget(sbHeight);

        bUp = new QPushButton(LayerBrowserDialog);
        bUp->setObjectName(QStringLiteral("bUp"));

        verticalLayout_5->addWidget(bUp);

        bDown = new QPushButton(LayerBrowserDialog);
        bDown->setObjectName(QStringLiteral("bDown"));

        verticalLayout_5->addWidget(bDown);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_3);

        verticalLayout_5->setStretch(0, 5);
        verticalLayout_5->setStretch(1, 5);
        verticalLayout_5->setStretch(2, 5);
        verticalLayout_5->setStretch(3, 74);

        horizontalLayout_3->addLayout(verticalLayout_5);

        horizontalLayout_3->setStretch(0, 50);
        horizontalLayout_3->setStretch(1, 50);

        verticalLayout_2->addLayout(horizontalLayout_3);

        verticalLayout_2->setStretch(0, 5);

        horizontalLayout_2->addLayout(verticalLayout_2);

        horizontalLayout_2->setStretch(0, 50);
        horizontalLayout_2->setStretch(1, 50);

        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        bSelect = new QPushButton(LayerBrowserDialog);
        bSelect->setObjectName(QStringLiteral("bSelect"));

        horizontalLayout->addWidget(bSelect);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        bClose = new QPushButton(LayerBrowserDialog);
        bClose->setObjectName(QStringLiteral("bClose"));

        horizontalLayout->addWidget(bClose);

        horizontalLayout->setStretch(0, 20);
        horizontalLayout->setStretch(1, 60);
        horizontalLayout->setStretch(2, 20);

        verticalLayout->addLayout(horizontalLayout);

        verticalLayout->setStretch(0, 95);
        verticalLayout->setStretch(1, 5);

        verticalLayout_3->addLayout(verticalLayout);


        retranslateUi(LayerBrowserDialog);

        QMetaObject::connectSlotsByName(LayerBrowserDialog);
    } // setupUi

    void retranslateUi(QDialog *LayerBrowserDialog)
    {
        LayerBrowserDialog->setWindowTitle(QApplication::translate("LayerBrowserDialog", "Floor Browser", 0));
        tbDescription->setPlaceholderText(QApplication::translate("LayerBrowserDialog", "Floor description...", 0));
        label->setText(QApplication::translate("LayerBrowserDialog", "Floor dimensions [w, h]:", 0));
        sbWidth->setSuffix(QApplication::translate("LayerBrowserDialog", " tiles", 0));
        bAdd->setText(QApplication::translate("LayerBrowserDialog", "Add", 0));
        bDelete->setText(QApplication::translate("LayerBrowserDialog", "Delete", 0));
        bChange->setText(QApplication::translate("LayerBrowserDialog", "Change", 0));
        sbHeight->setSuffix(QApplication::translate("LayerBrowserDialog", " tiles", 0));
        bUp->setText(QApplication::translate("LayerBrowserDialog", "Move Up", 0));
        bDown->setText(QApplication::translate("LayerBrowserDialog", "Move Down", 0));
        bSelect->setText(QApplication::translate("LayerBrowserDialog", "Select and Close", 0));
        bClose->setText(QApplication::translate("LayerBrowserDialog", "Close", 0));
    } // retranslateUi

};

namespace Ui {
    class LayerBrowserDialog: public Ui_LayerBrowserDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LAYERBROWSERDIALOG_H
