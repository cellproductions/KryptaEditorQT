#include "CSDialog.h"

CSDialog::CSDialog(QWidget *parent) : QDialog(parent), lastresult(DialogResult::CANCEL)
{
}

DialogResult CSDialog::showDialog()
{
	this->exec();
	return lastresult;
}
