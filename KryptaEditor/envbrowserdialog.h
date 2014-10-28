#ifndef ENVBROWSERDIALOG_H
#define ENVBROWSERDIALOG_H

#include "ObjectListItem.h"
#include "assets.h"
#include "DialogResult.h"
#include <QDialog>

namespace Ui
{
    class EnvBrowserDialog;
}

namespace EnvBr
{
	ObjectListItem* createListItem(Asset<kry::Graphics::Texture>* asset, const QString& imagefile, const QString& name);
}

class EnvBrowserDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit EnvBrowserDialog(QWidget *parent = 0);
        ~EnvBrowserDialog();

        DialogResult showDialog();
        inline ObjectListItem* getSelectedItem();
		inline Ui::EnvBrowserDialog* getUI();
		inline void overrideFirstLoad(bool toggle);

    private:
        Ui::EnvBrowserDialog* ui;
        ObjectListItem* selected;
        DialogResult lastresult;
		bool firstLoad;
};


ObjectListItem* EnvBrowserDialog::getSelectedItem()
{
    return selected;
}

Ui::EnvBrowserDialog* EnvBrowserDialog::getUI()
{
	return ui;
}

void EnvBrowserDialog::overrideFirstLoad(bool toggle)
{
	firstLoad = toggle;
}

#endif // ENVBROWSERDIALOG_H
