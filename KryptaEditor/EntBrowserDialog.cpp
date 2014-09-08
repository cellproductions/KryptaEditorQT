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
	ui->resProperties->item(0, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	ui->resProperties->item(1, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	ui->resProperties->item(2, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	ui->resProperties->item(3, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	connect(ui->lbIcons, &QListWidget::itemClicked, [this](QListWidgetItem* item)
	{
		AssetListItem* assetitem = dynamic_cast<AssetListItem*>(item);

		ui->lResName->setText(kryToQString(assetitem->asset->properties["global"]["name"]));
		ui->lResImage->setPixmap(assetitem->icon().pixmap(ui->lResImage->size()));
		ui->resProperties->setItem(0, 1, new QTableWidgetItem(assetitem->asset->path));
		ui->resProperties->setItem(1, 1, new QTableWidgetItem(kryToQString(assetitem->asset->properties["global"]["type"])));
		ui->resProperties->setItem(2, 1, new QTableWidgetItem(QString::number(assetitem->asset->resource->rawresource->getDimensions()[0])));
		ui->resProperties->setItem(3, 1, new QTableWidgetItem(QString::number(assetitem->asset->resource->rawresource->getDimensions()[1])));
		ui->resProperties->item(0, 1)->setFlags(0);
		ui->resProperties->item(1, 1)->setFlags(0);
		ui->resProperties->item(2, 1)->setFlags(0);
		ui->resProperties->item(3, 1)->setFlags(0);

		selected = assetitem;
	});
	connect(ui->bSelect, &QPushButton::clicked, [this]()
	{
		lastresult = DialogResult::OK;
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
	{
		for (auto& asset : Assets::getEntities())
			ui->lbIcons->addItem(new AssetListItem(asset.get(), QIcon(asset->resource->path), kryToQString(asset->properties["global"]["name"])));
	}

	this->exec();
	return lastresult;
}
