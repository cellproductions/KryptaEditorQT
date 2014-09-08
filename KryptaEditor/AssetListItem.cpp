#include "AssetListItem.h"

AssetListItem::AssetListItem(Asset<kry::Graphics::Texture>* asset, const QIcon& icon, const QString& text) : QListWidgetItem(icon, text), asset(asset)
{
}
