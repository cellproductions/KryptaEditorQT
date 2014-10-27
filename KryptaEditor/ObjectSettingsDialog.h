#ifndef OBJECTSETTINGSDIALOG_H
#define OBJECTSETTINGSDIALOG_H

#include "CSDialog.h"
#include <Media/ConfigFactory.h>
#include <Utilities\BasicString.h>
#include <set>

struct Object;

namespace Ui
{
	class ObjectSettingsDialog;
}

struct ObjectProperties
{
	kry::Util::String type;
	mutable kry::Media::Config settings;
	mutable kry::Media::Config hardtypesettings;
};

inline bool operator<(const ObjectProperties& left, const ObjectProperties& right);

class ObjectSettingsDialog : public CSDialog
{
	public:
		explicit ObjectSettingsDialog(QWidget *parent = 0);
		~ObjectSettingsDialog();

		DialogResult showDialog(const kry::Util::String& title, std::set<Object*>& objects, bool group = false);
		inline std::set<ObjectProperties> getAllProperties();

	private:
		void updateTables(std::set<Object*>& objects);

		std::set<ObjectProperties> objectprops;
		Ui::ObjectSettingsDialog *ui;
		bool ingroup;
};


bool operator<(const ObjectProperties& left, const ObjectProperties& right)
{
	return left.type < right.type;
}

std::set<ObjectProperties> ObjectSettingsDialog::getAllProperties()
{
	return objectprops;
}

#endif // OBJECTSETTINGSDIALOG_H
