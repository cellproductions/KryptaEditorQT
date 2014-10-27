#ifndef ENTBROWSERDIALOG_H
#define ENTBROWSERDIALOG_H

#include "ObjectListItem.h"
#include "DialogResult.h"
#include <QDialog>

namespace Ui
{
	class EntBrowserDialog;
}

class EntBrowserDialog : public QDialog
{
	Q_OBJECT

	public:
		explicit EntBrowserDialog(QWidget *parent = 0);
		~EntBrowserDialog();

		DialogResult showDialog();
		inline ObjectListItem* getSelectedItem();

	private:
		Ui::EntBrowserDialog *ui;
		ObjectListItem* selected;
		DialogResult lastresult;
		bool firstLoad;
};


ObjectListItem* EntBrowserDialog::getSelectedItem()
{
	return selected;
}

#endif // ENTBROWSERDIALOG_H
