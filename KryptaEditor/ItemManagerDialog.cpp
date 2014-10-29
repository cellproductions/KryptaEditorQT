#include "ItemManagerDialog.h"
#include "ui_ItemManagerDialog.h"
#include "Map.h"
#include "Resources.h"
#include "Utilities.h"
#include <QInputDialog>
#include <QComboBox>
#include <QLineEdit>

unsigned ItemManagerDialog::idcount(0);

ItemManagerDialog::ItemManagerDialog(QWidget *parent) : CSDialog(parent), ui(new Ui::ItemManagerDialog)
{
	ui->setupUi(this);

	connect(ui->lbItems, &QListWidget::currentRowChanged, [this](int index)
	{
		if (index < 0 || index >= ui->lbItems->count())
		{
			while (ui->tItemProps->rowCount() > 0)
				ui->tItemProps->removeRow(0);
			return;
		}

		auto lbitem = ui->lbItems->currentItem();
		while (ui->tItemProps->rowCount() > 0)
			ui->tItemProps->removeRow(0);
		auto item = Map::getMap()->getItems()[qToKString(lbitem->text().left(lbitem->text().indexOf(':')))];
		auto setup = [this, &item, lbitem](const kry::Util::String& section)
		{
			for (auto& key : item->properties[section].getKeyNames())
			{
				int rowindex = ui->tItemProps->rowCount();
				ui->tItemProps->insertRow(rowindex);
				ui->tItemProps->setItem(rowindex, 0, new QTableWidgetItem(kryToQString(key)));
				ui->tItemProps->item(rowindex, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
				auto type = const_cast<kry::Media::Config&>(Assets::getHardTypes())[section][key];
				if (type.isEmpty())
					ui->tItemProps->setItem(rowindex, 1, new QTableWidgetItem(kryToQString(item->properties[section][key])));
				else if (type == "ITEM_TYPE") /** #TODO(incomplete) this should reset the table (giving it the new type key/values) */
				{
					QComboBox* box = new QComboBox(ui->tItemProps);
					box->setEditable(false);
					box->addItem("loot"); /** #TODO(incomplete) add the rest of the types here */
					box->addItem("key");
					box->addItem("weapon");
					void(QComboBox::* animsSignal)(int) = &QComboBox::currentIndexChanged;
					connect(box, animsSignal, [this, item, section, key, box](int)
					{
						item->properties[section][key] = qToKString(box->currentText() + "item");
					});
					int index = 0;
					for (int i = 0; i < box->count(); ++i)
					{
						auto val = item->properties[section][key];
						if (val.substring(0, val.indexOf("item")) == qToKString(box->itemText(i)))
						{
							index = i;
							break;
						}
					}
					box->setCurrentIndex(index);
					ui->tItemProps->setCellWidget(rowindex, 1, box);
				}
				else if (type == "INT")
				{
					QLineEdit* edit = new QLineEdit(kryToQString(item->properties[section][key]), ui->tItemProps);
					edit->setValidator(new QIntValidator(edit));
					connect(edit, &QLineEdit::textChanged, [this, item, section, key](const QString& text)
					{
						item->properties[section][key] = qToKString(text);
					});
					ui->tItemProps->setCellWidget(rowindex, 1, edit);
				}
				else if (type == "ANIM_ID")
				{
					QComboBox* box = new QComboBox(ui->tItemProps);
					box->setEditable(false);
					unsigned index = 0;
					for (auto& anim : Resources::getAnimations())
						box->addItem(QString::number(index++) + ':' + kryToQString(anim->properties[0]["Skins"]["name"]));
					void(QComboBox::* animsSignal)(int) = &QComboBox::currentIndexChanged;
					connect(box, animsSignal, [this, item, section, key, box](int)
					{
						item->properties[section][key] = kry::Util::toString(box->currentIndex());
						ui->lbItems->currentItem()->setIcon(QIcon(Resources::getAnimations()[box->currentIndex()]->path));
					});
					if (!item->properties[section][key].isEmpty())
						box->setCurrentIndex(kry::Util::toIntegral<int>(item->properties[section][key]));
					ui->tItemProps->setCellWidget(rowindex, 1, box);
				}
			}
		};
		setup("item");
		setup(item->properties["item"]["type"]);
	});
	connect(ui->bAdd, &QPushButton::clicked, [this](bool)
	{
		bool ok = false;
		auto name = QInputDialog::getText(this, "Create New Item", "Enter a name for your item:", QLineEdit::Normal, QString(), &ok);
		if (!ok || name.isNull() || name.trimmed().isEmpty())
			return;
		QStringList list;
		list.push_back("loot"); /** #TODO(incomplete) add the rest of the types here */
		list.push_back("key");
		list.push_back("weapon");
		ok = false;
		auto type = QInputDialog::getItem(this, "Create New Item", "Select a type for your item:", list, 0, false, &ok);
		if (!ok || type.isNull() || type.trimmed().isEmpty())
			return;
		list.clear();
		unsigned index = 0;
		for (auto anim : Resources::getAnimations())
			list.push_back(QString::number(index++) + ':' + kryToQString(anim->properties[0]["Skins"]["name"]));
		ok = false;
		auto anim = QInputDialog::getItem(this, "Create New Item", "Select an animation for your item:", list, 0, false, &ok);
		if (!ok || anim.isNull() || anim.trimmed().isEmpty())
			return;
		anim = anim.left(anim.indexOf(':'));

		auto id = kry::Util::toString(idcount++);
		Item* item = new Item;
		item->name = name;
		for (auto& key : const_cast<kry::Media::Config&>(Assets::getHardTypes())["item"].getKeyNames())
			item->properties["item"][key] = "";
		type = type + "item";
		item->properties["item"]["type"] = qToKString(type);
		item->properties["item"]["inventorySkin"] = qToKString(anim);
		auto ktype = qToKString(type);
		for (auto& key : const_cast<kry::Media::Config&>(Assets::getHardTypes())[ktype].getKeyNames())
			item->properties[ktype][key] = "";
		Map::getMap()->getItems().emplace(kry::Util::String(id), std::make_shared<Item>(*item));

		auto animation = Resources::getAnimations()[kry::Util::toUIntegral<size_t>(qToKString(anim))];
		ui->lbItems->addItem(new QListWidgetItem(QIcon(animation->path), kryToQString(id) + ':' + name));
		
		ui->lbItems->setCurrentRow(ui->lbItems->count() - 1);

		if (!ui->bDelete->isEnabled())
			ui->bDelete->setEnabled(true);
	});
	connect(ui->bDelete, &QPushButton::clicked, [this](bool)
	{
		if (ui->lbItems->currentRow() < 0)
			return;

		auto id = ui->lbItems->currentItem()->text();
		id = id.left(id.indexOf(':'));

		Map::getMap()->getItems().erase(Map::getMap()->getItems().find(qToKString(id)));
		delete ui->lbItems->takeItem(ui->lbItems->currentRow());
		
		if (ui->lbItems->count() <= 0)
			ui->bDelete->setEnabled(false);
	});
	connect(ui->bDone, &QPushButton::clicked, [this](bool)
	{
		lastresult = DialogResult::OK;
		close();
	});
}

ItemManagerDialog::~ItemManagerDialog()
{
	delete ui;
}

DialogResult ItemManagerDialog::showDialog()
{
	setup();
	if (ui->lbItems->count() <= 0)
		ui->bDelete->setEnabled(false);
	ui->lbItems->setCurrentRow(0);

	this->exec();

	return lastresult;
}

void ItemManagerDialog::setup()
{
	while (ui->lbItems->count() > 0)
		delete ui->lbItems->item(0);
	for (auto pair : Map::getMap()->getItems())
	{
		auto value = kryToQString(pair.second->properties["item"]["inventorySkin"]);
		auto anim = Resources::getAnimations()[kry::Util::toUIntegral<size_t>(qToKString(value))];
		ui->lbItems->addItem(new QListWidgetItem(QIcon(anim->path), kryToQString(pair.first) + ':' + pair.second->name));
	}
}
