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

enum class AssetType : unsigned char /** #TODO(note) should audio be managed here as well? */
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
		static void loadAssets(const QString& rootdir);
        inline static const std::vector<std::shared_ptr<Asset<kry::Graphics::Texture>>>& getTiles();
		inline static const std::vector<std::shared_ptr<Asset<kry::Graphics::Texture>>>& getEntities();
        inline static const std::vector<std::shared_ptr<Asset<kry::Audio::Buffer>>>& getSounds();
		inline static const std::vector<std::shared_ptr<Asset<kry::Audio::Source>>>& getMusic();
		static const std::vector<std::shared_ptr<Asset<kry::Graphics::Texture>>> getAllTextureAssets();
		inline static const kry::Media::Config& getHardTypes();
        inline static bool isLoaded();
		inline static kry::Util::String getParentType(const kry::Util::String& hardtype);
		static std::shared_ptr<Asset<kry::Graphics::Texture>> getTileByHardtype(const kry::Util::String& type);
		static std::shared_ptr<Asset<kry::Graphics::Texture>> getEntityByHardtype(const kry::Util::String& type);

    private:
        Assets();

        static std::vector<std::shared_ptr<Asset<kry::Graphics::Texture>>> tiles;
		static std::vector<std::shared_ptr<Asset<kry::Graphics::Texture>>> entities;
        static std::vector<std::shared_ptr<Asset<kry::Audio::Buffer>>> sounds;
		static std::vector<std::shared_ptr<Asset<kry::Audio::Source>>> music;
		static kry::Media::Config hardtypes;
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

bool Assets::isLoaded()
{
    return loaded;
}

kry::Util::String Assets::getParentType(const kry::Util::String& hardtype) /** #TODO(incomplete) update this as you go */
{
	if (hardtype == "itemloot" || hardtype == "itemkey")
		return kry::Util::String("item");
	if (hardtype == "void" || hardtype == "solid" || hardtype == "wall")
		return kry::Util::String("floor");
	return kry::Util::String("entity");
}

#endif // ASSETS_H
