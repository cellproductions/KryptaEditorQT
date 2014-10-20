#include "EntBrowserDialog.h"
#include "ui_EntBrowserDialog.h"
#include "Resources.h"
#include "Utilities.h"

EntBrowserDialog::EntBrowserDialog(QWidget *parent) : QDialog(parent), ui(new Ui::EntBrowserDialog), selected(nullptr), lastresult(DialogResult::CANCEL)
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
	if (ui->lbIcons->count() <= 0)
		for (auto& asset : Assets::getEntities())
			ui->lbIcons->addItem(new AssetListItem(asset.get(), QIcon(asset->resource->path), kryToQString(asset->properties["global"]["name"])));

	this->exec();
	return lastresult;
}
