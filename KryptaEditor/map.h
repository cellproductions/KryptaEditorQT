#ifndef MAP_H
#define MAP_H

#include "Assets.h"
#include <Graphics/Primitives.h>
#include <Utilities/Vector.h>
#include <QListWidget>
#include <vector>
#include <memory>

struct Object
{
	kry::Media::Config properties;
	kry::Graphics::Sprite sprite;
	Asset<kry::Graphics::Texture>* asset;
};

struct Tile : public Object
{
	std::vector<std::shared_ptr<Object> > objects;
};

class Map
{
    public:
        struct Layer
        {
            std::vector<Tile> tiles;
            QString description;
            kry::Util::Vector2i size;
			kry::Util::Vector2i tilesize;
        };

		inline void setCurrentLayer(size_t index);
        inline std::vector<Tile>& getTiles(size_t layerindex);
        inline const kry::Util::Vector2i& getSize(size_t layerindex) const;
        inline std::vector<std::shared_ptr<Layer> >& getLayers();
        inline std::shared_ptr<Layer>& getCurrentLayer();
        inline const QString& getName() const;
        void resetMap();

		inline static void setProjectName(const QString& name);
		inline static const QString& getProjectName();
        static std::shared_ptr<Map> createMap(const QString& name, const Tile& defaulttile, QListWidget* layerList);
		static std::shared_ptr<Map> loadFromFile(const QString& name);
		static void saveToFile(const QString& name);
		static void exportToFile(const QString& name);
        static std::shared_ptr<Map> getMap();

    private:
        Map(const QString& name, size_t layerCount);

        std::vector<std::shared_ptr<Layer> > layers;
        QString name;
        std::shared_ptr<Layer> currentLayer;

        static std::shared_ptr<Map> single;
		static QString projectname;
};


void Map::setCurrentLayer(size_t index)
{
	currentLayer = layers[index];
}

std::vector<Tile>& Map::getTiles(size_t layerindex)
{
    return layers[layerindex]->tiles;
}

const kry::Util::Vector2i& Map::getSize(size_t layerindex) const
{
    return layers[layerindex]->size;
}

std::vector<std::shared_ptr<Map::Layer> >& Map::getLayers()
{
    return layers;
}

std::shared_ptr<Map::Layer>& Map::getCurrentLayer()
{
    return currentLayer;
}

const QString& Map::getName() const
{
    return name;
}

void Map::setProjectName(const QString& name)
{
	projectname = name;
}

const QString& Map::getProjectName()
{
	return projectname;
}

#endif // MAP_H
