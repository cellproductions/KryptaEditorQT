#include "ObjectSettingsDialog.h"
#include "ui_ObjectSettingsDialog.h"
#include "Map.h"
#include "EventButtonItem.h"
#include "EventEditDialog.h"
#include "Resources.h"
#include "Utilities.h"
#include <QTableWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
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

		for (int i = 0; i < ui->tabs->count() - 2; ++i)
		{
			QTableWidget* table = dynamic_cast<QTableWidget*>(ui->tabs->widget(i));
			for (int row = 0; row < table->rowCount(); ++row)
				settings[qToKString(ui->tabs->tabBar()->tabText(i))][qToKString(table->item(row, 0)->text())] = qToKString(table->item(row, 1)->text());
		}

		QTableWidget* table = dynamic_cast<QTableWidget*>(ui->tabs->widget(ui->tabs->count() - 2)); // hardtype tab
		kry::Util::String type = settings["global"]["hardtype"];
		
		auto saveHard = [this, table](const kry::Util::String& section)
		{
			for (auto& key : hardtypesettings[section].getKeyNames())
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

				auto type = const_cast<kry::Media::Config&>(Assets::getHardTypes())[section][key];
				if (type.isEmpty() || type == "COUNT" || type == "VEC_2") /** #TODO(change) add these 2 types as widgets instead */
					hardtypesettings[section][key] = qToKString(table->item(rowindex, 1)->text());
				else if (type == "FLOOR_ID")
				{
					auto text = dynamic_cast<QComboBox*>(table->cellWidget(rowindex, 1))->currentText();
					text = text.left(text.indexOf(':'));
					hardtypesettings[section][key] = qToKString(text);
				}
				else if (type == "BOOL")
					hardtypesettings[section][key] = qToKString(dynamic_cast<QCheckBox*>(table->cellWidget(rowindex, 1))->isChecked() ? "true" : "false");
				else if (type == "OBJECT_ID")
				{
					auto text = dynamic_cast<QComboBox*>(table->cellWidget(rowindex, 1))->currentText();
					text = text.left(text.indexOf(':'));
					if (text != "-1")
						hardtypesettings[section][key] = qToKString(text);
				}
				else if (type == "ANIM_ID")
				{
					auto text = dynamic_cast<QComboBox*>(table->cellWidget(rowindex, 1))->currentText();
					text = text.left(text.indexOf(':'));
					if (text != "-1")
						hardtypesettings[section][key] = qToKString(text);
				}
				else if (type == "ANIM_DIR")
				{
					auto text = dynamic_cast<QComboBox*>(table->cellWidget(rowindex, 1))->currentText();
					hardtypesettings[section][key] = qToKString(text).substring(0, 1);
				}
				else if (type == "DIR")
				{
					auto value = dynamic_cast<QSpinBox*>(table->cellWidget(rowindex, 1))->value();
					hardtypesettings[section][key] = kry::Util::toString(value);
				}

				/** #TODO(incomplete) add parts for whatever else that comes along */
			}
		};
		saveHard("all");
		saveHard(type);

		// ignore events stuff for now

		this->close();
	});
	connect(ui->bApply, &QPushButton::clicked, [this]()
	{
		ui->bSave->click();
		for (auto& layer : Map::getMap()->getLayers())
		{
			for (auto& tile : layer->tiles)
			{
				if (tile.hardproperties["all"]["skinIdle"] == hardtypesettings["all"]["skinIdle"])
					tile.hardproperties = hardtypesettings;
				else
				{
					for (auto& object : tile.objects)
						if (object->hardproperties["all"]["skinIdle"] == hardtypesettings["all"]["skinIdle"])
							object->hardproperties = hardtypesettings;
				}
			}
		}
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
	ui->lObject->setPixmap(QIcon(kryToQString(settings["global"]["resource"])).pixmap(ui->lObject->size()));
	updateTables(object);

	this->exec();

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
	auto setupHard = [this, table](const kry::Util::String& section)
	{
		for (auto& key : hardtypesettings[section].getKeyNames())
		{
			int index = table->rowCount();
			table->insertRow(index);
			table->setItem(index, 0, new QTableWidgetItem(kryToQString(key)));
			table->item(index, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
			auto type = const_cast<kry::Media::Config&>(Assets::getHardTypes())[section][key];
			if (type.isEmpty() || type == "COUNT" || type == "VEC_2")
				table->setItem(index, 1, new QTableWidgetItem(kryToQString(hardtypesettings[section][key])));
			else if (type == "FLOOR_ID")
			{
				QComboBox* box = new QComboBox(table);
				box->setEditable(false);
				unsigned i = 0;
				for (auto& floor : Map::getMap()->getLayers())
					box->addItem(QString::number(i++) + ':' + floor->description);
				if (!hardtypesettings[section][key].isEmpty())
					box->setCurrentIndex(kry::Util::toIntegral<int>(hardtypesettings[section][key]));
				table->setCellWidget(index, 1, box);
			}
			else if (type == "BOOL")
			{
				QCheckBox* box = new QCheckBox(table);
				if (!hardtypesettings[section][key].isEmpty())
					box->setChecked(hardtypesettings[section][key] == "true");
				table->setCellWidget(index, 1, box);
			}
			else if (type == "OBJECT_ID") /** #TODO(note) this is explicitly objects only (no tiles or player) */
			{
				QComboBox* box = new QComboBox(table);
				box->setEditable(false);
				unsigned curr = 1;
				unsigned index = 0;
				box->addItem("-1:none");
				for (Tile& tile : Map::getMap()->getCurrentLayer()->tiles)
				{
					for (auto& obj : tile.objects)
					{
						auto& id = obj->properties["global"]["id"];
						box->addItem(kryToQString(id + ':' + obj->properties["global"]["name"]));
						if (hardtypesettings[section][key] == id)
							curr = index;
						++index;
					}
				}
				box->setCurrentIndex(curr);
				table->setCellWidget(index, 1, box);
			}
			else if (type == "ANIM_ID")
			{
				QComboBox* box = new QComboBox(table);
				box->setEditable(false);
				unsigned i = 0;
				box->addItem("-1:none");
				for (auto& anim : Resources::getAnimations())
					box->addItem(QString::number(i++) + ':' + kryToQString(anim->properties[0]["Skins"]["name"]));
				if (!hardtypesettings[section][key].isEmpty())
					box->setCurrentIndex(kry::Util::toIntegral<int>(hardtypesettings[section][key]) + 1);
				table->setCellWidget(index, 1, box);
			}
			else if (type == "ANIM_DIR")
			{
				QComboBox* box = new QComboBox(table);
				box->setEditable(false);
				box->addItem("1 direction");
				box->addItem("4 directions");
				box->addItem("8 directions");
				if (!hardtypesettings[section][key].isEmpty())
				{
					int dir = kry::Util::toIntegral<int>(hardtypesettings[section][key]);
					box->setCurrentIndex(dir == 1 ? 0 : (dir == 4 ? 1 : 2));
				}
				table->setCellWidget(index, 1, box);
			}
			else if (type == "DIR")
			{
				QSpinBox* box = new QSpinBox(table);
				box->setMinimum(0);
				box->setMaximum(359);
				box->setSuffix(QString::fromWCharArray(L"°"));
				if (!hardtypesettings[section][key].isEmpty())
					box->setValue(kry::Util::toIntegral<int>(hardtypesettings[section][key]));
				table->setCellWidget(index, 1, box);
			}
			/** #TODO(incomplete) add parts for whatever else that comes along */
		}
	};
	setupHard(object->asset->type == AssetType::STATIC_TILE ? kry::Util::String("floor") : kry::Util::String("all"));
	setupHard(type);

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
