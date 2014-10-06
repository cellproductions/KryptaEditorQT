#include "PrjSettingsDialog.h"
#include "ui_PrjSettingsDialog.h"
#include "Map.h"
#include "LayerOptionsItem.h"
#include "Utilities.h"
#include <Utilities/StringConvert.h>
#include <QComboBox>

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

		settings["player"]["layer"] = kry::Util::toString(dynamic_cast<QComboBox*>(ui->playerProperties->cellWidget(0, 1))->currentIndex());
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

	if (!settings["player"].keyExists("layer"))
		settings["player"]["layer"] = kry::Util::toString(Map::getMap()->getCurrentLayer()->index);
	if (!settings["player"].keyExists("tilex"))
		settings["player"]["tilex"] = kry::Util::toString(0);
	if (!settings["player"].keyExists("tiley"))
		settings["player"]["tiley"] = kry::Util::toString(0);
	if (!settings["player"].keyExists("speed"))
		settings["player"]["speed"] = kry::Util::toString(1);
	if (!settings["player"].keyExists("fov"))
		settings["player"]["fov"] = kry::Util::toString(8);
}

void PrjSettingsDialog::setTableData()
{
	ui->projectProperties->setItem(0, 1, new QTableWidgetItem(kryToQString(settings["project"]["name"])));
	ui->projectProperties->item(0, 1)->setFlags(Qt::ItemIsSelectable);
	ui->projectProperties->setItem(1, 1, new QTableWidgetItem(kryToQString(settings["project"]["path"])));
	ui->projectProperties->item(1, 1)->setFlags(Qt::ItemIsSelectable);

	QComboBox* box = new QComboBox(ui->playerProperties);
	box->setEditable(false);
	for (auto& layer : Map::getMap()->getLayers())
		box->addItem(QString::number(layer->index) + ':' + layer->description);
	box->setCurrentIndex(kry::Util::toIntegral<int>(settings["player"]["layer"]));
	ui->playerProperties->setCellWidget(0, 1, box);
	ui->playerProperties->setItem(1, 1, new QTableWidgetItem(kryToQString(settings["player"]["tilex"]) + 'x' + kryToQString(settings["player"]["tiley"])));
	ui->playerProperties->setItem(2, 1, new QTableWidgetItem(kryToQString(settings["player"]["speed"])));
	ui->playerProperties->setItem(3, 1, new QTableWidgetItem(kryToQString(settings["player"]["fov"])));
}
