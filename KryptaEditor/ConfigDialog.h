#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include "DialogResult.h"
#include <Media/ConfigFactory.h>
#include <QDialog>

namespace Ui
{
	class ConfigDialog;
}

class ConfigDialog : public QDialog
{
		Q_OBJECT

	public:
		explicit ConfigDialog(QWidget *parent = 0);
		~ConfigDialog();

		DialogResult showDialog();
		void setConfigData(const kry::Media::Config& data);
		kry::Media::Config getConfig() const;

	private:
		kry::Media::Config settings;
		Ui::ConfigDialog *ui;
		DialogResult lastresult;

		void updateTable();
};

#endif // CONFIGDIALOG_H
