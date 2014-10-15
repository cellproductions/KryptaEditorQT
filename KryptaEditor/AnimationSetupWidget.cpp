#include "AnimationSetupWidget.h"
#include "ui_AnimationSetupWidget.h"
#include "AnimManagerDialog.h"
#include "ui_AnimManagerDialog.h"
#include "Resources.h"
#include "Utilities.h"
#include <QListWidgetItem>
#include <QFileDialog>
#include <QTimer>
#include <QImageReader>
#include <QInputDialog>
#include <QComboBox>
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

AnimationSetupWidget::AnimationSetupWidget(QWidget *parent) : QWidget(parent), ui(new Ui::AnimationSetupWidget),
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

	connect(ui->lbImages, &QListWidget::currentItemChanged, [this](QListWidgetItem*, QListWidgetItem*)
	{
		// what if the user reorders the images?
		if (ui->lbImages->currentRow() < 0 || ui->lbImages->currentItem() == nullptr)
			return;
		if (!timer->isActive())
			ui->tabs->setCurrentIndex(ui->lbImages->currentRow());

		int row = 0;
		auto table = dynamic_cast<QTableWidget*>(ui->tabs->currentWidget());
		for (; row < table->rowCount() && table->item(row, 0)->text() != "image"; ++row);

		QString path;
		if (!table->item(row, 1)->text().isEmpty())
			path = table->item(row, 1)->text();
		else
		{
			row = 0;
			table = ui->tSheetProps;
			for (; row < table->rowCount() && table->item(row, 0)->text() != "sheetImage"; ++row);
			path = table->item(row, 1)->text();
		}

		QImageReader reader(path); /** #TODO(bug) if either of the image fields are empty, this could fail */
		QSize iconsize = reader.size();
		QSize scaled = iconsize.scaled(ui->gvPivot->width(), ui->gvPivot->height(), Qt::KeepAspectRatio);
		ui->gvPivot->setup(ui->lbImages->currentItem()->icon().pixmap(scaled), 0.5, 0.5);
	});
	connect(ui->bAdd, &QPushButton::clicked, [this](bool)
	{
		QStringList files = QFileDialog::getOpenFileNames(this, "Create New Frame", QApplication::applicationDirPath() + "\\assets\\resources\\images", "Image files (*.jpg *.png *.bmp)");
		if (!files.empty())
		{
			for (QString& file : files)
			{
				int currentindex = dynamic_cast<AnimManagerDialog*>(this->parent())->getUI()->cbAnims->currentIndex();
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
		}
	});
	connect(ui->bRemove, &QPushButton::clicked, [this](bool)
	{
		int index = ui->lbImages->currentRow();\
		QString toremove = ui->tabs->tabText(index);
		delete ui->lbImages->takeItem(index);
		ui->tabs->removeTab(index);

		index = dynamic_cast<AnimManagerDialog*>(this->parent())->getUI()->cbAnims->currentIndex();
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

AnimationSetupWidget::~AnimationSetupWidget()
{
	delete ui;
}

void AnimationSetupWidget::setup(std::shared_ptr<Animation<kry::Graphics::Texture> > animation)
{
	auto name = animation->properties[""]["name"];

	QTableWidget* table = ui->tSheetProps;
	while (table->rowCount() > 0)
		table->removeRow(0);
	for (auto& key : animation->properties[name].getKeyNames())
	{
		kryToQString(animation->properties[name][key]);
		int index = table->rowCount();
		table->insertRow(index);
		table->setItem(index, 0, new QTableWidgetItem(kryToQString(key)));
		table->item(index, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		table->setItem(index, 1, new QTableWidgetItem(kryToQString(animation->properties[name][key])));
	}

	ui->lbImages->clear();
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
		ui->lbImages->addItem(new QListWidgetItem(QIcon(animation->properties[section]["image"].isEmpty() ? animation->path : kryToQString(animation->properties[section]["image"])), ""));
	}
}

void AnimationSetupWidget::release()
{
	timer->stop();
	ui->bAnimate->setText("Animate");
	ui->gvPivot->reset();
}
