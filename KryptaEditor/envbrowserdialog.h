#ifndef ENVBROWSERDIALOG_H
#define ENVBROWSERDIALOG_H

#include "assets.h"
#include "dialogresult.h"
#include <QDialog>
#include <QListWidgetItem>
#include <QIcon>

struct AssetListItem : public QListWidgetItem
{
    AssetListItem(Asset<kry::Graphics::Texture>* asset, const QIcon& icon, const QString& text);
    Asset<kry::Graphics::Texture>* asset;
};

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
