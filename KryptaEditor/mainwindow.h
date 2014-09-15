#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui
{
    class MainWindow;
}

class PrjSetupDialog;
class EntBrowserDialog;
class EnvBrowserDialog;
class LayerBrowserDialog;
class ConfigDialog;
class PrjSettingsDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();
        void init();

        inline Ui::MainWindow* getUI() const;

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
        PrjSetupDialog* prjsetupDialog;
		EntBrowserDialog* entbrowseDialog;
        EnvBrowserDialog* envbrowseDialog;
		LayerBrowserDialog* layerbrowseDialog;
		ConfigDialog* configDialog;
		PrjSettingsDialog* prjsettingsDialog;
		bool saved;
};


Ui::MainWindow* MainWindow::getUI() const
{
    return ui;
}

#endif // MAINWINDOW_H
