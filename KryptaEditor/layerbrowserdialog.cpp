#include "layerbrowserdialog.h"
#include "ui_layerbrowserdialog.h"
#include "layeroptionsitem.h"
#include <QMessageBox>
#include <QShortcut>

LayerBrowserDialog::LayerBrowserDialog(QWidget *parent) : QDialog(parent), ui(new Ui::LayerBrowserDialog)
{
    ui->setupUi(this);

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
	connect(ui->lbLayers, &QListWidget::currentRowChanged, [this](int index)
	{
		LayerOptionsItem* option = dynamic_cast<LayerOptionsItem*>(ui->lbLayers->item(index));
		ui->tbDescription->setText(option->getDescription());
		ui->sbWidth->setValue(option->getWidth());
		ui->sbHeight->setValue(option->getHeight());
	});
	connect(ui->bAdd, &QPushButton::clicked, [this]()
	{
		LayerOptionsItem* option = new LayerOptionsItem;
		option->setDescription(ui->tbDescription->text());
		option->setWidth(ui->sbWidth->value());
		option->setHeight(ui->sbHeight->value());
		option->setText(option->getDescription() + ": " + QString::number(option->getWidth()) + 'x' + QString::number(option->getHeight()) + " tiles");
		ui->lbLayers->addItem(option);

		ui->tbDescription->clear();
		ui->tbDescription->setFocus();
	});
	connect(ui->bDelete, &QPushButton::clicked, [this](bool)
	{
		if (QMessageBox::warning(this, "Delete Layer", "Are you sure you would like to delete ''?", QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
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
	connect(ui->tbDescription, &QLineEdit::returnPressed, [this]()
	{
		ui->bAdd->click();
	});
	connect(ui->bClose, &QPushButton::clicked, [this](bool)
	{
		lastresult = DialogResult::CANCEL;
		this->close();
	});
	connect(ui->bSelect, &QPushButton::clicked, [this](bool)
	{
		if (ui->lbLayers->count() <= 0)
		{
			QMessageBox::warning(this, "No Layers", "There must be at least one layer!", QMessageBox::Ok);
			return;
		}

		// remove layers that are no longer in the list
		// add layers that are new to the list
		// resize layers who's dimensions have been changed

		lastresult = DialogResult::OK;
		this->close();
	});
}

LayerBrowserDialog::~LayerBrowserDialog()
{
    delete ui;
}

DialogResult LayerBrowserDialog::showDialog()
{
	ui->lbLayers->clear();
	for (auto& layer : Map::getMap()->getLayers())
		ui->lbLayers->addItem(layer->description);

	this->exec();
	return lastresult;
}
