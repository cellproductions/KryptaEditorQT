#include "Assets.h"
#include "resources.h"
#include "Utilities.h"
#include <System/Filesystem.h>
#include <QDebug>

#define STRINGIFY(x) (#x)
AssetType strToAssetType(const kry::Util::String& str);

std::vector<std::shared_ptr<Asset<kry::Graphics::Texture> > > Assets::tiles;
std::vector<std::shared_ptr<Asset<kry::Graphics::Texture> > > Assets::entities;
std::vector<std::shared_ptr<Asset<kry::Audio::Buffer> > > Assets::sounds;
std::vector<std::shared_ptr<Asset<kry::Audio::Source> > > Assets::music;
kry::Media::Config Assets::hardtypes;
bool Assets::loaded(false);

void Assets::loadAssets(const QString& rootdir)
{
	auto strrootdir = rootdir.toStdString(); // if they were all in one directory, could load them and place them based on their type?
	hardtypes.fileToConfig(qToKString(rootdir + "\\types.ini"));

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
	assets = strrootdir + "\\sounds";
	for (kry::Util::String& path : kry::System::getAllFiles(kry::Util::String(assets.c_str(), assets.length())))
	{
		if (path[path.getLength() - 1] == '.')
			continue;

		Asset<kry::Audio::Buffer>* asset = new Asset<kry::Audio::Buffer>;
		asset->properties.fileToConfig(path);
		asset->type = strToAssetType(asset->properties["global"]["type"]);
		kry::Util::String res = asset->properties["global"]["resource"];
		res = std::move(res.replace('/', '\\'));
		asset->properties["global"]["resource"] = std::move(res);
		asset->path = QString::fromStdString(std::string(path.getData(), path.getLength()));

		sounds.emplace_back(asset);
	}
	assets = strrootdir + "\\music";
	for (kry::Util::String& path : kry::System::getAllFiles(kry::Util::String(assets.c_str(), assets.length())))
	{
		if (path[path.getLength() - 1] == '.')
			continue;

		Asset<kry::Audio::Source>* asset = new Asset<kry::Audio::Source>;
		asset->properties.fileToConfig(path);
		asset->type = strToAssetType(asset->properties["global"]["type"]);
		kry::Util::String res = asset->properties["global"]["resource"];
		res = std::move(res.replace('/', '\\'));
		asset->properties["global"]["resource"] = std::move(res);
		asset->path = QString::fromStdString(std::string(path.getData(), path.getLength()));

		music.emplace_back(asset);
	}

	Resources::loadAndAssignAnimations(tiles); /** #TODO(incomplete) dont forget sounds/music */
	Resources::loadAndAssignAnimations(entities);
	Resources::loadAndAssignSounds(sounds);
	Resources::loadAndAssignMusic(music);

    loaded = true;
}

const std::vector<std::shared_ptr<Asset<kry::Graphics::Texture> > > Assets::getAllTextureAssets()
{
	std::vector<std::shared_ptr<Asset<kry::Graphics::Texture> > > assets;
	for (auto& asset : tiles)
		assets.push_back(asset);
	for (auto& asset : entities)
		assets.push_back(asset);

	return assets;
}
/*
std::shared_ptr<Asset<kry::Graphics::Texture> >& Assets::getTileByIni(const QString& path)
{
	for (auto& asset : tiles)
		if (asset->path == path)
			return asset;
	throw kry::Util::Exception("Tile ini does not exist! [path=" + qToKString(path) + ']', KRY_EXCEPTION_DATA);
}

std::shared_ptr<Asset<kry::Graphics::Texture> >& Assets::getEntityByIni(const QString& path)
{
	for (auto& asset : entities)
		if (asset->path == path)
			return asset;
	throw kry::Util::Exception("Entity ini does not exist! [path=" + qToKString(path) + ']', KRY_EXCEPTION_DATA);
}
*/
std::shared_ptr<Asset<kry::Graphics::Texture>> Assets::getTileByHardtype(const kry::Util::String& type)
{
	for (auto asset : getTiles())
		if (asset->properties["global"]["hardtype"] == type)
			return asset;
	return std::shared_ptr<Asset<kry::Graphics::Texture>>(); // null
}

std::shared_ptr<Asset<kry::Graphics::Texture>> Assets::getEntityByHardtype(const kry::Util::String& type)
{
	for (auto asset : getEntities())
		if (asset->properties["global"]["hardtype"] == type)
			return asset;
	return std::shared_ptr<Asset<kry::Graphics::Texture>>(); // null
}

Assets::Assets()
{
}


AssetType strToAssetType(const kry::Util::String& str)
{
	AssetType TILE = AssetType::TILE;
	AssetType ENTITY = AssetType::ENTITY;
	AssetType SOUND = AssetType::SOUND;
	AssetType MUSIC = AssetType::MUSIC;
	if (str == STRINGIFY(TILE))
		return TILE;
	else if (str == STRINGIFY(ENTITY))
		return ENTITY;
	if (str == STRINGIFY(SOUND))
		return SOUND;
	else if (str == STRINGIFY(MUSIC))
		return MUSIC;
	return AssetType::NONE;
}
