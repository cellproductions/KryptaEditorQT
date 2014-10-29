#include "EnvBrowserDialog.h"
#include "ui_envbrowserdialog.h"
#include "Resources.h"
#include "Map.h"
#include "ObjectSettingsDialog.h"
#include "Utilities.h"
#include <QInputDialog>

namespace EnvBr
{
	ObjectListItem* createListItem(Asset<kry::Graphics::Texture>* asset, const QString& imagefile, const QString& name)
	{
		std::shared_ptr<Object> object(new Object);
		object->asset = asset;
		object->properties = asset->properties;
		auto type = asset->properties["global"]["hardtype"];
		auto parent = Assets::getParentType(type);
		for (auto& key : const_cast<kry::Media::Config&>(Assets::getHardTypes())[parent].getKeyNames())
			object->hardproperties[parent][key] = "";
		for (auto& key : const_cast<kry::Media::Config&>(Assets::getHardTypes())[type].getKeyNames())
			object->hardproperties[type][key] = "";
		unsigned index = 0;
		for (auto& resource : Resources::getAnimations())
		{
			if (resource.get() == asset->resource)
				break;
			++index;
		}
		auto item = new ObjectListItem(object, QIcon(imagefile), name);
		item->object->hardproperties["floor"]["skin"] = kry::Util::toString(index);
		item->object->hardproperties["floor"]["heuristic"] = type == "solid" ? kry::Util::String("1") : kry::Util::String("0");
		item->object->hardproperties["floor"]["sortDepth"] = type == "wall" ? kry::Util::String("1") : kry::Util::String("0");
		item->object->hardproperties["floor"]["sortPivotOffset"] = "{ 0, 0 }";
		item->path = asset->resource->path;
		return item;
	}
}

EnvBrowserDialog::EnvBrowserDialog(QWidget *parent) : QDialog(parent), ui(new Ui::EnvBrowserDialog), selected(nullptr), lastresult(DialogResult::CANCEL), firstLoad(true)
{
    ui->setupUi(this);

    ui->lbIcons->setIconSize({100, 100});
    ui->lbIcons->setResizeMode(QListWidget::Adjust);
    ui->resProperties->horizontalHeader()->resizeSections(QHeaderView::Interactive);

    connect(ui->lbIcons, &QListWidget::itemClicked, [this](QListWidgetItem* item)
    {
		if (item == nullptr)
		{
			ui->lResName->setText("");
			ui->lResImage->setPixmap(QIcon().pixmap(ui->lResImage->size()));
			while (ui->resProperties->rowCount() > 0)
				ui->resProperties->removeRow(0);
			return;
		}

        ObjectListItem* objectitem = dynamic_cast<ObjectListItem*>(item);

        ui->lResName->setText(kryToQString(objectitem->object->asset->properties["global"]["name"]));
        ui->lResImage->setPixmap(objectitem->icon().pixmap(ui->lResImage->size()));
		while (ui->resProperties->rowCount() > 0)
			ui->resProperties->removeRow(0);
		for (auto& key : objectitem->object->asset->properties["global"].getKeyNames())
		{
			int index = ui->resProperties->rowCount();
			ui->resProperties->insertRow(index);
			ui->resProperties->setItem(index, 0, new QTableWidgetItem(kryToQString(key)));
			ui->resProperties->item(index, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
			ui->resProperties->setItem(index, 1, new QTableWidgetItem(kryToQString(objectitem->object->asset->properties["global"][key])));
			ui->resProperties->item(index, 1)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		}
    });
    connect(ui->bSelect, &QPushButton::clicked, [this]()
    {
		selected = dynamic_cast<ObjectListItem*>(ui->lbIcons->selectedItems().empty() ? nullptr :
												ui->lbIcons->selectedItems()[0]);
		lastresult = ui->lbIcons->selectedItems().size() <= 0 ? DialogResult::CANCEL : DialogResult::OK;
        close();
    });
	connect(ui->bAdd, &QPushButton::clicked, [this]()
	{
		bool ok = false;
		auto name = QInputDialog::getText(this, "Create New Environment Piece", "Enter a name for the piece:", QLineEdit::Normal, QString(), &ok);
		if (!ok || name.isNull() || name.trimmed().isEmpty())
			return;
		QStringList list { "solid", "void", "wall" };
		ok = false;
		auto type = QInputDialog::getItem(this, "Create New Environment Piece", "Select a type for your piece:", list, 0, false, &ok);
		if (!ok || type.isNull() || type.trimmed().isEmpty())
			return;
		list.clear();
		unsigned index = 0;
		for (auto& anim : Resources::getAnimations())
			list.push_back(QString::number(index++) + ':' + kryToQString(anim->properties[0]["Skins"]["name"]));
		ok = false;
		auto stranim = QInputDialog::getItem(this, "Create New Environment Piece", "Select an animation for your piece:", list, 0, false, &ok);
		if (!ok || stranim.isNull() || stranim.trimmed().isEmpty())
			return;
		auto animation = Resources::getAnimations()[stranim.left(stranim.indexOf(':')).toUInt()];

		auto asset = Assets::getTileByHardtype(qToKString(type));
		auto item = EnvBr::createListItem(asset.get(), animation->path, name);
		item->object->hardproperties["floor"]["skin"] = kry::Util::toString(Resources::getAnimationIndex(animation));

		ObjectSettingsDialog dialog(this);
		std::set<Object*> tmp = { item->object.get() };
		if (dialog.showDialog(qToKString("Choose settings for " + name), tmp, true) == DialogResult::OK)
		{
			auto results = dialog.getAllProperties();
			item->object->hardproperties = (*results.begin()).hardtypesettings;
		}
		item->object->properties["global"]["name"] = qToKString(name);
		item->path = Resources::getAnimations()[kry::Util::toUIntegral<size_t>(item->object->hardproperties["floor"]["skin"])]->path;

		ui->lbIcons->addItem(item);
		ui->bRemove->setEnabled(true);
	});
	connect(ui->bRemove, &QPushButton::clicked, [this](bool)
	{
		if (ui->lbIcons->currentItem() == nullptr)
			return;
		delete ui->lbIcons->currentItem();
		ui->lbIcons->itemClicked(nullptr);
		if (ui->lbIcons->count() <= 0)
			ui->bRemove->setEnabled(false);
	});
    connect(ui->bClose, &QPushButton::clicked, [this]()
    {
        lastresult = DialogResult::CANCEL;
        close();
    });
}

EnvBrowserDialog::~EnvBrowserDialog()
{
    delete ui;
}

DialogResult EnvBrowserDialog::showDialog()
{
    if (firstLoad)
	{
        for (auto& asset : Assets::getTiles())
		{
			auto item = EnvBr::createListItem(asset.get(), asset->resource->path, kryToQString(asset->properties["global"]["name"]));
            ui->lbIcons->addItem(item);
		}
		firstLoad = false;
		return lastresult;
	}
	if (ui->lbIcons->count() <= 0)
		ui->bRemove->setEnabled(false);

    this->exec();
    return lastresult;
}
