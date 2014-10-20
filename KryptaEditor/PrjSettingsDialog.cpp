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
		if (!xy.isEmpty())
		{
			settings["player"]["tilex"] = xy.substring(0, xy.indexOf('x'));
			settings["player"]["tiley"] = xy.substring(xy.indexOf('x') + 1);
		}
		xy = qToKString(ui->playerProperties->item(2, 1)->text());
		if (!xy.isEmpty())
		{
			settings["player"]["dimensionsx"] = xy.substring(0, xy.indexOf('x'));
			settings["player"]["dimensionsy"] = xy.substring(xy.indexOf('x') + 1);
		}
		settings["player"]["seeInFog"] = qToKString(ui->playerProperties->item(3, 1)->text());
		settings["player"]["directions"] = qToKString(ui->playerProperties->item(4, 1)->text());
		settings["player"]["maxHeuristic"] = qToKString(ui->playerProperties->item(5, 1)->text());
		settings["player"]["viewDistance"] = qToKString(ui->playerProperties->item(6, 1)->text());
		settings["player"]["moveAcceleration"] = qToKString(ui->playerProperties->item(7, 1)->text());
		settings["player"]["turnAcceleration"] = qToKString(ui->playerProperties->item(8, 1)->text());
		settings["player"]["maxMoveSpeed"] = qToKString(ui->playerProperties->item(9, 1)->text());
		settings["player"]["maxTurnSpeed"] = qToKString(ui->playerProperties->item(10, 1)->text());
		settings["player"]["skinIdle"] = qToKString(ui->playerProperties->item(11, 1)->text());
		settings["player"]["skinRun"] = qToKString(ui->playerProperties->item(12, 1)->text());
		settings["player"]["skinDead"] = qToKString(ui->playerProperties->item(13, 1)->text());
		settings["player"]["health"] = qToKString(ui->playerProperties->item(14, 1)->text());

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
	settings["project"]["path"] = ""; // set from map probably?

	if (!settings["player"].keyExists("layer"))
		settings["player"]["layer"] = kry::Util::toString(Map::getMap()->getCurrentLayer()->index);
	if (!settings["player"].keyExists("tilex"))
		settings["player"]["tilex"] = kry::Util::toString(0);
	if (!settings["player"].keyExists("tiley"))
		settings["player"]["tiley"] = kry::Util::toString(0);
	if (!settings["player"].keyExists("dimensionsx"))
		settings["player"]["dimensionsx"] = kry::Util::toString(1);
	if (!settings["player"].keyExists("dimensionsy"))
		settings["player"]["dimensionsy"] = kry::Util::toString(1);
	if (!settings["player"].keyExists("seeInFog"))
		settings["player"]["seeInFog"] = kry::Util::toString(1);
	if (!settings["player"].keyExists("directions"))
		settings["player"]["directions"] = kry::Util::toString(8);
	if (!settings["player"].keyExists("maxHeuristic"))
		settings["player"]["maxHeuristic"] = kry::Util::toString(0);
	if (!settings["player"].keyExists("viewDistance"))
		settings["player"]["viewDistance"] = kry::Util::toString(3);
	if (!settings["player"].keyExists("moveAcceleration"))
		settings["player"]["moveAcceleration"] = kry::Util::toString(1.3);
	if (!settings["player"].keyExists("turnAcceleration"))
		settings["player"]["turnAcceleration"] = kry::Util::toString(50);
	if (!settings["player"].keyExists("maxMoveSpeed"))
		settings["player"]["maxMoveSpeed"] = kry::Util::toString(6);
	if (!settings["player"].keyExists("maxTurnSpeed"))
		settings["player"]["maxTurnSpeed"] = kry::Util::toString(700);
	if (!settings["player"].keyExists("skinIdle"))
		settings["player"]["skinIdle"] = "playerIdle";
	if (!settings["player"].keyExists("skinRun"))
		settings["player"]["skinRun"] = "playerMove";
	if (!settings["player"].keyExists("skinDead"))
		settings["player"]["skinDead"] = "playerDead";
	if (!settings["player"].keyExists("health"))
		settings["player"]["health"] = kry::Util::toString(1);
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
	/** #TODO(incomplete) add a widgets for specific options */
	ui->playerProperties->setItem(1, 1, new QTableWidgetItem(kryToQString(settings["player"]["tilex"]) + 'x' + kryToQString(settings["player"]["tiley"])));
	ui->playerProperties->setItem(2, 1, new QTableWidgetItem(kryToQString(settings["player"]["dimensionsx"]) + 'x' + kryToQString(settings["player"]["dimensionsy"])));
	ui->playerProperties->setItem(3, 1, new QTableWidgetItem(kryToQString(settings["player"]["seeInFog"])));
	ui->playerProperties->setItem(4, 1, new QTableWidgetItem(kryToQString(settings["player"]["directions"])));
	ui->playerProperties->setItem(5, 1, new QTableWidgetItem(kryToQString(settings["player"]["maxHeuristic"])));
	ui->playerProperties->setItem(6, 1, new QTableWidgetItem(kryToQString(settings["player"]["viewDistance"])));
	ui->playerProperties->setItem(7, 1, new QTableWidgetItem(kryToQString(settings["player"]["moveAcceleration"])));
	ui->playerProperties->setItem(8, 1, new QTableWidgetItem(kryToQString(settings["player"]["turnAcceleration"])));
	ui->playerProperties->setItem(9, 1, new QTableWidgetItem(kryToQString(settings["player"]["maxMoveSpeed"])));
	ui->playerProperties->setItem(10, 1, new QTableWidgetItem(kryToQString(settings["player"]["maxTurnSpeed"])));
	ui->playerProperties->setItem(11, 1, new QTableWidgetItem(kryToQString(settings["player"]["skinIdle"])));
	ui->playerProperties->setItem(12, 1, new QTableWidgetItem(kryToQString(settings["player"]["skinRun"])));
	ui->playerProperties->setItem(13, 1, new QTableWidgetItem(kryToQString(settings["player"]["skinDead"])));
	ui->playerProperties->setItem(14, 1, new QTableWidgetItem(kryToQString(settings["player"]["health"])));
}
