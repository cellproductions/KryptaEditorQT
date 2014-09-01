#include "assets.h"
#include "resources.h"
#include <System/Filesystem.h>

bool Assets::loaded(false);
std::vector<std::shared_ptr<Asset<kry::Graphics::Texture> > > Assets::tiles;

void Assets::loadAssets(const QString& rootdir)
{
    auto strrootdir = rootdir.toStdString();
    auto assets = strrootdir + "\\tiles";
    for (kry::Util::String& path : kry::System::getAllFiles(kry::Util::String(assets.c_str(), assets.length())))
    {
        if (path[path.getLength() - 1] == '.')
            continue;

        Asset<kry::Graphics::Texture>* asset = new Asset<kry::Graphics::Texture>;
        asset->type = AssetType::STATIC_TILE;
        asset->properties.fileToConfig(path);
        kry::Util::String res = asset->properties["global"]["resource"];
        res = std::move(res.replace('/', '\\'));
        asset->properties["global"]["resource"] = std::move(res);
        asset->path = QString::fromStdString(std::string(path.getData(), path.getLength()));

        tiles.emplace_back(asset);
    }

    Resources::loadAndAssignTextures(tiles); // run these at the end

    loaded = true;
}

Assets::Assets()
{
}
