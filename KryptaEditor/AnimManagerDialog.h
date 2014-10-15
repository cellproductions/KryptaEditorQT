#ifndef ANIMMANAGERDIALOG_H
#define ANIMMANAGERDIALOG_H

#include "CSDialog.h"
#include "Graphics/Texture.h"
#include <memory>

template <typename ResType>
struct Animation;

class QTimer;

namespace Ui
{
	class AnimManagerDialog;
}

class AnimManagerDialog : public CSDialog
{
		Q_OBJECT

	public:
		explicit AnimManagerDialog(QWidget *parent = 0);
		~AnimManagerDialog();

		DialogResult showDialog();
		inline Ui::AnimManagerDialog* getUI();

	private:
		Ui::AnimManagerDialog* ui;
};


Ui::AnimManagerDialog* AnimManagerDialog::getUI()
{
	return ui;
}

#endif // ANIMMANAGERDIALOG_H
