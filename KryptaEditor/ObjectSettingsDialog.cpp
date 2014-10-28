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


kry::Util::String getValue(QTableWidget* table, unsigned rowindex, const kry::Util::String& section, kry::Util::String& key) /** #TODO(incomplete) add parts for whatever else that comes along */
{
	auto tvalue = const_cast<kry::Media::Config&>(Assets::getHardTypes())[section][key];
	if (tvalue.isEmpty() || tvalue == "VEC_2" || tvalue == "VEC_2_POS" || tvalue == "VEC_2_ARR" || tvalue == "ENTITY_ARR" || tvalue == "ITEM_TYPE" || 
		tvalue == "ENTITY_GROUP_ARR" || tvalue == "ITEM_ARR") /** #TODO(change) add these types as widgets instead */
		return qToKString(table->item(rowindex, 1)->text());
	else if (tvalue == "FLOOR_ID")
	{
		auto text = dynamic_cast<QComboBox*>(table->cellWidget(rowindex, 1))->currentText();
		text = text.left(text.indexOf(':'));
		return qToKString(text);
	}
	else if (tvalue == "INT" || tvalue == "FLOAT")
	{
		auto text = dynamic_cast<QLineEdit*>(table->cellWidget(rowindex, 1))->text();
		return qToKString(text);
	}
	else if (tvalue == "BOOL")
		return qToKString(dynamic_cast<QCheckBox*>(table->cellWidget(rowindex, 1))->isChecked() ? "true" : "false");
	else if (tvalue == "OBJECT_ID")
	{
		auto text = dynamic_cast<QComboBox*>(table->cellWidget(rowindex, 1))->currentText();
		text = text.left(text.indexOf(':'));
		if (text != "-1")
			return qToKString(text);
	}
	else if (tvalue == "ANIM_ID" || tvalue == "ANIM_ID_FORCE")
	{
		auto text = dynamic_cast<QComboBox*>(table->cellWidget(rowindex, 1))->currentText();
		text = text.left(text.indexOf(':'));
		if (text != "-1") /** #TODO(note) during exporting, value will never be -1 */
			return qToKString(text);
	}
	else if (tvalue == "ANIM_DIR")
	{
		auto text = dynamic_cast<QComboBox*>(table->cellWidget(rowindex, 1))->currentText();
		return qToKString(text).substring(0, 1);
	}
	else if (tvalue == "DIR")
	{
		auto value = dynamic_cast<QSpinBox*>(table->cellWidget(rowindex, 1))->value();
		return kry::Util::toString(value);
	}
	else if (tvalue == "SOUND_ID")
	{
		auto text = dynamic_cast<QComboBox*>(table->cellWidget(rowindex, 1))->currentText();
		text = text.left(text.indexOf(':'));
		if (text != "-1")
			return qToKString(text);
	}
	else if (tvalue == "MUSIC_ID")
	{
		auto text = dynamic_cast<QComboBox*>(table->cellWidget(rowindex, 1))->currentText();
		text = text.left(text.indexOf(':'));
		if (text != "-1")
			return qToKString(text);
	}
	else if (tvalue == "ITEM_ID")
	{
		auto text = dynamic_cast<QComboBox*>(table->cellWidget(rowindex, 1))->currentText();
		text = text.left(text.indexOf(':'));
		return qToKString(text);
	}
	else if (tvalue == "WEAPONS")
	{
		auto text = dynamic_cast<QComboBox*>(table->cellWidget(rowindex, 1))->currentText();
		return qToKString(text);
	}
	return kry::Util::String();
}

ObjectSettingsDialog::ObjectSettingsDialog(QWidget *parent) : CSDialog(parent), ui(new Ui::ObjectSettingsDialog)
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
		
		unsigned tabindex = 0;
		for (auto& object : objectprops)
		{
			QTableWidget* table = dynamic_cast<QTableWidget*>(ui->tabs->widget(tabindex++)); // not sure if this is safe, test it for a bit i spose
			kry::Util::String type = object.type;
			kry::Util::String parent = Assets::getParentType(type);
		
			auto saveHard = [this, table, &object](const kry::Util::String& section)
			{
				for (auto& key : object.hardtypesettings[section].getKeyNames())
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

					object.hardtypesettings[section][key] = getValue(table, rowindex, section, key);
				}
			};
			saveHard(parent);
			saveHard(type);
		}

		// ignore events stuff for now
		
		if (!ingroup)
		{
			auto& object = *objectprops.begin();
			auto type = object.type;
			if (object.hardtypesettings[type].keyExists("path"))
			{
				QTableWidget* table = dynamic_cast<QTableWidget*>(ui->tabs->widget(1)); // waypoints tab
				object.hardtypesettings[type]["path"] = qToKString(table->item(1, 1)->text());
				object.hardtypesettings[type]["loopPath"] = dynamic_cast<QCheckBox*>(table->cellWidget(2, 1))->isChecked() ? kry::Util::String("true") : kry::Util::String("false");
			}
		}

		this->close();
	});
	connect(ui->bApply, &QPushButton::clicked, [this]() /** #TODO(incomplete) broken, needs a fix. also, cant garuntee that everything will update sprite/change floor/etc */
	{
		/*
		ui->bSave->click();
		for (auto& layer : Map::getMap()->getLayers())
		{
			for (auto& tile : layer->tiles)
			{
				if (tile.hardproperties["entity"]["skinIdle"] == hardtypesettings["entity"]["skinIdle"])
					tile.hardproperties = hardtypesettings;
				else
				{
					for (auto& object : tile.objects)
						if (object->hardproperties["entity"]["skinIdle"] == hardtypesettings["entity"]["skinIdle"])
							object->hardproperties = hardtypesettings;
				}
			}
		}
		*/
	});
}

ObjectSettingsDialog::~ObjectSettingsDialog()
{
	delete ui;
}

DialogResult ObjectSettingsDialog::showDialog(const kry::Util::String& title, std::set<Object*>& objects, bool group)
{
	ingroup = group;
	QString t = kryToQString(title);
	t[0] = t[0].toUpper();
	this->setWindowTitle(t);
	objectprops.clear();
	for (auto object : objects)
	{
		ObjectProperties o = { object->properties["global"]["hardtype"], object->properties, object->hardproperties };
		objectprops.insert(o);
	}
	if (!ingroup)
	{
		auto object = *objects.begin();
		auto parent = Assets::getParentType(object->properties["global"]["hardtype"]);
		kry::Util::String value = parent == "entity" ? object->hardproperties[parent]["skinIdle"] : object->hardproperties[parent]["skin"];
		assert(!value.isEmpty());
		auto anim = value == "-1" ? Resources::getEditorTexture(EditorResource::MISSING_TILE) : Resources::getAnimations()[kry::Util::toUIntegral<size_t>(value)];
		ui->lObject->setPixmap(QIcon(anim->path).pixmap(ui->lObject->size()));
	}
	else
	{
		auto anim = Resources::getEditorTexture(EditorResource::GROUP);
		ui->lObject->setPixmap(QIcon(anim->path).pixmap(ui->lObject->size()));
	}

	updateTables(objects);

	return CSDialog::showDialog();
}

QTableWidget* initTable(QTableWidget* table, bool addthird = false)
{
	table->setVerticalHeader(new QHeaderView(Qt::Vertical, table));
	table->verticalHeader()->setVisible(false);
	table->setHorizontalHeader(new QHeaderView(Qt::Horizontal, table));
	table->horizontalHeader()->setVisible(false);
	table->horizontalHeader()->setStretchLastSection(true);
	table->insertColumn(0);
	table->insertColumn(0);
	if (addthird)
		table->insertColumn(0);

	return table;
}

void ObjectSettingsDialog::updateTables(std::set<Object*>& objects)
{
	while (ui->tabs->count() > 0)
		delete ui->tabs->widget(0);
	
	for (auto& object : objectprops)
	{
		QTableWidget* table = initTable(new QTableWidget(ui->tabs));
		ui->tabs->addTab(table, kryToQString(object.type));
		kry::Util::String type = object.type;
		kry::Util::String parent = Assets::getParentType(type);
		auto setupHard = [this, &object](QTableWidget* table, const kry::Util::String& section)
		{
			for (auto& key : object.hardtypesettings[section].getKeyNames()) /** #TODO(incomplete) some of these should actually do things (change animation, move floor, etc) */
			{
				int index = table->rowCount();
				table->insertRow(index);
				table->setItem(index, 0, new QTableWidgetItem(kryToQString(key)));
				table->item(index, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
				auto type = const_cast<kry::Media::Config&>(Assets::getHardTypes())[section][key];
				if (type.isEmpty() || type == "VEC_2" || type == "VEC_2_POS" || type == "VEC_2_ARR" || type == "ENTITY_ARR" || type == "ITEM_TYPE" || type == "ENTITY_GROUP_ARR" || type == "ITEM_ARR")
					table->setItem(index, 1, new QTableWidgetItem(kryToQString(object.hardtypesettings[section][key])));
				else if (type == "INT")
				{
					QLineEdit* edit = new QLineEdit(kryToQString(object.hardtypesettings[section][key]), table);
					edit->setValidator(new QIntValidator(edit));
					table->setCellWidget(index, 1, edit);
				}
				else if (type == "FLOAT")
				{
					QLineEdit* edit = new QLineEdit(kryToQString(object.hardtypesettings[section][key]), table);
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
					if (!object.hardtypesettings[section][key].isEmpty())
						box->setCurrentIndex(kry::Util::toIntegral<int>(object.hardtypesettings[section][key]));
					table->setCellWidget(index, 1, box);
				}
				else if (type == "BOOL")
				{
					QCheckBox* box = new QCheckBox(table);
					if (!object.hardtypesettings[section][key].isEmpty())
						box->setChecked(object.hardtypesettings[section][key] == "true");
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
							if (object.hardtypesettings[section][key] == id)
								curr = index;
							++index;
						}
					}
					box->setCurrentIndex(curr);
					table->setCellWidget(index, 1, box);
				}
				else if (type == "ANIM_ID" || type == "ANIM_ID_FORCE")
				{
					QComboBox* box = new QComboBox(table);
					table->setCellWidget(index, 1, box);
					void(QComboBox::* animsSignal)(int) = &QComboBox::currentIndexChanged;
					connect(box, animsSignal, [this, table, box, index, type](int row)
					{
						auto key = qToKString(table->item(index, 0)->text());
						if (!ingroup && (type == "ANIM_ID_FORCE"))
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
					bool ismainskin = type == "ANIM_ID_FORCE";
					unsigned i = 0;
					if (!ismainskin)
						box->addItem("-1:none");
					for (auto& anim : Resources::getAnimations())
						box->addItem(QString::number(i++) + ':' + kryToQString(anim->properties[0]["Skins"]["name"]));
					if (!object.hardtypesettings[section][key].isEmpty())
						box->setCurrentIndex(kry::Util::toIntegral<int>(object.hardtypesettings[section][key]) + (ismainskin ? 0 : 1));
				}
				else if (type == "ANIM_DIR")
				{
					QComboBox* box = new QComboBox(table);
					box->setEditable(false);
					box->addItem("1 direction");
					box->addItem("4 directions");
					box->addItem("8 directions");
					if (!object.hardtypesettings[section][key].isEmpty())
					{
						int dir = kry::Util::toIntegral<int>(object.hardtypesettings[section][key]);
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
					if (!object.hardtypesettings[section][key].isEmpty())
						box->setValue(kry::Util::toIntegral<int>(object.hardtypesettings[section][key]));
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
					if (!object.hardtypesettings[section][key].isEmpty())
						box->setCurrentIndex(kry::Util::toIntegral<int>(object.hardtypesettings[section][key]) + 1);
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
					if (!object.hardtypesettings[section][key].isEmpty())
						box->setCurrentIndex(kry::Util::toIntegral<int>(object.hardtypesettings[section][key]) + 1);
					table->setCellWidget(index, 1, box);
				}
				else if (type == "ITEM_ID")
				{
					QComboBox* box = new QComboBox(table);
					box->setEditable(false);
					box->addItem("-1:none");
					for (auto& pair : Map::getMap()->getItems())
						box->addItem(kryToQString(pair.first) + ':' + pair.second->name);
					box->setCurrentIndex(kry::Util::toIntegral<int>(object.hardtypesettings[section][key]) + 1);
					table->setCellWidget(index, 1, box);
				}
				else if (type == "WEAPONS")
				{
					QComboBox* box = new QComboBox(table);
					box->setEditable(false);
					box->addItem("projectile");
					box->addItem("melee");
					box->addItem("area");
					box->addItem("trap");
					if (!object.hardtypesettings[section][key].isEmpty())
					{
						if (object.hardtypesettings[section][key] == "projectile")
							box->setCurrentIndex(0);
						else if (object.hardtypesettings[section][key] == "melee")
							box->setCurrentIndex(1);
						else if (object.hardtypesettings[section][key] == "area")
							box->setCurrentIndex(2);
						else if (object.hardtypesettings[section][key] == "trap")
							box->setCurrentIndex(3);
					}
					table->setCellWidget(index, 1, box);
				}
				/** #TODO(incomplete) add parts for whatever else that comes along */
				/*
				if (!ingroup)
				{
					QPushButton* button = new QPushButton("Apply All for Property", table);
					button->setToolTip("Applies this value to this property across all objects of the same type as this one (on this floor only).");
					connect(button, &QPushButton::clicked, [this, table, index, section, &object](bool)
					{
						auto key = qToKString(table->item(index, 0)->text());
						auto value = getValue(table, index, section, key);
						auto parent = Assets::getParentType(object.type);
						if (parent == "entity")
						{
							for (auto& tile : Map::getMap()->getCurrentLayer()->tiles)
							{
								for (auto& obj : tile.objects)
								{
									if (obj->properties["global"]["id"] == object.settings["global"]["id"] && obj->properties["global"]["hardtype"] == object.type)
										continue;
									if (obj->properties["global"]["hardtype"] == object.type)
										obj->hardproperties[section][key] = value;
								}
							}
						}
						else // floor
						{
							for (auto& tile : Map::getMap()->getCurrentLayer()->tiles)
							{
								if (tile.properties["global"]["id"] != object.settings["global"]["id"] && tile.properties["global"]["hardtype"] == object.type)
									tile.hardproperties[section][key] = value;
							}
						}
					});
					table->setCellWidget(index, 2, button);
				}
				*/
			}
		};
		setupHard(table, parent);
		setupHard(table, type);
	}
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

	if (!ingroup)
	{
		auto object = *objects.begin();
		auto type = object->properties["global"]["hardtype"];
		auto& propobject = *objectprops.begin();
		if (propobject.hardtypesettings[type].keyExists("path"))
		{
			QTableWidget* table = initTable(new QTableWidget(ui->tabs));
			ui->tabs->addTab(table, "waypoints");
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
			connect(button, &QPushButton::clicked, [this, table, &propobject](bool)
			{
				table->item(1, 1)->setText("");
				table->item(2, 1)->setText("false");
				propobject.hardtypesettings[propobject.type]["path"] = "";
				propobject.hardtypesettings[propobject.type]["loopPath"] = "false";
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
				table->setItem(1, 1, new QTableWidgetItem(kryToQString(propobject.hardtypesettings[propobject.type]["path"])));
				table->item(1, 1)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
				QCheckBox* box = new QCheckBox(table);
				if (!propobject.hardtypesettings[propobject.type]["loopPath"].isEmpty())
					box->setChecked(propobject.hardtypesettings[propobject.type]["loopPath"] == "true");
				table->setCellWidget(2, 1, box);
				table->item(2, 1)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
			}
		}
	}
}
