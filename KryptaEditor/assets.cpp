#include "Assets.h"
#include "resources.h"
#include <System/Filesystem.h>

#define STRINGIFY(x) (#x)
AssetType strToAssetType(const kry::Util::String& str);

bool Assets::loaded(false);
std::vector<std::shared_ptr<Asset<kry::Graphics::Texture> > > Assets::tiles;
std::vector<std::shared_ptr<Asset<kry::Graphics::Texture> > > Assets::objects;
std::vector<std::shared_ptr<Asset<kry::Graphics::Texture> > > Assets::entities;

void Assets::loadAssets(const QString& rootdir)
{
	auto strrootdir = rootdir.toStdString(); // if they were all in one directory, could load them and place them based on their type?
    auto assets = strrootdir + "\\tiles";
    for (kry::Util::String& path : kry::System::getAllFiles(kry::Util::String(assets.c_str(), assets.length())))
    {
        if (path[path.getLength() - 1] == '.')
            continue;

		Asset<kry::Graphics::Texture>* asset = new Asset<kry::Graphics::Texture>;
        asset->properties.fileToConfig(path);
		asset->type = strToAssetType(asset->properties["global"]["type"]);
        kry::Util::String res = asset->properties["global"]["resource"];
        res = std::move(res.replace('/', '\\'));
        asset->properties["global"]["resource"] = std::move(res);
        asset->path = QString::fromStdString(std::string(path.getData(), path.getLength()));

        tiles.emplace_back(asset);
    }
	assets = strrootdir + "\\objects";
	for (kry::Util::String& path : kry::System::getAllFiles(kry::Util::String(assets.c_str(), assets.length())))
	{
		if (path[path.getLength() - 1] == '.')
			continue;

		Asset<kry::Graphics::Texture>* asset = new Asset<kry::Graphics::Texture>;
		asset->properties.fileToConfig(path);
		asset->type = strToAssetType(asset->properties["global"]["type"]);
		kry::Util::String res = asset->properties["global"]["resource"];
		res = std::move(res.replace('/', '\\'));
		asset->properties["global"]["resource"] = std::move(res);
		asset->path = QString::fromStdString(std::string(path.getData(), path.getLength()));

		objects.emplace_back(asset);
	}
	assets = strrootdir + "\\entities";
	for (kry::Util::String& path : kry::System::getAllFiles(kry::Util::String(assets.c_str(), assets.length())))
	{
		if (path[path.getLength() - 1] == '.')
			continue;

		Asset<kry::Graphics::Texture>* asset = new Asset<kry::Graphics::Texture>;
		asset->properties.fileToConfig(path);
		asset->type = strToAssetType(asset->properties["global"]["type"]);
		kry::Util::String res = asset->properties["global"]["resource"];
		res = std::move(res.replace('/', '\\'));
		asset->properties["global"]["resource"] = std::move(res);
		asset->path = QString::fromStdString(std::string(path.getData(), path.getLength()));

		entities.emplace_back(asset);
	}

    Resources::loadAndAssignTextures(tiles); // run these at the end
	Resources::loadAndAssignTextures(objects);
	Resources::loadAndAssignTextures(entities);

    loaded = true;
}

Assets::Assets()
{
}


AssetType strToAssetType(const kry::Util::String& str)
{
	AssetType STATIC_TILE = AssetType::STATIC_TILE;
	AssetType STATIC_TILE_DECAL = AssetType::STATIC_TILE_DECAL;
	AssetType STATIC_TILE_TRAP = AssetType::STATIC_TILE_TRAP;
	AssetType STATIC_ENTITY = AssetType::STATIC_ENTITY;
	AssetType ANIM_TILE_DECAL = AssetType::ANIM_TILE_DECAL;
	AssetType ANIM_TILE_TRAP = AssetType::ANIM_TILE_TRAP;
	AssetType ANIM_ENTITY = AssetType::ANIM_ENTITY;
	if (str == STRINGIFY(STATIC_TILE))
		return STATIC_TILE;
	else if (str == STRINGIFY(STATIC_TILE_DECAL))
		return STATIC_TILE_DECAL;
	else if (str == STRINGIFY(STATIC_TILE_TRAP))
		return STATIC_TILE_TRAP;
	else if (str == STRINGIFY(STATIC_ENTITY))
		return STATIC_ENTITY;
	else if (str == STRINGIFY(ANIM_TILE_DECAL))
		return ANIM_TILE_DECAL;
	else if (str == STRINGIFY(ANIM_TILE_TRAP))
		return ANIM_TILE_TRAP;
	else if (str == STRINGIFY(ANIM_ENTITY))
		return ANIM_ENTITY;
	return AssetType::NONE;
}
