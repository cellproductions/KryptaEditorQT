/********************************************************************************
** Form generated from reading UI file 'AudioManagerDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AUDIOMANAGERDIALOG_H
#define UI_AUDIOMANAGERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AudioManagerDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QComboBox *cbAudio;
    QTableWidget *tAudioProps;
    QHBoxLayout *horizontalLayout;
    QPushButton *bDone;
    QPushButton *bPlay;
    QPushButton *bDelete;
    QPushButton *bAdd;

    void setupUi(QDialog *AudioManagerDialog)
    {
        if (AudioManagerDialog->objectName().isEmpty())
            AudioManagerDialog->setObjectName(QStringLiteral("AudioManagerDialog"));
        AudioManagerDialog->resize(380, 280);
        verticalLayout_2 = new QVBoxLayout(AudioManagerDialog);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        cbAudio = new QComboBox(AudioManagerDialog);
        cbAudio->setObjectName(QStringLiteral("cbAudio"));

        verticalLayout->addWidget(cbAudio);

        tAudioProps = new QTableWidget(AudioManagerDialog);
        if (tAudioProps->columnCount() < 2)
            tAudioProps->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tAudioProps->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tAudioProps->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        tAudioProps->setObjectName(QStringLiteral("tAudioProps"));
        tAudioProps->horizontalHeader()->setVisible(false);
        tAudioProps->horizontalHeader()->setStretchLastSection(true);
        tAudioProps->verticalHeader()->setVisible(false);

        verticalLayout->addWidget(tAudioProps);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        bDone = new QPushButton(AudioManagerDialog);
        bDone->setObjectName(QStringLiteral("bDone"));

        horizontalLayout->addWidget(bDone);

        bPlay = new QPushButton(AudioManagerDialog);
        bPlay->setObjectName(QStringLiteral("bPlay"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(bPlay->sizePolicy().hasHeightForWidth());
        bPlay->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(bPlay);

        bDelete = new QPushButton(AudioManagerDialog);
        bDelete->setObjectName(QStringLiteral("bDelete"));

        horizontalLayout->addWidget(bDelete);

        bAdd = new QPushButton(AudioManagerDialog);
        bAdd->setObjectName(QStringLiteral("bAdd"));

        horizontalLayout->addWidget(bAdd);


        verticalLayout->addLayout(horizontalLayout);

        verticalLayout->setStretch(0, 5);
        verticalLayout->setStretch(1, 90);
        verticalLayout->setStretch(2, 5);

        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(AudioManagerDialog);

        QMetaObject::connectSlotsByName(AudioManagerDialog);
    } // setupUi

    void retranslateUi(QDialog *AudioManagerDialog)
    {
        AudioManagerDialog->setWindowTitle(QApplication::translate("AudioManagerDialog", "Audio Manager", 0));
        QTableWidgetItem *___qtablewidgetitem = tAudioProps->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("AudioManagerDialog", "Property", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tAudioProps->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("AudioManagerDialog", "Value", 0));
        bDone->setText(QApplication::translate("AudioManagerDialog", "Done", 0));
        bPlay->setText(QApplication::translate("AudioManagerDialog", "Preview", 0));
        bDelete->setText(QApplication::translate("AudioManagerDialog", "Delete Audio", 0));
        bAdd->setText(QApplication::translate("AudioManagerDialog", "Add New Audio", 0));
    } // retranslateUi

};

namespace Ui {
    class AudioManagerDialog: public Ui_AudioManagerDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUDIOMANAGERDIALOG_H
