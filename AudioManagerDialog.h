#ifndef AUDIOMANAGERDIALOG_H
#define AUDIOMANAGERDIALOG_H

#include "CSDialog.h"

namespace Ui
{
	class AudioManagerDialog;
}

class AudioManagerDialog : public CSDialog
{
	Q_OBJECT

	public:
		AudioManagerDialog(QWidget *parent = 0);
		~AudioManagerDialog();

		DialogResult showDialog();

	private:
		void setupDropdown();

		Ui::AudioManagerDialog* ui;
		int lastindex;
};

#endif // AUDIOMANAGERDIALOG_H
