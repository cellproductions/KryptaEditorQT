#include "AnimManagerDialog.h"
#include "ui_AnimManagerDialog.h"
#include "Resources.h"
#include "Utilities.h"
#include <QListWidgetItem>
#include <QDebug>

AnimManagerDialog::AnimManagerDialog(QWidget *parent) : CSDialog(parent), ui(new Ui::AnimManagerDialog)
{
	ui->setupUi(this);

	ui->lbImages->setMovement(QListView::Snap);
	ui->lbImages->setIconSize(QSize(150, 75));
	ui->lbImages->setGridSize(ui->lbImages->iconSize());

	// set up connections
	connect(ui->bDone, &QPushButton::clicked, [this](bool)
	{
		lastresult = DialogResult::OK;

		close();
	});
	void(QComboBox::* animsSignal)(int) = &QComboBox::currentIndexChanged;
	connect(ui->cbAnims, animsSignal, [this](int index)
	{
		setup(Resources::getAnimations()[index]);
	});
	connect(ui->lbImages, &QListWidget::currentItemChanged, [this](QListWidgetItem* current, QListWidgetItem*)
	{
		// what if there are no tabs?
		// also, what if the user reorders the images?
		ui->tabs->setCurrentIndex(ui->lbImages->currentRow());
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
	return lastresult;
}

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

void AnimManagerDialog::setup(const std::shared_ptr<Animation<> >& animation)
{
	auto name = animation->properties[""]["name"];

	ui->lbImages->clear();
	ui->lbImages->addItem(new QListWidgetItem(QIcon(animation->path), ""));
	ui->lbImages->addItem(new QListWidgetItem(QIcon(animation->path), ""));
	ui->lbImages->addItem(new QListWidgetItem(QIcon(animation->path), ""));
	ui->lbImages->addItem(new QListWidgetItem(QIcon(animation->path), ""));
	ui->lbImages->addItem(new QListWidgetItem(QIcon(animation->path), ""));
	ui->lbImages->addItem(new QListWidgetItem(QIcon(animation->path), ""));
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
