#ifndef OBJECTSETTINGSDIALOG_H
#define OBJECTSETTINGSDIALOG_H

#include "DialogResult.h"
#include <Media/ConfigFactory.h>
#include <QDialog>

struct Object;

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

		DialogResult showDialog(const kry::Util::String& title, Object* object);
		kry::Media::Config getSettings() const;
		kry::Media::Config getHardTypeSettings() const;

	private:
		void updateTables(Object* object);

		kry::Media::Config settings;
		kry::Media::Config hardtypesettings;
		Ui::ObjectSettingsDialog *ui;
		DialogResult lastresult;
};

#endif // OBJECTSETTINGSDIALOG_H
