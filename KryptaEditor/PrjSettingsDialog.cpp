#include "PrjSettingsDialog.h"
#include "ui_PrjSettingsDialog.h"
#include "Map.h"
#include "LayerOptionsItem.h"
#include "Resources.h"
#include "Utilities.h"
#include <Utilities/StringConvert.h>
#include <QComboBox>
#include <QFileDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>

PrjSettingsDialog::PrjSettingsDialog(QWidget *parent) : CSDialog(parent), ui(new Ui::PrjSettingsDialog)
{
	ui->setupUi(this);

	for (int row = 0; row < ui->projectProperties->rowCount(); ++row)
		ui->projectProperties->item(row, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	for (int row = 0; row < ui->playerProperties->rowCount(); ++row)
		ui->playerProperties->item(row, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

	connect(ui->bCancel, &QPushButton::clicked, [this](bool)
	{
		lastresult = DialogResult::CANCEL;
		this->close();
	});

	connect(ui->bSave, &QPushButton::clicked, [this](bool)
	{
		/*
		settings["project"]["name"] = qToKString(ui->projectProperties->item(0, 1)->text());
		settings["project"]["path"] = qToKString(ui->projectProperties->item(1, 1)->text());
		settings["project"]["iconImage"] = qToKString(ui->projectProperties->item(2, 1)->text());
		settings["project"]["fogOfWar"] = qToKString(ui->projectProperties->item(3, 1)->text());
		settings["project"]["revealOfWar"] = qToKString(ui->projectProperties->item(4, 1)->text());
		settings["project"]["fogTint"] = qToKString(ui->projectProperties->item(5, 1)->text());
		settings["project"]["floorFadeTime"] = qToKString(ui->projectProperties->item(6, 1)->text());
		settings["project"]["tileDimensions"] = qToKString(ui->projectProperties->item(7, 1)->text());
		settings["project"]["soudtrackSize"] = qToKString(ui->projectProperties->item(8, 1)->text());
		settings["project"]["randomizeSoundtrack"] = qToKString(ui->projectProperties->item(9, 1)->text());*/
		
		auto saveHard = [this](QTableWidget* table, const kry::Util::String& section)
		{
			for (auto& key : settings[section].getKeyNames())
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

				auto tvalue = const_cast<kry::Media::Config&>(Assets::getHardTypes())[section][key];
				if (tvalue.isEmpty() || tvalue == "VEC_2" || tvalue == "VEC_2_POS" || tvalue == "VEC_4" || tvalue == "VEC_2_ARR" || tvalue == "ITEM_ID" || tvalue == "ENTITY_ARR" || tvalue == "ITEM_TYPE" || 
					tvalue == "ENTITY_GROUP_ARR" || tvalue == "ITEM_ARR") /** #TODO(change) add these types as widgets instead */
					settings[section][key] = qToKString(table->item(rowindex, 1)->text());
				else if (tvalue == "FLOOR_ID")
				{
					auto text = dynamic_cast<QComboBox*>(table->cellWidget(rowindex, 1))->currentText();
					text = text.left(text.indexOf(':'));
					settings[section][key] = qToKString(text);
				}
				else if (tvalue == "IMAGE_FILE")
				{
					auto text = dynamic_cast<QPushButton*>(table->cellWidget(rowindex, 1))->text();
					settings[section][key] = qToKString(text);
				}
				else if (tvalue == "INT" || tvalue == "FLOAT")
				{
					auto text = dynamic_cast<QLineEdit*>(table->cellWidget(rowindex, 1))->text();
					settings[section][key] = qToKString(text);
				}
				else if (tvalue == "BOOL")
					settings[section][key] = dynamic_cast<QCheckBox*>(table->cellWidget(rowindex, 1))->isChecked() ? kry::Util::String("true") : kry::Util::String("false");
				else if (tvalue == "OBJECT_ID")
				{
					auto text = dynamic_cast<QComboBox*>(table->cellWidget(rowindex, 1))->currentText();
					text = text.left(text.indexOf(':'));
					if (text != "-1")
						settings[section][key] = qToKString(text);
				}
				else if (tvalue == "ANIM_ID" || tvalue == "ANIM_ID_FORCE")
				{
					auto text = dynamic_cast<QComboBox*>(table->cellWidget(rowindex, 1))->currentText();
					text = text.left(text.indexOf(':'));
					if (text != "-1") /** #TODO(note) during exporting, value will never be -1 */
						settings[section][key] = qToKString(text);
				}
				else if (tvalue == "ANIM_DIR")
				{
					auto text = dynamic_cast<QComboBox*>(table->cellWidget(rowindex, 1))->currentText();
					settings[section][key] = qToKString(text).substring(0, 1);
				}
				else if (tvalue == "DIR")
				{
					auto value = dynamic_cast<QSpinBox*>(table->cellWidget(rowindex, 1))->value();
					settings[section][key] = kry::Util::toString(value);
				}
				else if (tvalue == "SOUND_ID")
				{
					auto text = dynamic_cast<QComboBox*>(table->cellWidget(rowindex, 1))->currentText();
					text = text.left(text.indexOf(':'));
					if (text != "-1")
						settings[section][key] = qToKString(text);
				}
				else if (tvalue == "MUSIC_ID")
				{
					auto text = dynamic_cast<QComboBox*>(table->cellWidget(rowindex, 1))->currentText();
					text = text.left(text.indexOf(':'));
					if (text != "-1")
						settings[section][key] = qToKString(text);
				}
				else if (tvalue == "ITEM_ID")
				{
					auto text = dynamic_cast<QComboBox*>(table->cellWidget(rowindex, 1))->currentText();
					text = text.left(text.indexOf(':'));
					settings[section][key] = qToKString(text);
				}
				else if (tvalue == "WEAPONS")
				{
					auto text = dynamic_cast<QComboBox*>(table->cellWidget(rowindex, 1))->currentText();
					settings[section][key] = qToKString(text);
				}
				/** #TODO(incomplete) add parts for whatever else that comes along */
			}
		};
		saveHard(ui->projectProperties, "project");
		saveHard(ui->playerProperties, "entity");
		saveHard(ui->playerProperties, "player");

		lastresult = DialogResult::OK;
		this->close();
	});
	connect(ui->projectProperties, &QTableWidget::cellDoubleClicked, [this](int row, int column)
	{
		if (row != 2 || column != 1)
			return;
		auto filepath = QFileDialog::getOpenFileName(this, "Select Map Image", QApplication::applicationDirPath(), "Image files (*.jpg *.png *.bmp)");
		if (filepath.isNull() || filepath.isEmpty())
			return;
		ui->projectProperties->item(row, column)->setText(filepath);
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
	settings["project"]["iconImage"] = "icon.png";
	settings["project"]["fogOfWar"] = "true";
	settings["project"]["revealOfWar"] = "true";
	settings["project"]["fogTint"] = "{ 0.4, 0.4, 0.4, 1.0 }";
	settings["project"]["fogThroughWalls"] = "false";
	settings["project"]["fogTillLastWall"] = "true";
	settings["project"]["tileDimensions"] = "700";
	settings["project"]["floorFadeTime"] = "250";
	settings["project"]["cameraScale"] = "0.2";
	settings["project"]["deathFadeTime"] = "5000";
	settings["project"]["gameOverSkin"] = "0";
	settings["project"]["soundtrackSize"] = "0";
	settings["project"]["randomizeSoundtrack"] = "true";
	
	if (!settings["entity"].keyExists("skinConfig"))
		settings["entity"]["skinConfig"] = "";
	if (!settings["entity"].keyExists("skinIdle"))
		settings["entity"]["skinIdle"] = "0";
	if (!settings["entity"].keyExists("skinRun"))
		settings["entity"]["skinRun"] = "-1";
	if (!settings["entity"].keyExists("skinDead"))
		settings["entity"]["skinDead"] = "-1";
	if (!settings["entity"].keyExists("position"))
		settings["entity"]["position"] = "{ 350, 0 }";
	if (!settings["entity"].keyExists("dimensions"))
		settings["entity"]["dimensions"] = "{ 1, 1 }";
	if (!settings["entity"].keyExists("floor"))
		settings["entity"]["floor"] = kry::Util::toString(Map::getMap()->getCurrentLayer()->index);
	if (!settings["entity"].keyExists("direction"))
		settings["entity"]["direction"] = "0";
	if (!settings["entity"].keyExists("directions"))
		settings["entity"]["directions"] = "1";
	if (!settings["entity"].keyExists("seeInFog"))
		settings["entity"]["seeInFog"] = "true";
	if (!settings["entity"].keyExists("maxHeuristic"))
		settings["entity"]["maxHeuristic"] = "0";
	if (!settings["entity"].keyExists("group"))
		settings["entity"]["group"] = "-1";

	if (!settings["player"].keyExists("viewDistance"))
		settings["player"]["viewDistance"] = "3";
	if (!settings["player"].keyExists("moveAcceleration"))
		settings["player"]["moveAcceleration"] = "1.3";
	if (!settings["player"].keyExists("turnAcceleration"))
		settings["player"]["turnAcceleration"] = "50";
	if (!settings["player"].keyExists("maxMoveSpeed"))
		settings["player"]["maxMoveSpeed"] = "6";
	if (!settings["player"].keyExists("maxTurnSpeed"))
		settings["player"]["maxTurnSpeed"] = "700";
	if (!settings["player"].keyExists("health"))
		settings["player"]["health"] = "1";
	if (!settings["player"].keyExists("idleSound"))
		settings["player"]["idleSound"] = "-1";
	if (!settings["player"].keyExists("runSound"))
		settings["player"]["runSound"] = "-1";
	if (!settings["player"].keyExists("deadSound"))
		settings["player"]["deadSound"] = "-1";
	if (!settings["player"].keyExists("inventory"))
		settings["player"]["inventory"] = "";
	if (!settings["player"].keyExists("invincibilityTime"))
		settings["player"]["invincibilityTime"] = "1000";
	if (!settings["player"].keyExists("invincibilityFlickerTime"))
		settings["player"]["invincibilityFlickerTime"] = "75";
	if (!settings["player"].keyExists("dieingTime"))
		settings["player"]["dieingTime"] = "1000";
	if (!settings["player"].keyExists("idleSound"))
		settings["player"]["idleSound"] = "-1";
	if (!settings["player"].keyExists("runSound"))
		settings["player"]["runSound"] = "-1";
	if (!settings["player"].keyExists("deadSound"))
		settings["player"]["deadSound"] = "-1";
	if (!settings["player"].keyExists("hurtSound"))
		settings["player"]["hurtSound"] = "-1";
	if (!settings["player"].keyExists("dieingSound"))
		settings["player"]["dieingSound"] = "-1";
}

void PrjSettingsDialog::setTableData()
{
	while (ui->projectProperties->rowCount() > 0)
		ui->projectProperties->removeRow(0);
	settings["project"]["soundtrackSize"] = kry::Util::toString(Assets::getMusic().size());
	/*
	ui->projectProperties->setItem(0, 1, new QTableWidgetItem(kryToQString(settings["project"]["name"])));
	ui->projectProperties->item(0, 1)->setFlags(Qt::ItemIsSelectable);
	ui->projectProperties->setItem(1, 1, new QTableWidgetItem(kryToQString(settings["project"]["path"])));
	ui->projectProperties->item(1, 1)->setFlags(Qt::ItemIsSelectable);
	ui->projectProperties->setItem(2, 1, new QTableWidgetItem(kryToQString(settings["project"]["iconImage"])));
	ui->projectProperties->setItem(3, 1, new QTableWidgetItem(kryToQString(settings["project"]["fogOfWar"])));
	ui->projectProperties->setItem(4, 1, new QTableWidgetItem(kryToQString(settings["project"]["revealOfWar"])));
	ui->projectProperties->setItem(5, 1, new QTableWidgetItem(kryToQString(settings["project"]["fogTint"])));
	ui->projectProperties->setItem(6, 1, new QTableWidgetItem(kryToQString(settings["project"]["floorFadeTime"])));
	ui->projectProperties->setItem(7, 1, new QTableWidgetItem(kryToQString(settings["project"]["tileDimensions"])));
	ui->projectProperties->setItem(8, 1, new QTableWidgetItem(kryToQString(settings["project"]["soundTrackSize"])));
	ui->projectProperties->setItem(9, 1, new QTableWidgetItem(kryToQString(settings["project"]["randomizeSoundtrack"])));
	*/

	while (ui->playerProperties->rowCount() > 0)
		ui->playerProperties->removeRow(0);
	auto setupHard = [this](QTableWidget* table, const kry::Util::String& section)
	{
		for (auto& key : settings[section].getKeyNames())
		{
			int index = table->rowCount();
			table->insertRow(index);
			table->setItem(index, 0, new QTableWidgetItem(kryToQString(key)));
			table->item(index, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
			auto type = const_cast<kry::Media::Config&>(Assets::getHardTypes())[section][key];
			if (type.isEmpty() || type == "VEC_2" || type == "VEC_2_POS" || type == "VEC_4" || type == "VEC_2_ARR" || type == "ITEM_ID" || type == "ENTITY_ARR" || type == "ITEM_TYPE" || type == "ENTITY_GROUP_ARR" || type == "ITEM_ARR")
				table->setItem(index, 1, new QTableWidgetItem(kryToQString(settings[section][key])));
			else if (type == "BLOCK")
			{
				table->setItem(index, 1, new QTableWidgetItem(kryToQString(settings[section][key])));
				table->item(index, 1)->setFlags(Qt::ItemIsSelectable);
			}
			else if (type == "IMAGE_FILE")
			{
				QPushButton* button = new QPushButton(kryToQString(settings[section][key]), table);
				connect(button, &QPushButton::clicked, [this, button](bool)
				{
					auto filepath = QFileDialog::getOpenFileName(this, "Select Map Image", QApplication::applicationDirPath(), "Image files (*.jpg *.png *.bmp)");
					if (filepath.isNull() || filepath.isEmpty())
						return;
					button->setText(filepath);
				});
				table->setCellWidget(index, 1, button);
			}
			else if (type == "INT")
			{
				QLineEdit* edit = new QLineEdit(kryToQString(settings[section][key]), table);
				edit->setValidator(new QIntValidator(edit));
				table->setCellWidget(index, 1, edit);
			}
			else if (type == "FLOAT")
			{
				QLineEdit* edit = new QLineEdit(kryToQString(settings[section][key]), table);
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
				if (!settings[section][key].isEmpty())
					box->setCurrentIndex(kry::Util::toIntegral<int>(settings[section][key]));
				table->setCellWidget(index, 1, box);
			}
			else if (type == "BOOL")
			{
				QCheckBox* box = new QCheckBox(table);
				if (!settings[section][key].isEmpty())
					box->setChecked(settings[section][key] == "true");
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
						if (settings[section][key] == id)
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
				box->setEditable(false);
				bool ismainskin = type == "ANIM_ID_FORCE";
				unsigned i = 0;
				if (!ismainskin)
					box->addItem("-1:none");
				for (auto& anim : Resources::getAnimations())
					box->addItem(QString::number(i++) + ':' + kryToQString(anim->properties[0]["Skins"]["name"]));
				if (!settings[section][key].isEmpty())
					box->setCurrentIndex(kry::Util::toIntegral<int>(settings[section][key]) + (ismainskin ? 0 : 1));
			}
			else if (type == "ANIM_DIR")
			{
				QComboBox* box = new QComboBox(table);
				box->setEditable(false);
				box->addItem("1 direction");
				box->addItem("4 directions");
				box->addItem("8 directions");
				if (!settings[section][key].isEmpty())
				{
					int dir = kry::Util::toIntegral<int>(settings[section][key]);
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
				if (!settings[section][key].isEmpty())
					box->setValue(kry::Util::toIntegral<int>(settings[section][key]));
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
				if (!settings[section][key].isEmpty())
					box->setCurrentIndex(kry::Util::toIntegral<int>(settings[section][key]) + 1);
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
				if (!settings[section][key].isEmpty())
					box->setCurrentIndex(kry::Util::toIntegral<int>(settings[section][key]) + 1);
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
					if (settings[section][key] == pair.first)
						currindex = index;
					++index;
				}
				box->setCurrentIndex(currindex);
				table->setCellWidget(index, 1, box);
			}
			else if (type == "WEAPONS")
			{
				QComboBox* box = new QComboBox(table);
				box->setEditable(false);
				box->addItem("projectile");
				box->addItem("melee");
				box->addItem("area");
				if (!settings[section][key].isEmpty())
				{
					if (settings[section][key] == "projectile")
						box->setCurrentIndex(0);
					else if (settings[section][key] == "melee")
						box->setCurrentIndex(1);
					else if (settings[section][key] == "area")
						box->setCurrentIndex(2);
				}
				table->setCellWidget(index, 1, box);
			}
			/** #TODO(incomplete) add parts for whatever else that comes along */
		}
	};
	setupHard(ui->projectProperties, "project");
	setupHard(ui->playerProperties, "entity");
	setupHard(ui->playerProperties, "player");
}
