#include "ObjectSettingsDialog.h"
#include "ui_ObjectSettingsDialog.h"
#include "Map.h"
#include "EventButtonItem.h"
#include "EventEditDialog.h"
#include "Resources.h"
#include "Utilities.h"
#include <QTableWidget>
#include <QComboBox>
#include <QDebug>

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

		for (int i = 0; i < ui->tabs->count() - 2; ++i) /** #TODO(bug) not correct, last 2 tabs are different to the first 2 */
		{
			QTableWidget* table = dynamic_cast<QTableWidget*>(ui->tabs->widget(i));
			for (int row = 0; row < table->rowCount(); ++row)
				settings[qToKString(ui->tabs->tabBar()->tabText(i))][qToKString(table->item(row, 0)->text())] = qToKString(table->item(row, 1)->text());
		}

		QTableWidget* table = dynamic_cast<QTableWidget*>(ui->tabs->widget(ui->tabs->count() - 2)); // hardtype tab
		kry::Util::String& type = settings["global"]["hardtype"];
		for (auto& key : hardtypesettings[type].getKeyNames())
		{
			int rowindex = 0;
			for (int row = 0; row < table->rowCount(); ++row)
			{
				if (qToKString(table->item(row, 0)->text()) == key)
				{
					rowindex = row;
					break;
				}
			}

			if (const_cast<kry::Media::Config&>(Assets::getHardTypes())[type][key].isEmpty())
				hardtypesettings[type][key] = qToKString(table->item(rowindex, 1)->text());
			else if (const_cast<kry::Media::Config&>(Assets::getHardTypes())[type][key] == "FLOOR_ID")
			{
				auto text = dynamic_cast<QComboBox*>(table->cellWidget(rowindex, 1))->currentText();
				text = text.left(text.indexOf(':'));
				hardtypesettings[type][key] = qToKString(text);
			}
			else if (const_cast<kry::Media::Config&>(Assets::getHardTypes())[type][key] == "BOOL")
				hardtypesettings[type][key] = qToKString(dynamic_cast<QComboBox*>(table->cellWidget(rowindex, 1))->currentText());
			else if (const_cast<kry::Media::Config&>(Assets::getHardTypes())[type][key] == "OBJECT_ID")
			{
				auto text = dynamic_cast<QComboBox*>(table->cellWidget(rowindex, 1))->currentText();
				text = text.left(text.indexOf(':'));
				hardtypesettings[type][key] = qToKString(text);
			}

			/** #TODO(incomplete) add parts for whatever else that comes along */
		}
		// get hardtype stuff

		// ignore events stuff for now

		this->close();
	});
	connect(ui->bPivotSet, &QPushButton::clicked, [this](bool)
	{
		ui->gvObject->setPivoting(true);
	});
}

ObjectSettingsDialog::~ObjectSettingsDialog()
{
	delete ui;
}

DialogResult ObjectSettingsDialog::showDialog(const kry::Util::String& title, Object* object)
{
	QString t = kryToQString(title);
	t[0] = t[0].toUpper();
	this->setWindowTitle(t);
	settings = object->properties;
	hardtypesettings = object->hardproperties;

	QSize iconsize;
	auto dim = object->asset->resource->rawresource->getDimensions();
	iconsize.setWidth(dim[0]);
	iconsize.setHeight(dim[1]);
	QSize scaled = iconsize.scaled(ui->gvObject->width(), ui->gvObject->height(), Qt::KeepAspectRatio);
	qreal x = 0.0, y = 0.0;
	if (settings.sectionExists("object"))
	{
		x = settings["object"].keyExists("relativex") ? kry::Util::toDecimal<qreal>(settings["object"]["relativex"]) : 0.0;
		y = settings["object"].keyExists("relativey") ? kry::Util::toDecimal<qreal>(settings["object"]["relativey"]) : 0.0;
	}
	else if (settings.sectionExists("entity"))
	{
		x = settings["entity"].keyExists("relativex") ? kry::Util::toDecimal<qreal>(settings["entity"]["relativex"]) : 0.0;
		y = settings["entity"].keyExists("relativey") ? kry::Util::toDecimal<qreal>(settings["entity"]["relativey"]) : 0.0;
	}

	ui->gvObject->setup(QIcon(kryToQString(settings["global"]["resource"])).pixmap(scaled),
			x * static_cast<qreal>(scaled.width()), y * static_cast<qreal>(scaled.height()));
	updateTables(object);

	ui->gvObject->setPivoting(false);
	this->exec();
	ui->gvObject->reset();
	return lastresult;
}

kry::Media::Config ObjectSettingsDialog::getSettings() const
{
	return settings;
}

kry::Media::Config ObjectSettingsDialog::getHardTypeSettings() const
{
	return hardtypesettings;
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

void ObjectSettingsDialog::updateTables(Object* object)
{
	while (ui->tabs->count() > 0)
		delete ui->tabs->widget(0);

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
		if (section.isEmpty() || section == "global" || section == "events")
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

	table = initTable(new QTableWidget(ui->tabs));
	ui->tabs->addTab(table, "hardtype");
	kry::Util::String& type = object->properties["global"]["hardtype"];
	for (auto& key : hardtypesettings[type].getKeyNames())
	{
		int index = table->rowCount();
		table->insertRow(index);
		table->setItem(index, 0, new QTableWidgetItem(kryToQString(key)));
		table->item(index, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		if (const_cast<kry::Media::Config&>(Assets::getHardTypes())[type][key].isEmpty())
			table->setItem(index, 1, new QTableWidgetItem(kryToQString(hardtypesettings[type][key])));
		else if (const_cast<kry::Media::Config&>(Assets::getHardTypes())[type][key] == "FLOOR_ID")
		{
			QComboBox* box = new QComboBox(table);
			box->setEditable(false);
			unsigned i = 0;
			for (auto& floor : Map::getMap()->getLayers())
				box->addItem(QString::number(i++) + ':' + floor->description);
			if (!hardtypesettings[type][key].isEmpty())
				box->setCurrentIndex(kry::Util::toIntegral<int>(hardtypesettings[type][key]));
			table->setCellWidget(index, 1, box);
		}
		else if (const_cast<kry::Media::Config&>(Assets::getHardTypes())[type][key] == "BOOL")
		{
			QComboBox* box = new QComboBox(table);
			box->setEditable(false);
			box->addItem("true");
			box->addItem("false");
			if (!hardtypesettings[type][key].isEmpty())
				box->setCurrentIndex(hardtypesettings[type][key] == "true" ? 0 : 1);
			table->setCellWidget(index, 1, box);
		}
		else if (const_cast<kry::Media::Config&>(Assets::getHardTypes())[type][key] == "OBJECT_ID")
		{
			QComboBox* box = new QComboBox(table);
			box->setEditable(false);
			unsigned curr = 0;
			unsigned index = 0;
			for (Tile& tile : Map::getMap()->getCurrentLayer()->tiles)
			{
				for (auto& obj : tile.objects)
				{
					auto& id = obj->properties["global"]["id"];
					box->addItem(kryToQString(id + ':' + obj->properties["global"]["name"]));
					if (hardtypesettings[type][key] == id)
						curr = index;
					++index;
				}
			}
			box->setCurrentIndex(curr);
			table->setCellWidget(index, 1, box);
		}
		/** #TODO(incomplete) add parts for whatever else that comes along */
	}

	table = initTable(new QTableWidget(ui->tabs));
	ui->tabs->addTab(table, "events");
	for (Event& event : object->events)
	{
		int index = table->rowCount();
		table->insertRow(index);
		table->setItem(index, 0, new QTableWidgetItem(event.event->name));
		table->item(index, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

		EventButtonItem* button = new EventButtonItem(event.actions.empty() ? "None" : "...", table);
		button->setEvent(event);
		button->setEnabled(false); /** #TODO(note) remove this line if events are added */
		connect(button, &EventButtonItem::clicked, [button](bool)
		{
			EventEditDialog edit(button);
			edit.showDialog(button);
		});
		table->setCellWidget(index, 1, button);
	}
}
