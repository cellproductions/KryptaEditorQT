#include "AnimManagerDialog.h"
#include "ui_AnimManagerDialog.h"
#include "AnimationSetupWidget.h"
#include "ui_AnimationSetupWidget.h"
#include "Resources.h"
#include "Map.h"
#include "Utilities.h"
#include <QListWidgetItem>
#include <QFileDialog>
#include <QImageReader>
#include <QInputDialog>
#include <QTableWidget>
#include <QTabWidget>
#include <QDebug>

namespace
{
	inline QTableWidget* initTable(QTableWidget* table)
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

	inline AnimationSetupWidget* getSetup(Ui::AnimManagerDialog* ui, int tabindex) /** #TODO(note) qt screwed up and didnt promote the widgets properly. have to get them manually */
	{
		switch (tabindex)
		{
			case 0: return ui->northAnim;
			case 1: return ui->northeastAnim;
			case 2: return ui->eastAnim;
			case 3: return ui->southeastAnim;
			case 4: return ui->southAnim;
			case 5: return ui->southwestAnim;
			case 6: return ui->westAnim;
			case 7: return ui->northwestAnim;
		}
		return nullptr;
	}
}

AnimManagerDialog::AnimManagerDialog(QWidget *parent) : CSDialog(parent), ui(new Ui::AnimManagerDialog), firstLoad(true)
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
			return;
		
		saveTabStates(lastindex);
		AnimationSetupWidget* currsetup;
		for (int i = Animation<>::MAX_DIRECTION_COUNT - 1; i >= 0; --i)
		{
			currsetup = getSetup(ui, i);
			currsetup->release();
			currsetup->setup(Resources::getAnimations()[index], i);
		}
		lastindex = index;
	});
	connect(ui->dirTabs, &QTabWidget::currentChanged, [this](int index) /** #TODO(note) switching tabs doesnt update the pivotview (coz the other dirs have no sheetImage) */
	{

	});
	connect(ui->bDeleteAnim, &QPushButton::clicked, [this](bool)
	{
		int currentindex = ui->cbAnims->currentIndex();
		Animation<>* todelete = Resources::getAnimations()[currentindex].get();

		auto missing = Resources::getEditorTexture(EditorResource::MISSING_TILE).get();
		for (auto& asset : Assets::getAllTextureAssets())
			if (asset->resource == todelete)
				asset->resource = missing;
		
		for (auto layer : Map::getMap()->getLayers())
		{
			for (auto& tile : layer->tiles)
			{
				if (tile.sprite.texture == todelete->rawresource)
					tile.sprite.texture = missing->rawresource;
				for (auto object : tile.objects)
					if (object->sprite.texture == todelete->rawresource)
						object->sprite.texture = missing->rawresource;
			}
		}
		Resources::getAnimations().erase(Resources::getAnimations().begin() + currentindex);
		ui->cbAnims->clear();

		if (!Resources::getAnimations().empty())
		{
			unsigned index = 0;
			for (auto anim : Resources::getAnimations())
			{
				ui->cbAnims->addItem(QString::number(index) + ':' + kryToQString(anim->properties[0]["Skins"]["name"]));
				++index;
			}
			saveTabStates(lastindex);
			ui->cbAnims->setCurrentIndex(currentindex < ui->cbAnims->count() ? currentindex : 0);
		}
		if (ui->cbAnims->count() <= 0)
			ui->bDeleteAnim->setEnabled(false);
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
			size_t size = Resources::getAnimations().size();
			Resources::getAnimations().emplace_back(animation);
			ui->cbAnims->addItem(QString::number(ui->cbAnims->count()) + ':' + kryToQString(animation->properties[0]["Skins"]["name"]));
			
			saveTabStates(lastindex);
			ui->cbAnims->setCurrentIndex(ui->cbAnims->count() - 1);
			ui->bDeleteAnim->setEnabled(true);
		}
	});
}

AnimManagerDialog::~AnimManagerDialog()
{
	for (int i = 0; i < ui->dirTabs->count(); ++i)
		getSetup(ui, i)->release();
	delete ui;
}

DialogResult AnimManagerDialog::showDialog()
{
	ui->bDeleteAnim->setEnabled(false);
	lastindex = -1;
	if (firstLoad)
	{
		unsigned index = 0;
		for (auto anim : Resources::getAnimations())
			ui->cbAnims->addItem(QString::number(index++) + ':' + kryToQString(anim->properties[0]["Skins"]["name"])); /** #TODO(bug) for some reason there's a 0 on the end of each name */
		if (ui->cbAnims->count() > 0)
			for (int i = Animation<>::MAX_DIRECTION_COUNT - 1; i >= 0; --i)
				getSetup(ui, i)->setup(Resources::getAnimations()[0], i);
		firstLoad = false;
	}
	if (!Resources::getAnimations().empty())
		ui->dirTabs->setCurrentIndex(0);
	if (ui->cbAnims->count() > 0)
		ui->bDeleteAnim->setEnabled(true);

	exec();

	saveTabStates(lastindex);
	return lastresult;
}

void AnimManagerDialog::saveTabStates(int animindex)
{
	if (lastindex >= 0 && lastindex < ui->cbAnims->count())
		for (int i = Animation<>::MAX_DIRECTION_COUNT - 1; i >= 0; --i)
			getSetup(ui, i)->saveTo(Resources::getAnimations()[animindex], i);
}
