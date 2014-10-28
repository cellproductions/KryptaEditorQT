#include "EntBrowserDialog.h"
#include "ui_EntBrowserDialog.h"
#include "Resources.h"
#include "Map.h"
#include "Utilities.h"

namespace
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
		auto item = new ObjectListItem(object, QIcon(imagefile), name);unsigned index = 0;
		for (auto& resource : Resources::getAnimations())
		{
			if (resource.get() == asset->resource)
				break;
			++index;
		}
		item->object->hardproperties["entity"]["skinIdle"] = kry::Util::toString(index);
		item->object->hardproperties["entity"]["floor"] = "0";
		item->object->hardproperties["entity"]["directions"] = "1";
		item->object->hardproperties["entity"]["dimensions"] = "{ 1, 1 }";
		item->object->hardproperties["entity"]["direction"] = "0";
		item->object->hardproperties["entity"]["seeInFog"] = "false";
		item->object->hardproperties["entity"]["group"] = "0";
		item->object->hardproperties["entity"]["maxHeuristic"] = "0";
		for (auto& key : const_cast<kry::Media::Config&>(Assets::getRequiredKeys())[type].getKeyNames())
		{
			auto widgettype = const_cast<kry::Media::Config&>(Assets::getRequiredKeys())[type][key];
			if (widgettype.isEmpty())
				item->object->hardproperties[type][key] = "";
			else if (widgettype == "FLOAT")
				item->object->hardproperties[type][key] = "1.0";
			else if (widgettype == "INT")
				item->object->hardproperties[type][key] = "1";
			else if (widgettype == "ENTITY_GROUP_ARR")
				item->object->hardproperties[type][key] = "-1";
			else if (widgettype == "ENTITY_ARR")
				item->object->hardproperties[type][key] = "";
			else if (widgettype == "WEAPONS")
				item->object->hardproperties[type][key] = "melee";
			else if (widgettype == "BOOL")
				item->object->hardproperties[type][key] = "false";
			else if (widgettype == "ITEM_ID")
				item->object->hardproperties[type][key] = "-1";
			else if (widgettype == "ITEM_ARR")
				item->object->hardproperties[type][key] = "";
			else if (widgettype == "VEC_2_POS_ARR")
				item->object->hardproperties[type][key] = "";
			else if (widgettype == "VEC_2_POS")
				item->object->hardproperties[type][key] = "{ 100, 100 }";
		}
		item->path = asset->resource->path;
		return item;
	}
}

EntBrowserDialog::EntBrowserDialog(QWidget *parent) : QDialog(parent), ui(new Ui::EntBrowserDialog), selected(nullptr), lastresult(DialogResult::CANCEL), firstLoad(true)
{
	ui->setupUi(this);

	ui->lbIcons->setIconSize({100, 100});
	ui->lbIcons->setResizeMode(QListWidget::Adjust);
	ui->resProperties->horizontalHeader()->resizeSections(QHeaderView::Interactive);

	connect(ui->lbIcons, &QListWidget::itemClicked, [this](QListWidgetItem* item)
	{
		ObjectListItem* assetitem = dynamic_cast<ObjectListItem*>(item);

		ui->lResName->setText(kryToQString(assetitem->object->asset->properties["global"]["name"]));
		ui->lResImage->setPixmap(assetitem->icon().pixmap(ui->lResImage->size()));
		while (ui->resProperties->rowCount() > 0)
			ui->resProperties->removeRow(0);
		for (auto& key : assetitem->object->asset->properties["global"].getKeyNames())
		{
			int index = ui->resProperties->rowCount();
			ui->resProperties->insertRow(index);
			ui->resProperties->setItem(index, 0, new QTableWidgetItem(kryToQString(key)));
			ui->resProperties->item(index, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
			ui->resProperties->setItem(index, 1, new QTableWidgetItem(kryToQString(assetitem->object->asset->properties["global"][key])));
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
	connect(ui->bClose, &QPushButton::clicked, [this]()
	{
		lastresult = DialogResult::CANCEL;
		close();
	});
}

EntBrowserDialog::~EntBrowserDialog()
{
	delete ui;
}

DialogResult EntBrowserDialog::showDialog()
{
	if (firstLoad)
	{
		for (auto& asset : Assets::getEntities())
		{
			auto item = createListItem(asset.get(), asset->resource->path, kryToQString(asset->properties["global"]["name"]));
			ui->lbIcons->addItem(item);
		}
		firstLoad = false;
	}

	this->exec();
	return lastresult;
}
