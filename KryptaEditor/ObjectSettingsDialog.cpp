#include "ObjectSettingsDialog.h"
#include "ui_ObjectSettingsDialog.h"
#include "Map.h"
#include "EventButtonItem.h"
#include "EventEditDialog.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "glpanel.h"
#include "Resources.h"
#include "Tool.h"
#include "Utilities.h"
#include <QTableWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QLineEdit>
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
		
		QTableWidget* table = dynamic_cast<QTableWidget*>(ui->tabs->widget(0)); // hardtype tab
		kry::Util::String type = settings["global"]["hardtype"];
		kry::Util::String parent = Assets::getParentType(type);
		
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
				if (type.isEmpty() || type == "VEC_2" || type == "VEC_2_ARR" || type == "ITEM_ID" || type == "ENTITY_ARR" || type == "ITEM_TYPE" || type == "ENTITY_GROUP_ARR" || type == "ITEM_ARR") /** #TODO(change) add these 2 types as widgets instead */
					hardtypesettings[section][key] = qToKString(table->item(rowindex, 1)->text());
				else if (type == "FLOOR_ID")
				{
					auto text = dynamic_cast<QComboBox*>(table->cellWidget(rowindex, 1))->currentText();
					text = text.left(text.indexOf(':'));
					hardtypesettings[section][key] = qToKString(text);
				}
				else if (type == "INT" || type == "FLOAT")
				{
					auto text = dynamic_cast<QLineEdit*>(table->cellWidget(rowindex, 1))->text();
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
					if (text != "-1") /** #TODO(note) during exporting, value will never be -1 */
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
				else if (type == "SOUND_ID")
				{
					auto text = dynamic_cast<QComboBox*>(table->cellWidget(rowindex, 1))->currentText();
					text = text.left(text.indexOf(':'));
					if (text != "-1")
						hardtypesettings[section][key] = qToKString(text);
				}
				else if (type == "MUSIC_ID")
				{
					auto text = dynamic_cast<QComboBox*>(table->cellWidget(rowindex, 1))->currentText();
					text = text.left(text.indexOf(':'));
					if (text != "-1")
						hardtypesettings[section][key] = qToKString(text);
				}
				else if (type == "ITEM_ID")
				{
					auto text = dynamic_cast<QComboBox*>(table->cellWidget(rowindex, 1))->currentText();
					text = text.left(text.indexOf(':'));
					hardtypesettings[section][key] = qToKString(text);
				}

				/** #TODO(incomplete) add parts for whatever else that comes along */
			}
		};
		saveHard(parent);
		saveHard(type);

		// ignore events stuff for now
		
		if (hardtypesettings[type].keyExists("path"))
		{
			table = dynamic_cast<QTableWidget*>(ui->tabs->widget(1)); // waypoints tab
			hardtypesettings[type]["path"] = qToKString(table->item(1, 1)->text());
			hardtypesettings[type]["loopPath"] = qToKString(table->item(2, 1)->text());
		}

		this->close();
	});
	connect(ui->bApply, &QPushButton::clicked, [this]() /** #TODO(incomplete) broken, needs a fix */
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
	auto parent = Assets::getParentType(settings["global"]["hardtype"]);
	kry::Util::String value = parent == "all" ? hardtypesettings[parent]["skinIdle"] : hardtypesettings[parent]["skin"];
	assert(!value.isEmpty());
	auto anim = value == "-1" ? Resources::getEditorTexture(EditorResource::MISSING_TILE) : Resources::getAnimations()[kry::Util::toUIntegral<size_t>(value)];
	ui->lObject->setPixmap(QIcon(anim->path).pixmap(ui->lObject->size()));

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
	ui->tabs->addTab(table, "Settings");
	kry::Util::String type = object->properties["global"]["hardtype"];
	kry::Util::String parent = Assets::getParentType(type);
	auto setupHard = [this, table](const kry::Util::String& section)
	{
		for (auto& key : hardtypesettings[section].getKeyNames()) /** #TODO(incomplete) some of these should actually do things (change animation, move floor, etc) */
		{
			int index = table->rowCount();
			table->insertRow(index);
			table->setItem(index, 0, new QTableWidgetItem(kryToQString(key)));
			table->item(index, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
			auto type = const_cast<kry::Media::Config&>(Assets::getHardTypes())[section][key];
			if (type.isEmpty() || type == "VEC_2" || type == "VEC_2_ARR" || type == "ITEM_ID" || type == "ENTITY_ARR" || type == "ITEM_TYPE" || type == "ENTITY_GROUP_ARR" || type == "ITEM_ARR")
				table->setItem(index, 1, new QTableWidgetItem(kryToQString(hardtypesettings[section][key])));
			else if (type == "INT")
			{
				QLineEdit* edit = new QLineEdit(kryToQString(hardtypesettings[section][key]), table);
				edit->setValidator(new QIntValidator(edit));
				table->setCellWidget(index, 1, edit);
			}
			else if (type == "FLOAT")
			{
				QLineEdit* edit = new QLineEdit(kryToQString(hardtypesettings[section][key]), table);
				edit->setValidator(new QDoubleValidator(edit));
				table->setCellWidget(index, 1, edit);
			}
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
				table->setCellWidget(index, 1, box);
				void(QComboBox::* animsSignal)(int) = &QComboBox::currentIndexChanged;
				connect(box, animsSignal, [this, table, box, index](int row)
				{
					auto key = qToKString(table->item(index, 0)->text());
					if (key == "skin" || key == "skinIdle")
					{
						auto text = box->itemText(row);
						text = text.left(text.indexOf(':'));
						if (text != "-1")
							ui->lObject->setPixmap(QIcon(Resources::getAnimations()[text.toInt()]->path).pixmap(ui->lObject->size()));
						else
							ui->lObject->setPixmap(QIcon(Resources::getEditorTexture(EditorResource::MISSING_TILE)->path).pixmap(ui->lObject->size()));
					}
				});
				box->setEditable(false);
				unsigned i = 0;
				box->addItem("-1:none");
				for (auto& anim : Resources::getAnimations())
					box->addItem(QString::number(i++) + ':' + kryToQString(anim->properties[0]["Skins"]["name"]));
				if (!hardtypesettings[section][key].isEmpty())
					box->setCurrentIndex(kry::Util::toIntegral<int>(hardtypesettings[section][key]) + 1);
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
			else if (type == "SOUND_ID")
			{
				QComboBox* box = new QComboBox(table);
				box->setEditable(false);
				unsigned i = 0;
				box->addItem("-1:none");
				for (auto& sound : Assets::getSounds())
					box->addItem(QString::number(i++) + ':' + kryToQString(sound->properties["global"]["name"]));
				if (!hardtypesettings[section][key].isEmpty())
					box->setCurrentIndex(kry::Util::toIntegral<int>(hardtypesettings[section][key]) + 1);
				table->setCellWidget(index, 1, box);
			}
			else if (type == "MUSIC_ID")
			{
				QComboBox* box = new QComboBox(table);
				box->setEditable(false);
				unsigned i = 0;
				box->addItem("-1:none");
				for (auto& music : Assets::getMusic())
					box->addItem(QString::number(i++) + ':' + kryToQString(music->properties["global"]["name"]));
				if (!hardtypesettings[section][key].isEmpty())
					box->setCurrentIndex(kry::Util::toIntegral<int>(hardtypesettings[section][key]) + 1);
				table->setCellWidget(index, 1, box);
			}
			else if (type == "ITEM_ID")
			{
				QComboBox* box = new QComboBox(table);
				box->setEditable(false);
				int index = 0;
				int currindex = 0;
				for (auto& pair : Map::getMap()->getItems())
				{
					box->addItem(kryToQString(pair.first) + ':' + pair.second->name);
					if (hardtypesettings[section][key] == pair.first)
						currindex = index;
					++index;
				}
				box->setCurrentIndex(currindex);
				table->setCellWidget(index, 1, box);
			}
			/** #TODO(incomplete) add parts for whatever else that comes along */
		}
	};
	setupHard(parent);
	setupHard(type);
#if 0
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
#endif


	if (hardtypesettings[type].keyExists("path"))
	{
		table = initTable(new QTableWidget(ui->tabs));
		ui->tabs->addTab(table, "Waypoints");
		table->insertRow(0);
		QPushButton* button = new QPushButton("Edit Waypoints", table); /** #TODO(note) 2 options for waypointcanvas. remove it from the renderer if its not needed, or place the waypoints in the normal canvas instead (after everything else), updateCanvas instead of waypointcanvas */
		button->setToolTip("Saves and closes the Object Settings window and begins recording of waypoint placement.");
		connect(button, &QPushButton::clicked, [this, table, object](bool)
		{
			QString message;
			Tool<>::switchTool(ToolType::WAYPOINT, message); // follow on from here. follower isnt rendering and neither are the waypoints >:|
			WaypointData data;
			if (!table->item(1, 1)->text().isEmpty())
			{
				auto list = qToKString(table->item(1, 1)->text()).explode("},");
				for (auto& vec : list)
				{
					Waypoint waypoint;
					waypoint.owner = object;
					waypoint.position = kry::Util::Vector2f::Vector((vec + '}').trim());
					data.waypoints.push_back(waypoint);
				}
			}
			data.object = object;
			data.looping = table->item(2, 1)->text() == "true";
			Tool<WaypointData>::getTool()->setData(data);
			dynamic_cast<MainWindow*>(this->parent()->parent()->parent())->getUI()->glWidget->updateCanvas();
			dynamic_cast<MainWindow*>(this->parent()->parent()->parent())->getStatusMain()->setText(message);

			lastresult = DialogResult::OK;
			close();
		});
		table->setCellWidget(0, 0, button);
		button = new QPushButton("Clear Waypoints", table);
		connect(button, &QPushButton::clicked, [this, table, object](bool)
		{
			table->item(1, 1)->setText("");
			table->item(2, 1)->setText("false");
			hardtypesettings[settings["global"]["hardtype"]]["path"] = "";
			hardtypesettings[settings["global"]["hardtype"]]["loopPath"] = "false";
		});
		table->setCellWidget(0, 1, button);
		table->insertRow(1);
		table->insertRow(2);
		table->setItem(1, 0, new QTableWidgetItem("path"));
		table->item(1, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		table->setItem(2, 0, new QTableWidgetItem("loopPath"));
		table->item(2, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		if (object->properties["global"]["hardtype"] == "enemy")
		{
			table->setItem(1, 1, new QTableWidgetItem(kryToQString(hardtypesettings[settings["global"]["hardtype"]]["path"])));
			table->item(1, 1)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
			table->setItem(2, 1, new QTableWidgetItem(kryToQString(hardtypesettings[settings["global"]["hardtype"]]["loopPath"])));
			table->item(2, 1)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		}
	}
}
