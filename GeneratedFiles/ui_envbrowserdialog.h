/********************************************************************************
** Form generated from reading UI file 'envbrowserdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ENVBROWSERDIALOG_H
#define UI_ENVBROWSERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_EnvBrowserDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_4;
    QListWidget *lbIcons;
    QVBoxLayout *verticalLayout_3;
    QLabel *lResName;
    QLabel *lResImage;
    QTableWidget *resProperties;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *bSelect;
    QSpacerItem *horizontalSpacer;
    QPushButton *bClose;

    void setupUi(QDialog *EnvBrowserDialog)
    {
        if (EnvBrowserDialog->objectName().isEmpty())
            EnvBrowserDialog->setObjectName(QStringLiteral("EnvBrowserDialog"));
        EnvBrowserDialog->setWindowModality(Qt::NonModal);
        EnvBrowserDialog->resize(640, 480);
        verticalLayout_2 = new QVBoxLayout(EnvBrowserDialog);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        lbIcons = new QListWidget(EnvBrowserDialog);
        lbIcons->setObjectName(QStringLiteral("lbIcons"));
        lbIcons->setMovement(QListView::Static);
        lbIcons->setViewMode(QListView::IconMode);

        verticalLayout_4->addWidget(lbIcons);

        verticalLayout_4->setStretch(0, 95);

        horizontalLayout->addLayout(verticalLayout_4);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        lResName = new QLabel(EnvBrowserDialog);
        lResName->setObjectName(QStringLiteral("lResName"));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setItalic(true);
        font.setUnderline(true);
        font.setWeight(75);
        font.setKerning(true);
        lResName->setFont(font);
        lResName->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(lResName);

        lResImage = new QLabel(EnvBrowserDialog);
        lResImage->setObjectName(QStringLiteral("lResImage"));
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lResImage->sizePolicy().hasHeightForWidth());
        lResImage->setSizePolicy(sizePolicy);
        lResImage->setFrameShape(QFrame::StyledPanel);
        lResImage->setScaledContents(true);
        lResImage->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(lResImage);

        resProperties = new QTableWidget(EnvBrowserDialog);
        if (resProperties->columnCount() < 2)
            resProperties->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        resProperties->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        resProperties->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        resProperties->setObjectName(QStringLiteral("resProperties"));
        resProperties->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        resProperties->setShowGrid(true);
        resProperties->setWordWrap(false);
        resProperties->setCornerButtonEnabled(false);
        resProperties->setRowCount(0);
        resProperties->setColumnCount(2);
        resProperties->horizontalHeader()->setVisible(false);
        resProperties->horizontalHeader()->setStretchLastSection(true);
        resProperties->verticalHeader()->setVisible(false);

        verticalLayout_3->addWidget(resProperties);

        verticalLayout_3->setStretch(0, 5);
        verticalLayout_3->setStretch(1, 45);
        verticalLayout_3->setStretch(2, 50);

        horizontalLayout->addLayout(verticalLayout_3);

        horizontalLayout->setStretch(0, 50);
        horizontalLayout->setStretch(1, 50);

        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        bSelect = new QPushButton(EnvBrowserDialog);
        bSelect->setObjectName(QStringLiteral("bSelect"));

        horizontalLayout_2->addWidget(bSelect);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        bClose = new QPushButton(EnvBrowserDialog);
        bClose->setObjectName(QStringLiteral("bClose"));

        horizontalLayout_2->addWidget(bClose);

        horizontalLayout_2->setStretch(0, 20);
        horizontalLayout_2->setStretch(1, 60);
        horizontalLayout_2->setStretch(2, 20);

        verticalLayout->addLayout(horizontalLayout_2);

        verticalLayout->setStretch(0, 95);
        verticalLayout->setStretch(1, 5);

        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(EnvBrowserDialog);

        QMetaObject::connectSlotsByName(EnvBrowserDialog);
    } // setupUi

    void retranslateUi(QDialog *EnvBrowserDialog)
    {
        EnvBrowserDialog->setWindowTitle(QApplication::translate("EnvBrowserDialog", "Environment Browser", 0));
        lResName->setText(QString());
        lResImage->setText(QString());
        QTableWidgetItem *___qtablewidgetitem = resProperties->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("EnvBrowserDialog", "Property", 0));
        QTableWidgetItem *___qtablewidgetitem1 = resProperties->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("EnvBrowserDialog", "Value", 0));
        bSelect->setText(QApplication::translate("EnvBrowserDialog", "Select and Close", 0));
        bClose->setText(QApplication::translate("EnvBrowserDialog", "Close", 0));
    } // retranslateUi

};

namespace Ui {
    class EnvBrowserDialog: public Ui_EnvBrowserDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ENVBROWSERDIALOG_H
