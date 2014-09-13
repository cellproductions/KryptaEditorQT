#ifndef OBJECTSETTINGSDIALOG_H
#define OBJECTSETTINGSDIALOG_H

#include "DialogResult.h"
#include <Media/ConfigFactory.h>
#include <QDialog>

namespace Ui
{
	class ObjectSettingsDialog;
}

class ObjectSettingsDialog : public QDialog
{
		Q_OBJECT

	public:
		explicit ObjectSettingsDialog(QWidget *parent = 0);
		~ObjectSettingsDialog();

		DialogResult showDialog(const kry::Util::String& title, const kry::Media::Config& config);
		kry::Media::Config getSettings() const;

	private:
		void updateTables();

		kry::Media::Config settings;
		Ui::ObjectSettingsDialog *ui;
		DialogResult lastresult;
};

#endif // OBJECTSETTINGSDIALOG_H
