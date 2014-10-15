#ifndef ANIMATIONSETUPWIDGET_H
#define ANIMATIONSETUPWIDGET_H

#include "Graphics/Texture.h"
#include <QWidget>
#include <memory>

template <typename ResType>
struct Animation;

class QTimer;

namespace Ui
{
	class AnimationSetupWidget;
}

class AnimationSetupWidget : public QWidget
{
		Q_OBJECT

	public:
		explicit AnimationSetupWidget(QWidget *parent = 0);
		~AnimationSetupWidget();

		void setup(std::shared_ptr<Animation<kry::Graphics::Texture> > animation);
		void release();
		inline Ui::AnimationSetupWidget* getUI();

	private:
		Ui::AnimationSetupWidget *ui;
		QTimer* timer;
};


Ui::AnimationSetupWidget* AnimationSetupWidget::getUI()
{
	return ui;
}

#endif
