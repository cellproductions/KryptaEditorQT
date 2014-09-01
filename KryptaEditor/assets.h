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

enum class AssetType : unsigned char
{
    STATIC_TILE
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
        inline static bool isLoaded();

    private:
        Assets();

        static bool loaded;
        static std::vector<std::shared_ptr<Asset<kry::Graphics::Texture> > > tiles;
};


const std::vector<std::shared_ptr<Asset<kry::Graphics::Texture> > >& Assets::getTiles()
{
    return tiles;
}

bool Assets::isLoaded()
{
    return loaded;
}

#endif // ASSETS_H
