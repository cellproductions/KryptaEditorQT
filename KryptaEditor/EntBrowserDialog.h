#ifndef ENTBROWSERDIALOG_H
#define ENTBROWSERDIALOG_H

#include "AssetListItem.h"
#include "DialogResult.h"
#include <QDialog>

namespace Ui
{
	class EntBrowserDialog;
}

class EntBrowserDialog : public QDialog
{
		Q_OBJECT

	public:
		explicit EntBrowserDialog(QWidget *parent = 0);
		~EntBrowserDialog();

		DialogResult showDialog();
		inline AssetListItem* getSelectedAssetItem();

	private:
		Ui::EntBrowserDialog *ui;
		AssetListItem* selected;
		DialogResult lastresult;
};


AssetListItem* EntBrowserDialog::getSelectedAssetItem()
{
	return selected;
}

#endif // ENTBROWSERDIALOG_H
