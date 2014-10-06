/********************************************************************************
** Form generated from reading UI file 'EventManagerDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EVENTMANAGERDIALOG_H
#define UI_EVENTMANAGERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_EventManagerDialog
{
public:

    void setupUi(QDialog *EventManagerDialog)
    {
        if (EventManagerDialog->objectName().isEmpty())
            EventManagerDialog->setObjectName(QStringLiteral("EventManagerDialog"));
        EventManagerDialog->resize(640, 480);

        retranslateUi(EventManagerDialog);

        QMetaObject::connectSlotsByName(EventManagerDialog);
    } // setupUi

    void retranslateUi(QDialog *EventManagerDialog)
    {
        EventManagerDialog->setWindowTitle(QApplication::translate("EventManagerDialog", "Dialog", 0));
    } // retranslateUi

};

namespace Ui {
    class EventManagerDialog: public Ui_EventManagerDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EVENTMANAGERDIALOG_H
