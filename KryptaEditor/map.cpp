#include "map.h"
#include "prjsetupdialog.h"
#include "layeroptionsitem.h"

std::shared_ptr<Map> Map::single;

void Map::resetMap()
{
    layers.clear();
    name.clear();
    currentLayer.reset();

    single.reset();
}

std::shared_ptr<Map> Map::createMap(const QString& name, const Tile& defaulttile, QListWidget* layerList)
{
    if (!single)
        single.reset(new Map(name, layerList->count()));

    for (int i = 0; i < layerList->count(); ++i)
    {
		if (layerList->item(i)->type() != LayerOptionsItem::LayerOptionType)
            continue;
		LayerOptionsItem* option = dynamic_cast<LayerOptionsItem*>(layerList->item(i));
        Layer* layer = new Layer { std::move(std::vector<Tile>(option->getWidth() * option->getHeight(), defaulttile)), option->getDescription(), {option->getWidth(), option->getHeight()} };

        single->layers.emplace_back(layer);
    }

    layerList->clear();
    single->currentLayer = single->layers[0];
    return single;
}

std::shared_ptr<Map> Map::getMap()
{
    return single;
}

Map::Map(const QString& name, size_t layerCount) : name(name)
{
    layers.reserve(layerCount);
}
