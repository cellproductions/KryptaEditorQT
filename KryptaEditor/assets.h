#ifndef ASSETS_H
#define ASSETS_H

#include <Graphics/Texture.h>
#include <Audio/Buffer.h>
#include <Audio/Source.h>
#include <Media/ConfigFactory.h>
#include <QString>
#include <vector>
#include <memory>

template <typename ResType>
struct Resource; // resources.h

enum class AssetType : unsigned char /** #TODO(note) are these now redundant? */
{
	STATIC_TILE,
	STATIC_TILE_DECAL,
	STATIC_TILE_OBJECT,
	STATIC_ENTITY,
	ANIM_TILE_DECAL,
	ANIM_TILE_OBJECT,
	ANIM_ENTITY,
	NONE
};

template <typename ResType>
struct Asset
{
    kry::Media::Config properties;
    QString path;
    Resource<ResType>* resource;
    AssetType type;
};

class Assets
{
    public:
		static void loadAssets(const QString& rootdir);
        inline static const std::vector<std::shared_ptr<Asset<kry::Graphics::Texture> > >& getTiles();
		inline static const std::vector<std::shared_ptr<Asset<kry::Graphics::Texture> > >& getObjects();
		inline static const std::vector<std::shared_ptr<Asset<kry::Graphics::Texture> > >& getEntities();
		static std::shared_ptr<Asset<kry::Graphics::Texture> >& getTileByIni(const QString& path);
		static std::shared_ptr<Asset<kry::Graphics::Texture> >& getObjectByIni(const QString& path);
		static std::shared_ptr<Asset<kry::Graphics::Texture> >& getEntityByIni(const QString& path);
		inline static const kry::Media::Config& getHardTypes();
        inline static bool isLoaded();

    private:
        Assets();

        static std::vector<std::shared_ptr<Asset<kry::Graphics::Texture> > > tiles;
		static std::vector<std::shared_ptr<Asset<kry::Graphics::Texture> > > objects;
		static std::vector<std::shared_ptr<Asset<kry::Graphics::Texture> > > entities;
		static kry::Media::Config hardtypes;
		static bool loaded;
};


const std::vector<std::shared_ptr<Asset<kry::Graphics::Texture> > >& Assets::getTiles()
{
    return tiles;
}

const std::vector<std::shared_ptr<Asset<kry::Graphics::Texture> > >& Assets::getObjects()
{
	return objects;
}

const std::vector<std::shared_ptr<Asset<kry::Graphics::Texture> > >& Assets::getEntities()
{
	return entities;
}

const kry::Media::Config& Assets::getHardTypes()
{
	return hardtypes;
}

bool Assets::isLoaded()
{
    return loaded;
}

#endif // ASSETS_H
