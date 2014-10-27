#ifndef ENVBROWSERDIALOG_H
#define ENVBROWSERDIALOG_H

#include "ObjectListItem.h"
#include "DialogResult.h"
#include <QDialog>

namespace Ui
{
    class EnvBrowserDialog;
}

class EnvBrowserDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit EnvBrowserDialog(QWidget *parent = 0);
        ~EnvBrowserDialog();

        DialogResult showDialog();
        inline ObjectListItem* getSelectedItem();

    private:
        Ui::EnvBrowserDialog *ui;
        ObjectListItem* selected;
        DialogResult lastresult;
		bool firstLoad;
};


ObjectListItem* EnvBrowserDialog::getSelectedItem()
{
    return selected;
}

#endif // ENVBROWSERDIALOG_H
