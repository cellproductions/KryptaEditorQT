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
std::vector<std::shared_ptr<Resource<kry::Graphics::Texture> > > Resources::editortextures;

template <typename ResType>
Animation<ResType>* Animation<ResType>::createDefaultAnimation(const kry::Util::String& strimagefile, const kry::Util::String& animname)
{
	auto imagefile = strimagefile.replace('\\', '/');

	Animation<>* animation = new Animation<>;
	animation->type = ResourceType::ANIMATION;
	animation->path = kryToQString(imagefile);
	auto name = imagefile.substring(imagefile.lastIndexOf('/') + 1);
	animation->name = kryToQString(animname.isEmpty() ? name : animname);
	for (int i = 0; i < Animation<>::MAX_DIRECTION_COUNT; ++i)
	{
		auto pname = animname.isEmpty() ? name.substring(0, name.lastIndexOf('.')) : animname;
		pname += kry::Util::toString(i);
		animation->properties[i]["Skins"]["name"] = pname;
		animation->properties[i][pname]["frames"] = "1";
		animation->properties[i][pname]["fps"] = "1";
		animation->properties[i][pname]["sheetDimensions"] = "{ 1, 1 }";
		if (i == 0)
		{
			animation->properties[i][pname]["sheetImage"] = imagefile;
			if (imagefile.endsWith(".sheet"))
			{
				/** #TODO(incomplete) add spritesheet loading */
				// load first image from spritesheet and set rawresource to that
			}
			else
			{
				animation->rawresource = new kry::Graphics::Texture(kry::Media::imageFileToTexture(imagefile, Configuration::getConfig()["editor"]["mipmapping"] == "true"));
				auto dims = animation->rawresource->getDimensions();
				animation->properties[i][pname]["framePivot"] = "{ " + kry::Util::toString(dims[0] * 0.5f) + ", " + kry::Util::toString(dims[1] * 0.5f) + " }";
			}
		}
		else
		{
			animation->properties[i][pname]["sheetImage"] = "";
			animation->properties[i][pname]["framePivot"] = "";
		}
		animation->properties[i][pname]["frameDimensions"] = "";
		animation->properties[i][pname]["dimensions"] = "";
		animation->properties[i][pname]["sheetPosition"] = "";
		animation->properties[i][pname]["frameRGBA"] = "";
		animation->properties[i][pname]["frameRotation"] = "";
		animation->properties[i][pname]["frameLinearFilter"] = "";
		animation->properties[i][pname]["framePivot"] = "";
		animation->properties[i][pname]["frameSortDepth"] = "";
		animation->properties[i][pname]["frameSortPivotOffset"] = "";
		animation->properties[i][pname]["nextSkin"] = "";

		pname += ": 0";
		animation->properties[i][pname]["image"] = "";
		animation->properties[i][pname]["pivot"] = "";
		animation->properties[i][pname]["dimensions"] = "";
		animation->properties[i][pname]["RGBA"] = "";
		animation->properties[i][pname]["rotation"] = "";
		animation->properties[i][pname]["UVposition"] = "";
		animation->properties[i][pname]["UVdimensions"] = "";
		animation->properties[i][pname]["linearFilter"] = "";
		animation->properties[i][pname]["sortDepth"] = "";
		animation->properties[i][pname]["sortPivotOffset"] = "";
	}

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
		resource->rawresource = new kry::Graphics::Texture(kry::Media::imageFileToTexture(path, Configuration::getConfig()["editor"]["mipmapping"] == "true"));
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
				for (auto& properties : animation->properties)
				{
					properties[properties["Skins"]["name"]]["framePivot"] = "{ " +
						kry::Util::toString((asset->properties["object"].keyExists("relativex") ? kry::Util::toDecimal<float>(asset->properties["object"]["relativex"]) : 0.5f) * dims[0]) +
						", " +
						kry::Util::toString((asset->properties["object"].keyExists("relativey") ? kry::Util::toDecimal<float>(asset->properties["object"]["relativey"]) : 0.5f) * dims[1]) +
						" }";
				}
			}
			else if (asset->properties.sectionExists("entity"))
			{
				for (auto& properties : animation->properties)
				{
					properties[properties["Skins"]["name"]]["framePivot"] = "{ " +
							kry::Util::toString(kry::Util::toDecimal<float>(asset->properties["entity"]["relativex"]) * dims[0]) + ", " +
							kry::Util::toString(kry::Util::toDecimal<float>(asset->properties["entity"]["relativey"]) * dims[1]) + " }";
				}
			}
			else
				for (auto& properties : animation->properties)
					properties[properties["Skins"]["name"]]["framePivot"] = "{ " + kry::Util::toString(dims[0] * 0.5f) + ", " + kry::Util::toString(dims[1] * 0.5f) + " }";

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
		resource->rawresource = new kry::Graphics::Texture(kry::Media::imageFileToTexture(path, Configuration::getConfig()["editor"]["mipmapping"] == "true"));
		resource->path = kryToQString(path);
        auto name = path.substring(path.lastIndexOf('\\') + 1);
		resource->name = kryToQString(name);

        textures.emplace_back(resource);
        asset->resource = resource;
    }
}

void Resources::initEditorTextures()
{
	try
	{
		Resource<kry::Graphics::Texture>* missing = new Resource<kry::Graphics::Texture>;
		missing->path = QString("editor\\missing.png");
		missing->name = QString("missing.png");
		missing->type = ResourceType::TEXTURE;
		missing->rawresource = new kry::Graphics::Texture(kry::Media::imageFileToTexture("editor\\missing.png", Configuration::getConfig()["editor"]["mipmapping"] == "true"));

		Resource<kry::Graphics::Texture>* flag_red = new Resource<kry::Graphics::Texture>;
		flag_red->path = QString("editor\\flag_red.png");
		flag_red->name = QString("flag_red.png");
		flag_red->type = ResourceType::TEXTURE;
		flag_red->rawresource = new kry::Graphics::Texture(kry::Media::imageFileToTexture("editor\\flag_red.png", Configuration::getConfig()["editor"]["mipmapping"] == "true"));

		Resource<kry::Graphics::Texture>* flag_green = new Resource<kry::Graphics::Texture>;
		flag_green->path = QString("editor\\flag_green.png");
		flag_green->name = QString("flag_green.png");
		flag_green->type = ResourceType::TEXTURE;
		flag_green->rawresource = new kry::Graphics::Texture(kry::Media::imageFileToTexture("editor\\flag_green.png", Configuration::getConfig()["editor"]["mipmapping"] == "true"));

		editortextures.emplace_back(missing);
		editortextures.emplace_back(flag_red);
		editortextures.emplace_back(flag_green);
	}
	catch (const kry::Util::Exception& e)
	{
		QMessageBox::information(nullptr, "Editor Loading Error", e.what(), QMessageBox::Ok);
	}
}

Resources::Resources()
{
}
