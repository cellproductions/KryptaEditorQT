#ifndef LAYERBROWSERDIALOG_H
#define LAYERBROWSERDIALOG_H

#include "DialogResult.h"
#include "Map.h"
#include <QDialog>

class LayerOptionsItem;

namespace Ui
{
	class LayerBrowserDialog;
}

class LayerBrowserDialog : public QDialog
{
	Q_OBJECT

	public:
		explicit LayerBrowserDialog(QWidget *parent = 0);
		~LayerBrowserDialog();

		DialogResult showDialog();
		std::shared_ptr<Map::Layer>& getSelectedLayer();

	private:
		std::vector<LayerOptionsItem*> todelete;
		std::shared_ptr<Map::Layer> currlayer;
		Ui::LayerBrowserDialog *ui;
		DialogResult lastresult;
};

#endif // LAYERBROWSERDIALOG_H
