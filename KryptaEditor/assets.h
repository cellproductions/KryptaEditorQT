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
struct Resource;

enum class AssetType : unsigned char
{
	TILE,
	ENTITY,
	SOUND,
	MUSIC,
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
		static void loadAssets(const QString& rootdir, bool assign);
        inline static const std::vector<std::shared_ptr<Asset<kry::Graphics::Texture>>>& getTiles();
		inline static const std::vector<std::shared_ptr<Asset<kry::Graphics::Texture>>>& getEntities();
        inline static const std::vector<std::shared_ptr<Asset<kry::Audio::Buffer>>>& getSounds();
		inline static const std::vector<std::shared_ptr<Asset<kry::Audio::Source>>>& getMusic();
		static const std::vector<std::shared_ptr<Asset<kry::Graphics::Texture>>> getAllTextureAssets();
		inline static const kry::Media::Config& getHardTypes();
		inline static const kry::Media::Config& getRequiredKeys();
        inline static bool isLoaded();
		inline static kry::Util::String getParentType(const kry::Util::String& hardtype);
		inline static bool isParentType(const kry::Util::String& hardtype);
		static std::shared_ptr<Asset<kry::Graphics::Texture>> getTileByHardtype(const kry::Util::String& type);
		static std::shared_ptr<Asset<kry::Graphics::Texture>> getEntityByHardtype(const kry::Util::String& type);
		inline static void eraseAll();

    private:
        Assets();

        static std::vector<std::shared_ptr<Asset<kry::Graphics::Texture>>> tiles;
		static std::vector<std::shared_ptr<Asset<kry::Graphics::Texture>>> entities;
        static std::vector<std::shared_ptr<Asset<kry::Audio::Buffer>>> sounds;
		static std::vector<std::shared_ptr<Asset<kry::Audio::Source>>> music;

		static kry::Media::Config hardtypes;
		static kry::Media::Config requiredkeys;
		static bool loaded;
};


const std::vector<std::shared_ptr<Asset<kry::Graphics::Texture> > >& Assets::getTiles()
{
    return tiles;
}

const std::vector<std::shared_ptr<Asset<kry::Graphics::Texture> > >& Assets::getEntities()
{
	return entities;
}

const std::vector<std::shared_ptr<Asset<kry::Audio::Buffer> > >& Assets::getSounds()
{
    return sounds;
}

const std::vector<std::shared_ptr<Asset<kry::Audio::Source> > >& Assets::getMusic()
{
	return music;
}

const kry::Media::Config& Assets::getHardTypes()
{
	return hardtypes;
}

const kry::Media::Config& Assets::getRequiredKeys()
{
	return requiredkeys;
}

bool Assets::isLoaded()
{
    return loaded;
}

kry::Util::String Assets::getParentType(const kry::Util::String& hardtype) /** #TODO(incomplete) update this as you go */
{
	if (hardtype == "lootitem" || hardtype == "keyitem" || hardtype == "weaponitem")
		return kry::Util::String("item");
	if (hardtype == "void" || hardtype == "solid" || hardtype == "wall")
		return kry::Util::String("floor");
	return kry::Util::String("entity");
}

bool Assets::isParentType(const kry::Util::String& hardtype)
{
	return hardtype == "entity" || hardtype == "floor" || hardtype == "item";
}

void Assets::eraseAll()
{
	tiles.clear();
	entities.clear();
	sounds.clear();
	music.clear();
}

#endif // ASSETS_H
