#include "Resources.h"
#include "Configuration.h"
#include "Utilities.h"
#include <System/Filesystem.h>
#include <Media/ImageFactory.h>
#include <Media/AudioFactory.h>
#include <QMessageBox>
#include <QDebug>

std::vector<std::shared_ptr<Animation<> > > Resources::animations;
std::vector<std::shared_ptr<Resource<kry::Graphics::Texture> > > Resources::textures;
std::vector<std::shared_ptr<Resource<kry::Audio::Buffer> > > Resources::sounds;
std::vector<std::shared_ptr<Resource<kry::Audio::Source> > > Resources::music;
std::shared_ptr<Resource<kry::Graphics::Texture> > Resources::missingtexture;

template <typename ResType>
Animation<ResType>* Animation<ResType>::createDefaultAnimation(const kry::Util::String& strimagefile)
{
	auto imagefile = strimagefile.replace('\\', '/');

	Animation<>* animation = new Animation<>;
	animation->type = ResourceType::ANIMATION;
	animation->path = kryToQString(imagefile);
	auto name = imagefile.substring(imagefile.lastIndexOf('/') + 1);
	auto pname = name.substring(0, name.lastIndexOf('.'));
	animation->name = kryToQString(name);
	animation->properties[""]["name"] = pname;
	animation->properties[pname]["frames"] = "1";
	animation->properties[pname]["fps"] = "1";
	animation->properties[pname]["sheetImage"] = imagefile;
	animation->properties[pname]["sheetDimensions"] = "{ 1, 1 }";
	if (imagefile.endsWith(".sheet"))
	{
		/** #TODO(incomplete) add spritesheet loading */
		// load first image from spritesheet and set rawresource to that
	}
	else
	{
		animation->rawresource = new kry::Graphics::Texture(std::move(kry::Media::imageFileToTexture(imagefile, Configuration::getConfig()["editor"]["mipmapping"] == "true")));
		auto dims = animation->rawresource->getDimensions();
		animation->properties[pname]["framePivot"] = "{ " + kry::Util::toString(dims[0] * 0.5f) + ", " + kry::Util::toString(dims[1] * 0.5f) + " }";
	}
	animation->properties[pname]["frameDimensions"] = "";
	animation->properties[pname]["dimensions"] = "";
	animation->properties[pname]["sheetPosition"] = "";
	animation->properties[pname]["frameRGBA"] = "";
	animation->properties[pname]["frameRotation"] = "";
	animation->properties[pname]["frameLinearFilter"] = "";
	animation->properties[pname]["framePivot"] = "";
	animation->properties[pname]["frameSortDepth"] = "";
	animation->properties[pname]["frameSortPivotOffset"] = "";
	animation->properties[pname]["nextSkin"] = "";

	pname += ": 0";
	animation->properties[pname]["image"] = "";
	animation->properties[pname]["pivot"] = "";
	animation->properties[pname]["dimensions"] = "";
	animation->properties[pname]["RGBA"] = "";
	animation->properties[pname]["rotation"] = "";
	animation->properties[pname]["UVposition"] = "";
	animation->properties[pname]["UVdimensions"] = "";
	animation->properties[pname]["linearFilter"] = "";
	animation->properties[pname]["sortDepth"] = "";
	animation->properties[pname]["sortPivotOffset"] = "";

	return animation;
}

void Resources::loadResources(const QString& rootdir) /** #TODO(incomplete) add exception handling here (collect list of resources that failed to load, display them in message box) */
{
    auto strrootdir = rootdir.toStdString();
    auto resources = strrootdir + "\\images";
    for (kry::Util::String& path : kry::System::getAllFiles(kry::Util::String(resources.c_str(), resources.length())))
    {
        if (path[path.getLength() - 1] == '.')
            continue;

        Resource<kry::Graphics::Texture>* resource = new Resource<kry::Graphics::Texture>;
		resource->type = ResourceType::TEXTURE;
		resource->rawresource = new kry::Graphics::Texture(std::move(kry::Media::imageFileToTexture(path, Configuration::getConfig()["editor"]["mipmapping"] == "true")));
        resource->path = QString::fromStdString(std::string(path.getData(), path.getLength()));
        auto name = path.substring(path.lastIndexOf('\\') + 1);
        resource->name = QString::fromStdString(std::string(name.getData(), name.getLength()));

        textures.emplace_back(resource);
    }
    resources = strrootdir + "\\audio";
    for (kry::Util::String& path : kry::System::getAllFiles(kry::Util::String(resources.c_str(), resources.length())))
    {
        if (path[path.getLength() - 1] == '.')
            continue;

        Resource<kry::Audio::Buffer>* resource = new Resource<kry::Audio::Buffer>;
        resource->type = ResourceType::SOUND;
        resource->rawresource = nullptr;
        resource->path = QString::fromStdString(std::string(path.getData(), path.getLength()));
        auto name = path.substring(path.lastIndexOf('\\') + 1);
        resource->name = QString::fromStdString(std::string(name.getData(), name.getLength()));

        sounds.emplace_back(resource);
    }
    resources += "\\music";
    for (kry::Util::String& path : kry::System::getAllFiles(kry::Util::String(resources.c_str(), resources.length())))
    {
        if (path[path.getLength() - 1] == '.')
            continue;

        Resource<kry::Audio::Source>* resource = new Resource<kry::Audio::Source>;
        resource->type = ResourceType::MUSIC;
        resource->rawresource = nullptr;
        resource->path = QString::fromStdString(std::string(path.getData(), path.getLength()));
        auto name = path.substring(path.lastIndexOf('\\') + 1);
        resource->name = QString::fromStdString(std::string(name.getData(), name.getLength()));

        music.emplace_back(resource);
    }
}

void Resources::loadAndAssignAnimations(std::vector<std::shared_ptr<Asset<kry::Graphics::Texture> > >& assets)
{
	try
	{
		for (std::shared_ptr<Asset<kry::Graphics::Texture> >& asset : assets)
		{
			Animation<>* animation = Animation<>::createDefaultAnimation(asset->properties["global"]["resource"]);
			auto dims = animation->rawresource->getDimensions();
			if (asset->properties.sectionExists("object"))
			{
				animation->properties[animation->properties[""]["name"]]["framePivot"] = "{ " +
						kry::Util::toString((asset->properties["object"].keyExists("relativex") ? kry::Util::toDecimal<float>(asset->properties["object"]["relativex"]) : 0.5f) * dims[0]) +
						", " +
						kry::Util::toString((asset->properties["object"].keyExists("relativey") ? kry::Util::toDecimal<float>(asset->properties["object"]["relativey"]) : 0.5f) * dims[1]) +
						" }";
			}
			else if (asset->properties.sectionExists("entity"))
			{
				animation->properties[animation->properties[""]["name"]]["framePivot"] = "{ " +
						kry::Util::toString(kry::Util::toDecimal<float>(asset->properties["entity"]["relativex"]) * dims[0]) + ", " +
						kry::Util::toString(kry::Util::toDecimal<float>(asset->properties["entity"]["relativey"]) * dims[1]) + " }";
			}
			else
				animation->properties[animation->properties[""]["name"]]["framePivot"] = "{ " + kry::Util::toString(dims[0] * 0.5f) + ", " + kry::Util::toString(dims[1] * 0.5f) + " }";

			animations.emplace_back(animation);
			asset->resource = animation;
		}
	}
	catch (const kry::Util::Exception& e)
	{
		QMessageBox::information(nullptr, "Animation Loading Error", e.what(), QMessageBox::Ok);
	}
}

void Resources::loadAndAssignTextures(std::vector<std::shared_ptr<Asset<kry::Graphics::Texture> > >& assets)
{
    for (std::shared_ptr<Asset<kry::Graphics::Texture> >& asset : assets)
    {
        Resource<kry::Graphics::Texture>* resource = new Resource<kry::Graphics::Texture>;
        resource->type = ResourceType::TEXTURE;
        auto path = asset->properties["global"]["resource"];
		resource->rawresource = new kry::Graphics::Texture(std::move(kry::Media::imageFileToTexture(path, Configuration::getConfig()["editor"]["mipmapping"] == "true")));
		resource->path = kryToQString(path);
        auto name = path.substring(path.lastIndexOf('\\') + 1);
		resource->name = kryToQString(name);

        textures.emplace_back(resource);
        asset->resource = resource;
    }
}

void Resources::initMissingTexture()
{
	Resource<kry::Graphics::Texture>* missing = new Resource<kry::Graphics::Texture>;
	missing->path = "editor\\missing.png";
	missing->name = "missing.png";
	missing->type = ResourceType::TEXTURE;
	missing->rawresource = new kry::Graphics::Texture(std::move(kry::Media::imageFileToTexture("editor\\missing.png", Configuration::getConfig()["editor"]["mipmapping"] == "true")));
	missingtexture.reset(missing);
}

Resources::Resources()
{
}
