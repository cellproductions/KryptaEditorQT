#ifndef SAVEDIALOG_H
#define SAVEDIALOG_H

#include "DialogResult.h"
#include <QDialog>

namespace Ui
{
	class SaveDialog;
}

class SaveDialog : public QDialog
{
		Q_OBJECT

	public:
		explicit SaveDialog(QWidget *parent = 0);
		~SaveDialog();

		DialogResult showDialog();

	private:
		Ui::SaveDialog *ui;
		DialogResult lastresult;
};

#endif // SAVEDIALOG_H
