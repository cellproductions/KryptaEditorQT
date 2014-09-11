#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "PrjSetupDialog.h"
#include "ui_prjsetupdialog.h"
#include "EntBrowserDialog.h"
#include "EnvBrowserDialog.h"
#include "LayerBrowserDialog.h"
#include "ConfigDialog.h"
#include "Configuration.h"
#include "Map.h"
#include "Assets.h"
#include "Tool.h"
#include <QGraphicsColorizeEffect>
#include <QToolButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

static const QString mainTitle("Krypta Map Editor");

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), prjsetupDialog(new PrjSetupDialog(this)),
	entbrowseDialog(new EntBrowserDialog(this)), envbrowseDialog(new EnvBrowserDialog(this)), layerbrowseDialog(new LayerBrowserDialog(this)), configDialog(new ConfigDialog(this))
{
    ui->setupUi(this);
/*
    QMenu* menu = new QMenu("FILE", ui->toolMain);
    ui->toolMain->addAction(menu->menuAction());

    auto exit = new QAction("Exit", menu);
    menu->addAction(exit);

    connect(exit, SIGNAL(triggered()), this, SLOT(onExitTrigger()));*/

    ui->layerProperties->horizontalHeader()->resizeSections(QHeaderView::Interactive);
    ui->layerProperties->item(0, 0)->setFlags(ui->layerProperties->item(0, 0)->flags() ^ Qt::ItemIsEditable);
    ui->layerProperties->item(1, 0)->setFlags(ui->layerProperties->item(1, 0)->flags() ^ Qt::ItemIsEditable);
    ui->layerProperties->item(2, 0)->setFlags(ui->layerProperties->item(2, 0)->flags() ^ Qt::ItemIsEditable);
    connect(ui->layerProperties, &QTableWidget::cellChanged, [this](int row, int column)
    {
        Map::getMap()->getCurrentLayer()->description = ui->layerProperties->item(row, column)->text();
    });

    connect(ui->miFileNew, SIGNAL(triggered()), this, SLOT(onNewTrigger()));
    connect(ui->miFileOpen, SIGNAL(triggered()), this, SLOT(onOpenTrigger()));
    connect(ui->miFileExit, SIGNAL(triggered()), this, SLOT(onExitTrigger()));
	connect(ui->miPreferences, &QAction::triggered, [this]()
	{
		if (configDialog->showDialog() == DialogResult::OK)
			Configuration::updateConfig(configDialog->getConfig());
	});
	connect(ui->bBrowseEntities, &QPushButton::clicked, [this]()
	{
		if (entbrowseDialog->showDialog() == DialogResult::OK)
		{
			ui->lEntityName->setText(entbrowseDialog->getSelectedAssetItem()->text());
			ui->lEntity->setPixmap(entbrowseDialog->getSelectedAssetItem()->icon().pixmap(ui->lEntity->size()));

			ui->lEntity->clicked();
		}
	});
	connect(ui->lEntity, &ClickableLabel::clicked, [this]()
	{
		QGraphicsColorizeEffect* effect = new QGraphicsColorizeEffect(ui->lEntity);
		effect->setColor(QColor(255, 255, 0));
		effect->setStrength(0.5f);
		ui->lEntity->setGraphicsEffect(effect);
		ui->lEnv->setGraphicsEffect(nullptr);

		if (Tool<>::getTool()->getType() == ToolType::PAINT)
			Tool<PaintData>::getTool()->getData().assetitem = entbrowseDialog->getSelectedAssetItem();
	});
    connect(ui->bBrowseEnv, &QPushButton::clicked, [this]()
    {
        if (envbrowseDialog->showDialog() == DialogResult::OK)
        {
            ui->lEnvName->setText(envbrowseDialog->getSelectedAssetItem()->text());
            ui->lEnv->setPixmap(envbrowseDialog->getSelectedAssetItem()->icon().pixmap(ui->lEnv->size()));

			ui->lEnv->clicked();
        }
    });
	connect(ui->lEnv, &ClickableLabel::clicked, [this]()
	{
		QGraphicsColorizeEffect* effect = new QGraphicsColorizeEffect(ui->lEnv);
		effect->setColor(QColor(255, 255, 0));
		effect->setStrength(0.5f);
		ui->lEnv->setGraphicsEffect(effect);
		ui->lEntity->setGraphicsEffect(nullptr);

		if (Tool<>::getTool()->getType() == ToolType::PAINT)
			Tool<PaintData>::getTool()->getData().assetitem = envbrowseDialog->getSelectedAssetItem();
	});
	connect(ui->bLayerMan, &QPushButton::clicked, [this]()
	{
		if (layerbrowseDialog->showDialog() == DialogResult::OK)
		{

		}
	});

	void(QComboBox:: *cbLayersSignal)(int) = &QComboBox::currentIndexChanged;
	connect(ui->cbLayers, cbLayersSignal, [this](int index)
	{
		if (Map::getMap())
			if (index < 0 || index >= Map::getMap()->getLayers().size())
				return;
		Map::getMap()->setCurrentLayer(index);
		ui->glWidget->resetCamera();
		ui->glWidget->updateCanvas();
	});

	connect(ui->bPointer, &QToolButton::clicked, [this]()
	{
		//for (QObject* child : ui->toolMain->children())
		//	dynamic_cast<QWidget*>(child)->setVisible(false);

		Tool<>::switchTool(ToolType::POINTER);
	});
	connect(ui->bPaint, &QToolButton::clicked, [this]()
	{
		//for (QObject* child : ui->toolMain->children())
		//	dynamic_cast<QWidget*>(child)->setVisible(false);

		static QLabel* lSpin = nullptr;
		static QSpinBox* spin = nullptr;
		if (lSpin == nullptr)
		{
			lSpin = new QLabel("Brush size: ", ui->toolMain);
			spin = new QSpinBox(ui->toolMain);
			spin->setMaximum(10);
			spin->setMinimum(1);
			spin->setSuffix(" tiles");
			ui->toolMain->addWidget(lSpin);
			ui->toolMain->addWidget(spin);
		}

		Tool<>::switchTool(ToolType::PAINT);
		PaintData data;
		data.size = spin->value();
		data.assetitem = envbrowseDialog->getSelectedAssetItem();
		Tool<PaintData>::getTool()->setData(data);
	});
}

MainWindow::~MainWindow()
{
	Configuration::saveToFile("editor.cfg");

    delete ui;
}

void MainWindow::init()
{
    try
	{
        if (Configuration::loadFromFile("editor.cfg")["editor"]["maximised"] == "true")
            this->showMaximized();
		configDialog->setConfigData(Configuration::getConfig());
    }
    catch (const kry::Util::Exception&)
    {
        QMessageBox::information(this, "Config Error", "Failed to open config file!", QMessageBox::Ok);
    }
}

void MainWindow::onNewTrigger()
{
    if (prjsetupDialog->showDialog() == DialogResult::OK)
    {
        try
        {
            this->setWindowTitle("Krypta Map Editor - " + prjsetupDialog->getUI()->tbPrjName->text());

            if (!Assets::isLoaded())
                Assets::loadAssets("assets");

            if (Map::getMap())
                Map::getMap()->resetMap();
			Tile defaulttile;
			defaulttile.asset = Assets::getTiles()[0].get();
			auto map = Map::createMap(prjsetupDialog->getUI()->tbMapName->text(), defaulttile,
									  prjsetupDialog->getUI()->lbLayers);
			ui->cbLayers->clear();
			for (auto& layer : map->getLayers())
				ui->cbLayers->addItem(layer->description);
            ui->layerProperties->setItem(0, 1, new QTableWidgetItem(map->getCurrentLayer()->description));
            ui->layerProperties->setItem(1, 1, new QTableWidgetItem(QString::number(map->getCurrentLayer()->size[0])));
            ui->layerProperties->setItem(2, 1, new QTableWidgetItem(QString::number(map->getCurrentLayer()->size[1])));
			ui->glWidget->resetCamera();
            ui->glWidget->updateCanvas();
        }
        catch (const kry::Util::Exception& e)
        {
            QMessageBox::information(this, "Map Creation Error", e.what(), QMessageBox::Ok);
        }
    }
}

void MainWindow::onOpenTrigger()
{
    QString file = QFileDialog::getOpenFileName(this, "Open Project", QApplication::applicationDirPath(), "Project files (*.kryprj)");
    int index = file.lastIndexOf('/') + 1;
    file = file.mid(index, file.lastIndexOf('.') - index);
    this->setWindowTitle(mainTitle + " - " + file);
}

void MainWindow::onExitTrigger()
{
    close();
}
