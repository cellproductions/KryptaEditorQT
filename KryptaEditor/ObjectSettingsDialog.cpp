#include "ObjectSettingsDialog.h"
#include "ui_ObjectSettingsDialog.h"
#include "Utilities.h"
#include <QTableWidget>

ObjectSettingsDialog::ObjectSettingsDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ObjectSettingsDialog), lastresult(DialogResult::CANCEL)
{
	ui->setupUi(this);

	connect(ui->bCancel, &QPushButton::clicked, [this]()
	{
		lastresult = DialogResult::CANCEL;
		this->close();
	});
	connect(ui->bSave, &QPushButton::clicked, [this]()
	{
		lastresult = DialogResult::OK;

		for (int i = 0; i < ui->tabs->count(); ++i)
		{
			QTableWidget* table = dynamic_cast<QTableWidget*>(ui->tabs->widget(i));
			for (int row = 0; row < table->rowCount(); ++row)
				settings[qToKString(ui->tabs->tabBar()->tabText(i))][qToKString(table->item(row, 0)->text())] = qToKString(table->item(row, 1)->text());
		}

		this->close();
	});
}

ObjectSettingsDialog::~ObjectSettingsDialog()
{
	delete ui;
}

DialogResult ObjectSettingsDialog::showDialog(const kry::Util::String& title, const kry::Media::Config& config)
{
	QString t = kryToQString(title);
	t[0] = t[0].toUpper();
	this->setWindowTitle(t);
	settings = config;
	ui->lObject->setPixmap(QIcon(kryToQString(settings["global"]["resource"])).pixmap(ui->lObject->size()));
	updateTables();

	this->exec();
	return lastresult;
}

kry::Media::Config ObjectSettingsDialog::getSettings() const
{
	return settings;
}

QTableWidget* initTable(QTableWidget* table)
{
	table->setVerticalHeader(new QHeaderView(Qt::Vertical, table));
	table->verticalHeader()->setVisible(false);
	table->setHorizontalHeader(new QHeaderView(Qt::Horizontal, table));
	table->horizontalHeader()->setVisible(false);
	table->horizontalHeader()->setStretchLastSection(true);
	table->insertColumn(0);
	table->insertColumn(0);

	return table;
}

void ObjectSettingsDialog::updateTables()
{
	while (ui->tabs->count() > 0)
		delete ui->tabs->widget(ui->tabs->count() - 1);

	QTableWidget* table = initTable(new QTableWidget(ui->tabs));
	ui->tabs->addTab(table, "global");

	for (kry::Util::String& key : settings["global"].getKeyNames())
	{
		int index = table->rowCount();
		table->insertRow(index);
		table->setItem(index, 0, new QTableWidgetItem(kryToQString(key)));
		table->item(index, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		table->setItem(index, 1, new QTableWidgetItem(kryToQString(settings["global"][key])));
	}

	for (kry::Util::String& section : settings.getSectionNames())
	{
		if (section.isEmpty() || section == "global")
			continue;

		QTableWidget* table = initTable(new QTableWidget(ui->tabs));
		ui->tabs->addTab(table, kryToQString(section));

		for (kry::Util::String& key : settings[section].getKeyNames())
		{
			int index = table->rowCount();
			table->insertRow(index);
			table->setItem(index, 0, new QTableWidgetItem(kryToQString(key)));
			table->item(index, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
			table->setItem(index, 1, new QTableWidgetItem(kryToQString(settings[section][key])));
		}
	}
}
