#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Graphics\Texture.h>
#include <QMainWindow>

class QLabel;
class PrjSetupDialog;
class EntBrowserDialog;
class EnvBrowserDialog;
class LayerBrowserDialog;
class ConfigDialog;
class PrjSettingsDialog;
class AnimManagerDialog;
class AudioManagerDialog;
class ItemManagerDialog;

struct Object;

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();
        void init();
		void clearAndSwitchTool();

        inline Ui::MainWindow* getUI() const;
		inline QLabel* getStatusMain() const;
		inline QLabel* getStatusPos() const;
		inline QLabel* getStatusTile() const;
		inline EnvBrowserDialog* getEnvBrowser() const;

		struct ToolBarItem
		{
			QAction* action;
			QWidget* widget;
		};

		inline std::vector<ToolBarItem>& getToolbarItems();

    public slots:
        void onNewTrigger();
        void onOpenTrigger();
		void onSaveTrigger();
		void onSaveAsTrigger();
		void onExportTrigger();
        void onExitTrigger();

    private:
		void closeEvent(QCloseEvent*);

        Ui::MainWindow *ui;
		std::vector<ToolBarItem> toolbarItems;
		QLabel* statusMain;
		QLabel* statusPos;
		QLabel* statusTile;
        PrjSetupDialog* prjsetupDialog;
		EntBrowserDialog* entbrowseDialog;
        EnvBrowserDialog* envbrowseDialog;
		std::shared_ptr<Object> prevObject;
		LayerBrowserDialog* layerbrowseDialog;
		ConfigDialog* configDialog;
		PrjSettingsDialog* prjsettingsDialog;
		AnimManagerDialog* animmanagerDialog;
		AudioManagerDialog* audiomanagerDialog;
		ItemManagerDialog* itemmanagerDialog;
		bool saved;
};


Ui::MainWindow* MainWindow::getUI() const
{
    return ui;
}

QLabel* MainWindow::getStatusMain() const
{
	return statusMain;
}

QLabel* MainWindow::getStatusPos() const
{
	return statusPos;
}

QLabel* MainWindow::getStatusTile() const
{
	return statusTile;
}

EnvBrowserDialog* MainWindow::getEnvBrowser() const
{
	return envbrowseDialog;
}

std::vector<MainWindow::ToolBarItem>& MainWindow::getToolbarItems()
{
	return toolbarItems;
}

#endif // MAINWINDOW_H
