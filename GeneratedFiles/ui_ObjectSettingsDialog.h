/********************************************************************************
** Form generated from reading UI file 'ObjectSettingsDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OBJECTSETTINGSDIALOG_H
#define UI_OBJECTSETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ObjectSettingsDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *lObject;
    QTabWidget *tabs;
    QHBoxLayout *horizontalLayout;
    QPushButton *bSave;
    QPushButton *bApply;
    QPushButton *bCancel;

    void setupUi(QDialog *ObjectSettingsDialog)
    {
        if (ObjectSettingsDialog->objectName().isEmpty())
            ObjectSettingsDialog->setObjectName(QStringLiteral("ObjectSettingsDialog"));
        ObjectSettingsDialog->resize(380, 480);
        verticalLayout_2 = new QVBoxLayout(ObjectSettingsDialog);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        lObject = new QLabel(ObjectSettingsDialog);
        lObject->setObjectName(QStringLiteral("lObject"));
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lObject->sizePolicy().hasHeightForWidth());
        lObject->setSizePolicy(sizePolicy);
        lObject->setFrameShape(QFrame::StyledPanel);
        lObject->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lObject);

        tabs = new QTabWidget(ObjectSettingsDialog);
        tabs->setObjectName(QStringLiteral("tabs"));

        verticalLayout->addWidget(tabs);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        bSave = new QPushButton(ObjectSettingsDialog);
        bSave->setObjectName(QStringLiteral("bSave"));

        horizontalLayout->addWidget(bSave);

        bApply = new QPushButton(ObjectSettingsDialog);
        bApply->setObjectName(QStringLiteral("bApply"));

        horizontalLayout->addWidget(bApply);

        bCancel = new QPushButton(ObjectSettingsDialog);
        bCancel->setObjectName(QStringLiteral("bCancel"));

        horizontalLayout->addWidget(bCancel);


        verticalLayout->addLayout(horizontalLayout);

        verticalLayout->setStretch(0, 45);
        verticalLayout->setStretch(1, 50);
        verticalLayout->setStretch(2, 5);

        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(ObjectSettingsDialog);

        tabs->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(ObjectSettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *ObjectSettingsDialog)
    {
        ObjectSettingsDialog->setWindowTitle(QApplication::translate("ObjectSettingsDialog", "Object Settings", 0));
        lObject->setText(QString());
        bSave->setText(QApplication::translate("ObjectSettingsDialog", "Save and Close", 0));
#ifndef QT_NO_TOOLTIP
        bApply->setToolTip(QApplication::translate("ObjectSettingsDialog", "Saves the settings of this object and all objects using the same animation.", 0));
#endif // QT_NO_TOOLTIP
        bApply->setText(QApplication::translate("ObjectSettingsDialog", "Save Across All", 0));
        bCancel->setText(QApplication::translate("ObjectSettingsDialog", "Cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class ObjectSettingsDialog: public Ui_ObjectSettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OBJECTSETTINGSDIALOG_H
