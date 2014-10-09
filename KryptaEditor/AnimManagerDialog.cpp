#include "AnimManagerDialog.h"
#include "ui_AnimManagerDialog.h"
#include "Resources.h"
#include "Utilities.h"
#include <QListWidgetItem>
#include <QFileDialog>
#include <QTimer>
#include <QDebug>

namespace
{
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
}

AnimManagerDialog::AnimManagerDialog(QWidget *parent) : CSDialog(parent), ui(new Ui::AnimManagerDialog),
	timer(new QTimer(this))
{
	ui->setupUi(this);

	ui->lbImages->setMovement(QListView::Snap);
	ui->lbImages->setIconSize(QSize(130, 65));
	ui->lbImages->setGridSize(ui->lbImages->iconSize());

	connect(timer, &QTimer::timeout, [this]()
	{
		int index = ui->lbImages->currentRow();
		ui->lbImages->setCurrentRow(index < 0 || index >= (ui->lbImages->count() - 1) ? 0 : index + 1);
	});

	connect(ui->bDone, &QPushButton::clicked, [this](bool)
	{
		lastresult = DialogResult::OK;

		close();
	});
	void(QComboBox::* animsSignal)(int) = &QComboBox::currentIndexChanged;
	connect(ui->cbAnims, animsSignal, [this](int index)
	{
		if (index < 0)
			return;
		setup(Resources::getAnimations()[index]);
	});
	connect(ui->lbImages, &QListWidget::currentItemChanged, [this](QListWidgetItem* current, QListWidgetItem*)
	{
		// what if the user reorders the images?
		if (!timer->isActive())
			ui->tabs->setCurrentIndex(ui->lbImages->currentRow());
		if (ui->lbImages->currentItem() != nullptr)
			ui->lDisplay->setPixmap(ui->lbImages->currentItem()->icon().pixmap(ui->lDisplay->size()));
	});
	//connect(ui->lbImages, &QListWidget::)
	connect(ui->bDeleteAnim, &QPushButton::clicked, [this](bool)
	{
		int currentindex = ui->cbAnims->currentIndex();
		Animation<>* todelete = Resources::getAnimations()[currentindex].get();

		for (auto& asset : Assets::getAllTextureAssets())
			if (asset->resource == todelete)
				asset->resource = Resources::getMissingTexture().get();

		Resources::getAnimations().erase(Resources::getAnimations().begin() + currentindex);
		ui->cbAnims->clear();

		if (!Resources::getAnimations().empty())
		{
			unsigned index = 0;
			for (auto& anim : Resources::getAnimations())
				ui->cbAnims->addItem(QString::number(index++) + ':' + kryToQString(anim->properties[""]["name"]));
			ui->cbAnims->setCurrentIndex(currentindex < ui->cbAnims->count() ? currentindex : 0);
		}
	});
	connect(ui->bAddAnim, &QPushButton::clicked, [this](bool)
	{
		// where im up to
		QString file = QFileDialog::getOpenFileName(this, "Create New Animation", QApplication::applicationDirPath() + "\\assets\\resources\\images", "Image files (*.jpg *.png *.bmp);;Spritesheet files (*.sheet)");
		if (!file.isNull())
		{
			Animation<>* animation = Animation<>::createDefaultAnimation(qToKString(file));
			Resources::getAnimations().emplace_back(animation);
			ui->cbAnims->addItem(QString::number(ui->cbAnims->count()) + ':' + kryToQString(animation->properties[""]["name"]));
			ui->cbAnims->setCurrentIndex(ui->cbAnims->count() - 1);
		}
	});
	connect(ui->bAdd, &QPushButton::clicked, [this](bool)
	{
		QString file = QFileDialog::getOpenFileName(this, "Create New Animation", QApplication::applicationDirPath() + "\\assets\\resources\\images", "Image files (*.jpg *.png *.bmp)");
		if (!file.isNull())
		{
			int currentindex = ui->cbAnims->currentIndex();
			Animation<>* currentanim = Resources::getAnimations()[currentindex].get();

			QString section = kryToQString(currentanim->properties[""]["name"] + ": ") + QString::number(ui->tabs->count());
			auto ksection = qToKString(section);
			kry::Media::Config framesettings;
			framesettings[ksection]["image"] = qToKString(file);
			framesettings[ksection]["pivot"] = "";
			framesettings[ksection]["dimensions"] = "";
			framesettings[ksection]["RGBA"] = "";
			framesettings[ksection]["rotation"] = "";
			framesettings[ksection]["UVposition"] = "";
			framesettings[ksection]["UVdimensions"] = "";
			framesettings[ksection]["linearFilter"] = "";
			framesettings[ksection]["sortDepth"] = "";
			framesettings[ksection]["sortPivotOffset"] = "";

			QTableWidget* table = initTable(new QTableWidget(ui->tabs));
			for (auto& key : framesettings[ksection].getKeyNames())
			{
				int index = table->rowCount();
				table->insertRow(index);
				table->setItem(index, 0, new QTableWidgetItem(kryToQString(key)));
				table->item(index, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
				table->setItem(index, 1, new QTableWidgetItem(kryToQString(framesettings[ksection][key])));

				currentanim->properties[ksection][key] = framesettings[ksection][key];
			}
			ui->tabs->addTab(table, section);

			ui->lbImages->addItem(new QListWidgetItem(QIcon(file), ""));
		}
	});
	connect(ui->bRemove, &QPushButton::clicked, [this](bool)
	{
		int index = ui->lbImages->currentRow();\
		QString toremove = ui->tabs->tabText(index);
		delete ui->lbImages->takeItem(index);
		ui->tabs->removeTab(index);

		index = ui->cbAnims->currentIndex();
		Resources::getAnimations()[index]->properties.removeSection(qToKString(toremove));
	});
	connect(ui->bAnimate, &QPushButton::clicked, [this](bool)
	{
		int row = 0;
		for (; row < ui->tSheetProps->rowCount() && ui->tSheetProps->item(row, 0)->text() != "fps"; ++row);
		auto strfps = qToKString(ui->tSheetProps->item(row, 1)->text());
		int fps = strfps.trim().isEmpty() ? 0 : kry::Util::toIntegral<int>(strfps);

		if (timer->isActive())
		{
			timer->stop();
			ui->bAnimate->setText("Animate");
			ui->tabs->setCurrentIndex(ui->lbImages->currentRow());
		}
		else
		{
			timer->start(1000 / fps);
			ui->bAnimate->setText("Stop Animating");
		}
	});
}

AnimManagerDialog::~AnimManagerDialog()
{
	delete ui;
}

DialogResult AnimManagerDialog::showDialog()
{
	if (ui->cbAnims->count() <= 0)
	{
		if (!Resources::getAnimations().empty())
		{
			unsigned index = 0;
			for (auto& anim : Resources::getAnimations())
				ui->cbAnims->addItem(QString::number(index++) + ':' + kryToQString(anim->properties[""]["name"]));
			setup(Resources::getAnimations()[0]);
		}
	}

	exec();
	timer->stop();
	ui->bAnimate->setText("Animate");
	return lastresult;
}

void AnimManagerDialog::setup(const std::shared_ptr<Animation<> >& animation)
{
	auto name = animation->properties[""]["name"];

	ui->lbImages->clear();
	ui->lbImages->addItem(new QListWidgetItem(QIcon(animation->path), ""));

	QTableWidget* table = ui->tSheetProps;
	while (table->rowCount() > 0)
		table->removeRow(0);
	for (auto& key : animation->properties[name].getKeyNames())
	{
		int index = table->rowCount();
		table->insertRow(index);
		table->setItem(index, 0, new QTableWidgetItem(kryToQString(key)));
		table->item(index, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		table->setItem(index, 1, new QTableWidgetItem(kryToQString(animation->properties[name][key])));
	}

	while (ui->tabs->count() > 0)
		delete ui->tabs->widget(0);
	for (auto& section : animation->properties.getSectionNames())
	{
		if (section.isEmpty() || section == name)
			continue;
		table = initTable(new QTableWidget(ui->tabs));
		for (auto& key : animation->properties[section].getKeyNames())
		{
			int index = table->rowCount();
			table->insertRow(index);
			table->setItem(index, 0, new QTableWidgetItem(kryToQString(key)));
			table->item(index, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
			table->setItem(index, 1, new QTableWidgetItem(kryToQString(animation->properties[section][key])));
		}
		ui->tabs->addTab(table, kryToQString(section));
	}
}
