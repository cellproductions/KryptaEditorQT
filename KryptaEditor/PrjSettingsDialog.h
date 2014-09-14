#ifndef PRJSETTINGSDIALOG_H
#define PRJSETTINGSDIALOG_H

#include "CSDialog.h"
#include <Utilities/String.h>
#include <Media/ConfigFactory.h>

namespace Ui
{
	class PrjSettingsDialog;
}

class PrjSettingsDialog : public CSDialog
{
		Q_OBJECT

	public:
		explicit PrjSettingsDialog(QWidget *parent = 0);
		~PrjSettingsDialog();

		DialogResult showDialog();

		void resetSettings();
		inline kry::Media::Config getAllSettings() const;

	private:
		void setTableData();

		kry::Media::Config settings;
		Ui::PrjSettingsDialog *ui;
};


kry::Media::Config PrjSettingsDialog::getAllSettings() const
{
	return settings;
}

#endif // PRJSETTINGSDIALOG_H
