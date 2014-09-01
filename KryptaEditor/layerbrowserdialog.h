#ifndef LAYERBROWSERDIALOG_H
#define LAYERBROWSERDIALOG_H

#include <QDialog>

namespace Ui {
class LayerBrowserDialog;
}

class LayerBrowserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LayerBrowserDialog(QWidget *parent = 0);
    ~LayerBrowserDialog();

private:
    Ui::LayerBrowserDialog *ui;
};

#endif // LAYERBROWSERDIALOG_H
