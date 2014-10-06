#ifndef ANIMMANAGERDIALOG_H
#define ANIMMANAGERDIALOG_H

#include "CSDialog.h"
#include "Graphics/Texture.h"
#include <memory>

template <typename ResType = kry::Graphics::Texture>
struct Animation;

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

	private:
		void setup(const std::shared_ptr<Animation<> >& animation);

		Ui::AnimManagerDialog *ui;
};

#endif // ANIMMANAGERDIALOG_H
