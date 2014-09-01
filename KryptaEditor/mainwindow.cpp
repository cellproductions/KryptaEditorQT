#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "prjsetupdialog.h"
#include "ui_prjsetupdialog.h"
#include "envbrowserdialog.h"
#include "configuration.h"
#include "map.h"
#include "assets.h"
#include <QToolButton>
#include <QFileDialog>
#include <QMessageBox>

static const QString mainTitle("Krypta Map Editor");

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), prjsetupDialog(new PrjSetupDialog(this)),
    envbrowseDialog(new EnvBrowserDialog(this))
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
    connect(ui->bBrowseEnv, &QPushButton::clicked, [this]()
    {
        if (envbrowseDialog->showDialog() == DialogResult::OK)
        {
            ui->lEnvName->setText(envbrowseDialog->getSelectedAssetItem()->text());
            ui->lEnv->setPixmap(envbrowseDialog->getSelectedAssetItem()->icon().pixmap(ui->lEnv->size()));
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    try
    {
        if (Configuration::loadFromFile("editor.cfg")["editor"]["maximised"] == "true")
            this->showMaximized();
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
            auto map = Map::createMap(prjsetupDialog->getUI()->tbMapName->text(), { Assets::getTiles()[0].get(), kry::Graphics::Sprite() }, prjsetupDialog->getUI()->lbLayers);
            ui->layerProperties->setItem(0, 1, new QTableWidgetItem(map->getCurrentLayer()->description));
            ui->layerProperties->setItem(1, 1, new QTableWidgetItem(QString::number(map->getCurrentLayer()->size[0])));
            ui->layerProperties->setItem(2, 1, new QTableWidgetItem(QString::number(map->getCurrentLayer()->size[1])));
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
