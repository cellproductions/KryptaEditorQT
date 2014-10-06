#ifndef EVENTMANAGERDIALOG_H
#define EVENTMANAGERDIALOG_H

#include <QDialog>

namespace Ui {
	class EventManagerDialog;
}

class EventManagerDialog : public QDialog
{
		Q_OBJECT

	public:
		explicit EventManagerDialog(QWidget *parent = 0);
		~EventManagerDialog();

	private:
		Ui::EventManagerDialog *ui;
};

#endif // EVENTMANAGERDIALOG_H
