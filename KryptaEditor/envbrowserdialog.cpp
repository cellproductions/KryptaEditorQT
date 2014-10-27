#include "EnvBrowserDialog.h"
#include "ui_envbrowserdialog.h"
#include "Resources.h"
#include "Map.h"
#include "ObjectSettingsDialog.h"
#include "Utilities.h"
#include <QInputDialog>

namespace
{
	ObjectListItem* createListItem(Asset<kry::Graphics::Texture>* asset, const QString& imagefile, const QString& name)
	{
		std::shared_ptr<Object> object(new Object);
		object->asset = asset;
		object->properties = asset->properties;
		auto type = asset->properties["global"]["hardtype"];
		auto parent = Assets::getParentType(type);
		for (auto& key : const_cast<kry::Media::Config&>(Assets::getHardTypes())[type].getKeyNames())
			object->hardproperties[type][key] = "";
		for (auto& key : const_cast<kry::Media::Config&>(Assets::getHardTypes())[parent].getKeyNames())
			object->hardproperties[type][key] = "";
		return new ObjectListItem(object, QIcon(imagefile), name);
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
		auto name = QInputDialog::getText(this, "Create New Environment Piece", "Enter a name for the piece:");
		if (name.isNull() || name.trimmed().isEmpty())
			return;
		QStringList list { "solid", "void", "wall" };
		auto type = QInputDialog::getItem(this, "Create New Environment Piece", "Select a type for your piece:", list, 0, false);
		if (type.isNull() || type.trimmed().isEmpty())
			return;
		list.clear();
		unsigned index = 0;
		for (auto& anim : Resources::getAnimations())
			list.push_back(QString::number(index++) + ':' + kryToQString(anim->properties[0]["Skins"]["name"]));
		auto stranim = QInputDialog::getItem(this, "Create New Environment Piece", "Select an animation for your piece:", list, 0, false);
		if (stranim.isNull() || stranim.trimmed().isEmpty())
			return;
		auto animation = Resources::getAnimations()[stranim.left(stranim.indexOf(':')).toUInt()];

		auto asset = Assets::getTileByHardtype(qToKString(type));
		auto item = createListItem(asset.get(), animation->path, name);

		ObjectSettingsDialog dialog(this);
		std::set<Object*> tmp = { item->object.get() };
		if (dialog.showDialog(qToKString("Choose settings for " + name), tmp, true) == DialogResult::OK)
		{
			auto results = dialog.getAllProperties();
			item->object->hardproperties = (*results.begin()).hardtypesettings;
		}
		item->object->properties["global"]["name"] = qToKString(name);
		
		// set up the object for use as a template outside (using the object settings dialog)
		/** #TODO(continue) */
		/*
		Asset<kry::Graphics::Texture>* asset = new Asset<kry::Graphics::Texture>;
		asset->path = "";
		asset->type = type == "wall" ? AssetType::ENTITY : AssetType::TILE;
		asset->properties["global"]["name"] = qToKString(name);
		asset->properties["global"]["resource"] = qToKString(animation->path);
		asset->properties["global"]["type"] = type == "wall" ? kry::Util::String("ENTITY") : kry::Util::String("TILE");
		asset->properties["global"]["hardtype"] = qToKString(type);
		asset->properties["global"]["gridsnap"] = "true";
		asset->properties["global"]["relativex"] = "0.5";
		asset->properties["global"]["relativey"] = "0.5";
		asset->resource = animation.get();
		*/

		ui->lbIcons->addItem(item);
		ui->bRemove->setEnabled(false);
	});
	connect(ui->bRemove, &QPushButton::clicked, [this](bool)
	{
		if (ui->lbIcons->currentRow() < 0)
			return;
		ui->lbIcons->removeItemWidget(ui->lbIcons->currentItem());
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
			auto item = createListItem(asset.get(), asset->resource->path, kryToQString(asset->properties["global"]["name"]));
			unsigned index = 0;
			for (auto& resource : Resources::getAnimations())
			{
				if (resource.get() == asset->resource)
					break;
				++index;
			}
			item->object->hardproperties["floor"]["skin"] = kry::Util::toString(index);
            ui->lbIcons->addItem(item);
		}
		firstLoad = false;
	}
	if (ui->lbIcons->count() <= 0)
		ui->bRemove->setEnabled(false);

    this->exec();
    return lastresult;
}
