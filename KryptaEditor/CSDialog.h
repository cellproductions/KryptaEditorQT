#ifndef CSDIALOG_H
#define CSDIALOG_H

#include "DialogResult.h"
#include <QDialog>

class CSDialog : public QDialog
{
		Q_OBJECT
	public:
		explicit CSDialog(QWidget *parent = 0);
		virtual ~CSDialog() {}

		virtual DialogResult showDialog();

	protected:
		DialogResult lastresult;
};

#endif // CSDIALOG_H
