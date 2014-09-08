#include "ConfigDialog.h"
#include "ui_ConfigDialog.h"
#include "Utilities.h"
#include <QDebug>

ConfigDialog::ConfigDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ConfigDialog)
{
	ui->setupUi(this);

	connect(ui->bCancel, &QPushButton::clicked, [this]()
	{
		this->lastresult = DialogResult::CANCEL;
		close();
	});
	connect(ui->bSave, &QPushButton::clicked, [this]()
	{
		for (int x = 0; x < ui->settings->rowCount(); ++x)
			settings["editor"][qToKString(ui->settings->item(x, 0)->text())] = qToKString(ui->settings->item(x, 1)->text());

		this->lastresult = DialogResult::OK;
		close();
	});
}

ConfigDialog::~ConfigDialog()
{
	delete ui;
}

DialogResult ConfigDialog::showDialog()
{
	updateTable();

	this->exec();
	return lastresult;
}

void ConfigDialog::setConfigData(const kry::Media::Config& data)
{
	settings = data;
}

kry::Media::Config ConfigDialog::getConfig() const
{
	return settings;
}

void ConfigDialog::updateTable()
{
	while (ui->settings->rowCount() > 0)
		ui->settings->removeRow(ui->settings->rowCount() - 1);

	for (auto& key : settings["editor"].getKeyNames())
	{
		int count = ui->settings->rowCount();
		ui->settings->insertRow(count);
		ui->settings->setItem(count, 0, new QTableWidgetItem(kryToQString(key)));
		ui->settings->item(count, 0)->setFlags(Qt::ItemIsEnabled);
		ui->settings->setItem(count, 1, new QTableWidgetItem(kryToQString(settings["editor"][key])));
	}
}
