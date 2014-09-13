#include "SaveDialog.h"
#include "ui_SaveDialog.h"

SaveDialog::SaveDialog(QWidget *parent) : QDialog(parent), ui(new Ui::SaveDialog), lastresult(DialogResult::CANCEL)
{
	ui->setupUi(this);

	connect(ui->bYes, &QPushButton::clicked, [this]()
	{
		lastresult = DialogResult::YES;
		this->close();
	});
	connect(ui->bNo, &QPushButton::clicked, [this]()
	{
		lastresult = DialogResult::NO;
		this->close();
	});
	connect(ui->bCancel, &QPushButton::clicked, [this]()
	{
		lastresult = DialogResult::CANCEL;
		this->close();
	});
}

SaveDialog::~SaveDialog()
{
	delete ui;
}

DialogResult SaveDialog::showDialog()
{
	this->exec();
	return lastresult;
}
