#include "ui_prjsetupdialog.h"
#include "PrjSetupDialog.h"
#include "LayerOptionsItem.h"
#include <QShortcut>
#include <QMessageBox>

PrjSetupDialog::PrjSetupDialog(QWidget *parent) : QDialog(parent), ui(new Ui::PrjWizard) /** #TODO(incomplete) layer names should be unique */
{
    ui->setupUi(this);

    connect(ui->tbPrjName, SIGNAL(textEdited(QString)), ui->tbMapName, SLOT(setText(QString)));

    connect(new QShortcut(QKeySequence(Qt::Key_Delete), ui->lbLayers), &QShortcut::activated, [this]()
    {
        ui->bDelete->click();
    });

    connect(new QShortcut(QKeySequence(Qt::Key_Left), ui->lbLayers), &QShortcut::activated, [this]()
    {
        ui->bUp->click();
    });

    connect(new QShortcut(QKeySequence(Qt::Key_Right), ui->lbLayers), &QShortcut::activated, [this]()
    {
        ui->bDown->click();
    });

    connect(ui->bAdd, &QPushButton::clicked, [this]()
    {
		LayerOptionsItem* option = new LayerOptionsItem;
        option->setDescription(ui->tbDescr->text());
        option->setWidth(ui->sbWidth->value());
        option->setHeight(ui->sbHeight->value());
        option->setText(option->getDescription() + ": " + QString::number(option->getWidth()) + 'x' + QString::number(option->getHeight()) + " tiles");
        ui->lbLayers->addItem(option);

        ui->tbDescr->clear();
        ui->tbDescr->setFocus();
    });

    connect(ui->bDelete, &QPushButton::clicked, [this]()
    {
        delete ui->lbLayers->currentItem();
    });

    connect(ui->bUp, &QPushButton::clicked, [this]()
    {
        auto row = ui->lbLayers->currentRow();
        auto item = ui->lbLayers->takeItem(row);
        if (row == 0)
            ui->lbLayers->addItem(item);
        else
            ui->lbLayers->insertItem(row - 1, item);
        ui->lbLayers->setCurrentItem(item);
    });

    connect(ui->bDown, &QPushButton::clicked, [this]()
    {
        auto row = ui->lbLayers->currentRow();
        auto item = ui->lbLayers->takeItem(row);
        if (row == ui->lbLayers->count())
            ui->lbLayers->insertItem(0, item);
        else
            ui->lbLayers->insertItem(row + 1, item);
        ui->lbLayers->setCurrentItem(item);
    });

    connect(ui->tbDescr, &QLineEdit::returnPressed, [this]()
    {
        ui->bAdd->click();
    });

    connect(ui->bDone, &QPushButton::clicked, [this]()
    {
        if (ui->tbPrjName->text().trimmed().isEmpty() || ui->lbLayers->count() <= 0)
        {
            QMessageBox::warning(this, "Empty Fields", "The project name must not be empty and there must be at least one layer!", QMessageBox::Ok);
            return;
        }

        this->lastresult = DialogResult::OK;
        this->close();
    });

    connect(ui->bCancel, &QPushButton::clicked, [this]()
    {
        this->lastresult = DialogResult::CANCEL;
        this->close();
    });
}

PrjSetupDialog::~PrjSetupDialog()
{
    delete ui;
}

DialogResult PrjSetupDialog::showDialog()
{
    this->exec();
    return lastresult;
}
