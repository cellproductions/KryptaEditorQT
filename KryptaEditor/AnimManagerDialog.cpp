#include "AnimManagerDialog.h"
#include "ui_AnimManagerDialog.h"
#include "AnimationSetupWidget.h"
#include "Resources.h"
#include "Utilities.h"
#include <QListWidgetItem>
#include <QFileDialog>
#include <QTimer>
#include <QImageReader>
#include <QInputDialog>
#include <QTableWidget>
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

AnimManagerDialog::AnimManagerDialog(QWidget *parent) : CSDialog(parent), ui(new Ui::AnimManagerDialog)
{
	ui->setupUi(this);

	connect(ui->bDone, &QPushButton::clicked, [this](bool)
	{
		lastresult = DialogResult::OK;

		close();
	});
	void(QComboBox::* animsSignal)(int) = &QComboBox::currentIndexChanged;
	connect(ui->cbAnims, animsSignal, [this](int index)
	{
		if (index < 0)
			return; /** #TODO(incomplete) state is not saved per direction. also, might be setting up for the wrong direction */
		dynamic_cast<AnimationSetupWidget*>(ui->dirTabs->currentWidget())->setup(Resources::getAnimations()[index]);
	});
	connect(ui->bDeleteAnim, &QPushButton::clicked, [this](bool)
	{
		int currentindex = ui->cbAnims->currentIndex();
		Animation<>* todelete = Resources::getAnimations()[currentindex].get();

		for (auto& asset : Assets::getAllTextureAssets())
			if (asset->resource == todelete)
				asset->resource = Resources::getEditorTexture(EditorResource::MISSING_TILE).get();

		Resources::getAnimations().erase(Resources::getAnimations().begin() + currentindex);
		ui->cbAnims->clear();

		if (!Resources::getAnimations().empty())
		{
			unsigned index = 0;
			for (auto anim : Resources::getAnimations())
				ui->cbAnims->addItem(QString::number(index++) + ':' + kryToQString(anim->properties[""]["name"]));
			ui->cbAnims->setCurrentIndex(currentindex < ui->cbAnims->count() ? currentindex : 0);
		}
	});
	connect(ui->bAddAnim, &QPushButton::clicked, [this](bool)
	{
		QString file = QFileDialog::getOpenFileName(this, "Create New Animation", QApplication::applicationDirPath() + "\\assets\\resources\\images", "Image files (*.jpg *.png *.bmp);;Spritesheet files (*.sheet)");
		if (!file.isNull())
		{
			QInputDialog dialog(this);
			dialog.setWindowTitle("Create New Animation");
			dialog.setLabelText("Enter a name for the animation:");
			dialog.exec();
			QString result = dialog.textValue();
			if (result.trimmed().isEmpty() || dialog.result() == QDialog::Rejected)
				return;
			QString test = result;
			unsigned testcount = 0;
			while (ui->cbAnims->findText(test) != -1) // check if the name is taken. if it is, add a number to it
				test = result + QString::number(testcount++);
			result = test;

			Animation<>* animation = Animation<>::createDefaultAnimation(qToKString(file), qToKString(result));
			Resources::getAnimations().emplace_back(animation);
			ui->cbAnims->addItem(QString::number(ui->cbAnims->count()) + ':' + kryToQString(animation->properties[""]["name"]));
			ui->cbAnims->setCurrentIndex(ui->cbAnims->count() - 1);
		}
	});
}

AnimManagerDialog::~AnimManagerDialog()
{
	delete ui;
}

DialogResult AnimManagerDialog::showDialog() /** #TODO(incomplete) add directions to dialog */
{
	if (ui->cbAnims->count() <= 0)
	{
		if (!Resources::getAnimations().empty())
		{
			unsigned index = 0;
			for (auto anim : Resources::getAnimations())
				ui->cbAnims->addItem(QString::number(index++) + ':' + kryToQString(anim->properties[""]["name"]));
			
			//setup(Resources::getAnimations()[0]);
		}
	}

	exec();

	return lastresult;
}
