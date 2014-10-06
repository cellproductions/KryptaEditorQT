#ifndef EVENTEDITDIALOG_H
#define EVENTEDITDIALOG_H

#include "CSDialog.h"

class EventButtonItem;

namespace Ui
{
	class EventEditDialog;
}

class EventEditDialog : public CSDialog
{
		Q_OBJECT

	public:
		explicit EventEditDialog(QWidget *parent = 0);
		~EventEditDialog();

		DialogResult showDialog(EventButtonItem* item);

	private:
		void updateTree(EventButtonItem* item);

		Ui::EventEditDialog *ui;
};

#endif // EVENTEDITDIALOG_H
