#include "LayerBrowserDialog.h"
#include "ui_layerbrowserdialog.h"
#include "LayerOptionsItem.h"
#include <QMessageBox>
#include <QShortcut>
#include <QItemSelectionModel>
#include <QDebug>

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
	connect(ui->lbLayers->selectionModel(), &QItemSelectionModel::selectionChanged, [this](const QItemSelection&, const QItemSelection&)
	{
		if (ui->lbLayers->count() <= 0)
			return;

		LayerOptionsItem* option = dynamic_cast<LayerOptionsItem*>(ui->lbLayers->selectedItems()[0]);
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
			todelete.push_back(dynamic_cast<LayerOptionsItem*>(ui->lbLayers->takeItem(ui->lbLayers->currentRow())));
	});
	connect(ui->bChange, &QPushButton::clicked, [this](bool)
	{
		LayerOptionsItem* option = dynamic_cast<LayerOptionsItem*>(ui->lbLayers->selectedItems()[0]);
		option->setDescription(ui->tbDescription->text());
		option->setWidth(ui->sbWidth->value());
		option->setHeight(ui->sbHeight->value());
		option->setText(ui->tbDescription->text() + ": " + QString::number(ui->sbWidth->value()) + 'x' + QString::number(ui->sbHeight->value()) + " tiles");
	});
	connect(ui->bUp, &QPushButton::clicked, [this]()
	{
		if (ui->lbLayers->count() <= 1)
			return;

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
		if (ui->lbLayers->count() <= 1)
			return;

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
		todelete.clear();
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

		/** #TODO(change) maybe move this to the MainWindow connect? */
		// delete layers in todelete, remove them from the current map
		// add layers that are new to the list
		// resize layers who's dimensions have been changed
		for (int i = 0; i < ui->lbLayers->count(); ++i) /** #TODO(bug) crash here when adding new layer. also, change the name "Layer" to "Floor" */
		{
			LayerOptionsItem* option = dynamic_cast<LayerOptionsItem*>(ui->lbLayers->item(i));
			option->getLayer()->description = option->getDescription();
		}

		todelete.clear();
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
	if (Map::getMap())
	{
		for (auto& layer : Map::getMap()->getLayers())
		{
			LayerOptionsItem* option = new LayerOptionsItem;
			option->setDescription(layer->description);
			option->setWidth(layer->size[0]);
			option->setHeight(layer->size[1]);
			option->setLayer(layer.get());
			option->setText(option->getDescription() + ": " + QString::number(option->getWidth()) + 'x' + QString::number(option->getHeight()) + " tiles");
			ui->lbLayers->addItem(option);
		}

		if (ui->lbLayers->count() > 0)
			ui->lbLayers->item(0)->setSelected(true);
	}

	this->exec();
	return lastresult;
}
