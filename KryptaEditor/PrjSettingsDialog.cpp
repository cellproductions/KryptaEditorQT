#include "PrjSettingsDialog.h"
#include "ui_PrjSettingsDialog.h"
#include "Map.h"
#include "LayerOptionsItem.h"
#include "Utilities.h"
#include <Utilities/StringConvert.h>

struct LayerTableItem : QTableWidgetItem
{
	size_t index;
	Map::Layer* layer;
};

PrjSettingsDialog::PrjSettingsDialog(QWidget *parent) : CSDialog(parent), ui(new Ui::PrjSettingsDialog)
{
	ui->setupUi(this);

	for (int row = 0; row < ui->projectProperties->rowCount(); ++row)
		ui->projectProperties->item(row, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	for (int row = 0; row < ui->playerProperties->rowCount(); ++row)
		ui->playerProperties->item(row, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

	connect(ui->bCancel, &QPushButton::clicked, [this](bool)
	{
		setTableData();

		lastresult = DialogResult::CANCEL;
		this->close();
	});

	connect(ui->bSave, &QPushButton::clicked, [this](bool)
	{
		settings["project"]["name"] = qToKString(ui->projectProperties->item(0, 1)->text());
		settings["project"]["path"] = qToKString(ui->projectProperties->item(1, 1)->text());

		settings["player"]["layer"] = kry::Util::toString(dynamic_cast<LayerTableItem*>(ui->playerProperties->item(0, 1))->index);
		auto xy = qToKString(ui->playerProperties->item(1, 1)->text());
		settings["player"]["tilex"] = xy.substring(0, xy.indexOf('x'));
		settings["player"]["tiley"] = xy.substring(xy.indexOf('x') + 1);
		settings["player"]["speed"] = qToKString(ui->playerProperties->item(2, 1)->text());
		settings["player"]["fov"] = qToKString(ui->playerProperties->item(3, 1)->text());

		lastresult = DialogResult::OK;
		this->close();
	});
}

PrjSettingsDialog::~PrjSettingsDialog()
{
	delete ui;
}

DialogResult PrjSettingsDialog::showDialog()
{
	setTableData();

	return CSDialog::showDialog();
}

void PrjSettingsDialog::resetSettings()
{
	settings["project"]["name"] = qToKString(Map::getProjectName());
	settings["project"]["path"]; // set from map probably?

	unsigned index = 0;
	for ( ; index < Map::getMap()->getLayers().size(); ++index)
		if (Map::getMap()->getLayers()[index] == Map::getMap()->getCurrentLayer())
			break;

	settings["player"]["layer"] = kry::Util::toString(index);
	settings["player"]["tilex"] = kry::Util::toString(0);
	settings["player"]["tiley"] = kry::Util::toString(0);
	settings["player"]["speed"] = kry::Util::toString(1);
	settings["player"]["fov"] = kry::Util::toString(8);
}

void PrjSettingsDialog::setTableData()
{
	ui->projectProperties->setItem(0, 1, new QTableWidgetItem(kryToQString(settings["project"]["name"])));
	ui->projectProperties->item(0, 1)->setFlags(Qt::ItemIsSelectable);
	ui->projectProperties->setItem(1, 1, new QTableWidgetItem(kryToQString(settings["project"]["path"])));
	ui->projectProperties->item(1, 1)->setFlags(Qt::ItemIsSelectable);

	LayerTableItem* item = new LayerTableItem;
	item->index = kry::Util::toUIntegral<size_t>(settings["player"]["layer"]);
	item->layer = Map::getMap()->getLayers()[item->index].get();
	item->setText(item->layer->description);
	ui->playerProperties->setItem(0, 1, item);
	ui->playerProperties->setItem(1, 1, new QTableWidgetItem(kryToQString(settings["player"]["tilex"]) + 'x' + kryToQString(settings["player"]["tiley"])));
	ui->playerProperties->setItem(2, 1, new QTableWidgetItem(kryToQString(settings["player"]["speed"])));
	ui->playerProperties->setItem(3, 1, new QTableWidgetItem(kryToQString(settings["player"]["fov"])));
}
