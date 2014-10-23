#ifndef ITEMMANAGERDIALOG_H
#define ITEMMANAGERDIALOG_H

#include "CSDialog.h"

namespace Ui
{
	class ItemManagerDialog;
}

class ItemManagerDialog : public CSDialog
{
	Q_OBJECT

	public:
		ItemManagerDialog(QWidget *parent = 0);
		~ItemManagerDialog();

		DialogResult showDialog();

		static inline unsigned getIDCount();

	private:
		void setup();

		Ui::ItemManagerDialog* ui;

		static unsigned idcount;
};


unsigned ItemManagerDialog::getIDCount()
{
	return idcount;
}

#endif // ITEMMANAGERDIALOG_H
