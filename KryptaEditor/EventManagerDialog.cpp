#include "EventManagerDialog.h"
#include "ui_EventManagerDialog.h"

EventManagerDialog::EventManagerDialog(QWidget *parent) : QDialog(parent), ui(new Ui::EventManagerDialog)
{
	ui->setupUi(this);
}

EventManagerDialog::~EventManagerDialog()
{
	delete ui;
}
