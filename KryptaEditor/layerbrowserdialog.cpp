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
	connect(ui->lbLayers, &QListWidget::currentRowChanged, [this](int row)
	{
		if (row < 0)
			return;
		
		LayerOptionsItem* option = dynamic_cast<LayerOptionsItem*>(ui->lbLayers->item(row));
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
		ui->bDelete->setEnabled(true);
	});
	connect(ui->bDelete, &QPushButton::clicked, [this](bool)
	{
		if (QMessageBox::warning(this, "Delete Layer", "Are you sure you would like to delete this floor?", QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
		{
			auto item = dynamic_cast<LayerOptionsItem*>(ui->lbLayers->takeItem(ui->lbLayers->currentRow()));
			todelete.push_back(item->getLayer());
			delete item;
			if (ui->lbLayers->count() > -1)
				ui->lbLayers->setCurrentRow(0);
		}
		if (ui->lbLayers->count() <= 0)
			ui->bDelete->setEnabled(false);
	});
	connect(ui->bChange, &QPushButton::clicked, [this](bool)
	{
		if (ui->lbLayers->currentRow() < 0)
			return;
		LayerOptionsItem* option = dynamic_cast<LayerOptionsItem*>(ui->lbLayers->currentItem());
		option->setDescription(ui->tbDescription->text());
		option->setWidth(ui->sbWidth->value());
		option->setHeight(ui->sbHeight->value());
		option->setText(ui->tbDescription->text() + ": " + QString::number(ui->sbWidth->value()) + 'x' + QString::number(ui->sbHeight->value()) + " tiles");
	});
	connect(ui->bUp, &QPushButton::clicked, [this](bool)
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

		for (auto itemlayer : todelete) /** #TODO(bug) crash here when adding new layer */
		{
			auto& layers = Map::getMap()->getLayers();
			for (auto itr = layers.begin(); itr != layers.end(); ++itr)
			{
				if (itr->get() == itemlayer)
				{
					Map::getMap()->getLayers().erase(itr);
					break;
				}
			}
		}
		todelete.clear();

		auto oldlayers = Map::getMap()->getLayers();
		Map::getMap()->getLayers().clear();
		for (int i = 0; i < ui->lbLayers->count(); ++i)
		{
			auto item = dynamic_cast<LayerOptionsItem*>(ui->lbLayers->item(i));
			if (i < oldlayers.size() && item->getLayer() == oldlayers[i].get())
			{
				auto layer = oldlayers[i];
				Map::getMap()->getLayers().push_back(layer);
				kry::Util::Vector2i size(item->getWidth(), item->getHeight());
				if (layer->size != size)
				{
					if (layer->size[0] < size[0]) /** #TODO(incomplete) finish these */
					{
						// add new tiles to the x axis
					}
					else if (layer->size[0] > size[0])
					{
						// remove tiles from the x axis
					}
					if (layer->size[1] < size[1])
					{
						// add new tiles to the y axis
					}
					else if (layer->size[1] > size[1])
					{
						// remove tiles from the y axis
					}
				}
				unsigned index = 0;
				for (auto& tile : layer->tiles)
					tile.properties["global"]["id"] = kry::Util::toString(static_cast<unsigned>(i) * 10000u + (index++));
				layer->index = i;
			}
			else
			{
				Tile tile;
				tile.asset = Assets::getTiles()[0].get(); /** #TODO(bug) there might not be any assets */
				Map::Layer* layer = Map::createLayer(tile, item, i);
				Map::getMap()->getLayers().emplace_back(layer);
			}
		}

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
		for (auto layer : Map::getMap()->getLayers())
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
		else
			ui->bDelete->setEnabled(false);
	}
	else
		return DialogResult::CANCEL;

	this->exec();
	return lastresult;
}

size_t LayerBrowserDialog::getSelectedIndex()
{
	return ui->lbLayers->currentRow();
}
