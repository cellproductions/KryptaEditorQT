#include "layerbrowserdialog.h"
#include "ui_layerbrowserdialog.h"

LayerBrowserDialog::LayerBrowserDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LayerBrowserDialog)
{
    ui->setupUi(this);
}

LayerBrowserDialog::~LayerBrowserDialog()
{
    delete ui;
}
