#ifndef LAYERBROWSERDIALOG_H
#define LAYERBROWSERDIALOG_H

#include "DialogResult.h"
#include "Map.h"
#include <QDialog>

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
		size_t getSelectedIndex();

	private:
		std::vector<Map::Layer*> todelete;
		Ui::LayerBrowserDialog *ui;
		DialogResult lastresult;
};

#endif // LAYERBROWSERDIALOG_H
