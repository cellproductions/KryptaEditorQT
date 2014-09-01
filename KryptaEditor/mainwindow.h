#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui
{
    class MainWindow;
}

class PrjSetupDialog;
class EnvBrowserDialog;

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
        void onExitTrigger();

    private:
        Ui::MainWindow *ui;
        PrjSetupDialog* prjsetupDialog;
        EnvBrowserDialog* envbrowseDialog;
};


Ui::MainWindow* MainWindow::getUI() const
{
    return ui;
}

#endif // MAINWINDOW_H
