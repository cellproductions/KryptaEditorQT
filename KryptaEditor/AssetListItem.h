#ifndef ASSETLISTITEM_H
#define ASSETLISTITEM_H

#include "Assets.h"
#include <QListWidgetItem>

struct AssetListItem : public QListWidgetItem
{
	AssetListItem(Asset<kry::Graphics::Texture>* asset, const QIcon& icon, const QString& text);
	Asset<kry::Graphics::Texture>* asset;
};

#endif // ASSETLISTITEM_H
