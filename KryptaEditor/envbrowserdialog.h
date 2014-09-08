#ifndef ENVBROWSERDIALOG_H
#define ENVBROWSERDIALOG_H

#include "AssetListItem.h"
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
        inline AssetListItem* getSelectedAssetItem();

    private:
        Ui::EnvBrowserDialog *ui;
        AssetListItem* selected;
        DialogResult lastresult;
};


AssetListItem* EnvBrowserDialog::getSelectedAssetItem()
{
    return selected;
}

#endif // ENVBROWSERDIALOG_H
