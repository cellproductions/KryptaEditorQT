#include "EnvBrowserDialog.h"
#include "ui_envbrowserdialog.h"
#include "Resources.h"
#include "Utilities.h"
#include <QInputDialog>

EnvBrowserDialog::EnvBrowserDialog(QWidget *parent) : QDialog(parent), ui(new Ui::EnvBrowserDialog), selected(nullptr), lastresult(DialogResult::CANCEL)
{
    ui->setupUi(this);

    ui->lbIcons->setIconSize({100, 100});
    ui->lbIcons->setResizeMode(QListWidget::Adjust);
    ui->resProperties->horizontalHeader()->resizeSections(QHeaderView::Interactive);

    connect(ui->lbIcons, &QListWidget::itemClicked, [this](QListWidgetItem* item)
    {
        AssetListItem* assetitem = dynamic_cast<AssetListItem*>(item);

        ui->lResName->setText(kryToQString(assetitem->asset->properties["global"]["name"]));
        ui->lResImage->setPixmap(assetitem->icon().pixmap(ui->lResImage->size()));
		while (ui->resProperties->rowCount() > 0)
			ui->resProperties->removeRow(0);
		for (auto& key : assetitem->asset->properties["global"].getKeyNames())
		{
			int index = ui->resProperties->rowCount();
			ui->resProperties->insertRow(index);
			ui->resProperties->setItem(index, 0, new QTableWidgetItem(kryToQString(key)));
			ui->resProperties->item(index, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
			ui->resProperties->setItem(index, 1, new QTableWidgetItem(kryToQString(assetitem->asset->properties["global"][key])));
			ui->resProperties->item(index, 1)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		}
    });
    connect(ui->bSelect, &QPushButton::clicked, [this]()
    {
		selected = dynamic_cast<AssetListItem*>(ui->lbIcons->selectedItems().empty() ? nullptr :
												ui->lbIcons->selectedItems()[0]);
		lastresult = ui->lbIcons->selectedItems().size() <= 0 ? DialogResult::CANCEL : DialogResult::OK;
        close();
    });
	connect(ui->bAdd, &QPushButton::clicked, [this]()
	{
		/** #TODO(incomplete) not entirely sure what to do with this */
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

		Asset<kry::Graphics::Texture>* asset = new Asset<kry::Graphics::Texture>;
		asset->path = "";
		asset->type = AssetType::TILE;
		asset->properties["global"]["name"] = qToKString(name);
		asset->properties["global"]["resource"] = qToKString(animation->path);
		asset->properties["global"]["type"] = type == "wall" ? kry::Util::String("ENTITY") : kry::Util::String("TILE");
		asset->properties["global"]["hardtype"] = qToKString(type);
		asset->properties["global"]["gridsnap"] = "true";
		asset->properties["global"]["relativex"] = "0.5";
		asset->properties["global"]["relativey"] = "0.5";
		asset->resource = animation.get();

		const_cast<std::vector<std::shared_ptr<Asset<kry::Graphics::Texture>>>&>(Assets::getTiles()).emplace_back(asset);
		ui->lbIcons->addItem(new AssetListItem(asset, QIcon(asset->resource->path), kryToQString(asset->properties["global"]["name"])));
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
    if (ui->lbIcons->count() <= 0)
        for (auto& asset : Assets::getTiles())
            ui->lbIcons->addItem(new AssetListItem(asset.get(), QIcon(asset->resource->path), kryToQString(asset->properties["global"]["name"])));

    this->exec();
    return lastresult;
}
