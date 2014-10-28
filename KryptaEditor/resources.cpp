#include "Resources.h"
#include "Configuration.h"
#include "Utilities.h"
#include <System/Filesystem.h>
#include <Media/ImageFactory.h>
#include <Media/AudioFactory.h>
#include <QMessageBox>
#include <QDebug>

std::vector<std::shared_ptr<Animation<> > > Resources::animations;
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
	auto name = imagefile.contains('/') ? imagefile.substring(imagefile.lastIndexOf('/') + 1) : imagefile;
	animation->name = kryToQString(animname.isEmpty() ? name : animname);
	for (int i = 0; i < Animation<>::MAX_DIRECTION_COUNT; ++i)
	{
		auto pname = animname.isEmpty() ? name.substring(0, name.lastIndexOf('.')) : animname;
		pname += kry::Util::toString(i);
		animation->properties[i]["Skins"]["name"] = pname;
		animation->properties[i][pname]["frames"] = "1";
		animation->properties[i][pname]["fps"] = "1";
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
				auto dims = animation->rawresource->getDimensions() * 0.5f;
				animation->properties[i][pname]["framePivot"] = dims.toString();
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

		animation->frames[i].push_back(kry::Media::Config());
		animation->frames[i].at(0)["Skins"]["name"] = pname;
		animation->frames[i].at(0)[pname]["image"] = "";
		animation->frames[i].at(0)[pname]["pivot"] = "";
		animation->frames[i].at(0)[pname]["dimensions"] = "";
		animation->frames[i].at(0)[pname]["RGBA"] = "";
		animation->frames[i].at(0)[pname]["rotation"] = "";
		animation->frames[i].at(0)[pname]["UVposition"] = "";
		animation->frames[i].at(0)[pname]["UVdimensions"] = "";
		animation->frames[i].at(0)[pname]["linearFilter"] = "";
		animation->frames[i].at(0)[pname]["sortDepth"] = "";
		animation->frames[i].at(0)[pname]["sortPivotOffset"] = "";
	}

	return animation;
}

Resource<kry::Audio::Buffer>* createDefaultSound(const kry::Util::String& soundfile)
{
	Resource<kry::Audio::Buffer>* resource = new Resource<kry::Audio::Buffer>;
	resource->name = kryToQString(soundfile.substring(soundfile.lastIndexOf('\\') + 1));
	resource->path = kryToQString(soundfile);
	resource->type = ResourceType::SOUND;
	kry::Audio::Format format;
	unsigned frequency;
	auto pcm = kry::Media::mp3FileToPCM(soundfile, format, frequency);
	resource->rawresource = new kry::Audio::Buffer(pcm, format, frequency);
	return resource;
}

Resource<kry::Audio::Source>* createDefaultMusic(const kry::Util::String& musicfile)
{
	Resource<kry::Audio::Source>* resource = new Resource<kry::Audio::Source>;
	resource->name = kryToQString(musicfile.substring(musicfile.lastIndexOf('\\') + 1));
	resource->path = kryToQString(musicfile);
	resource->type = ResourceType::MUSIC;
	resource->rawresource = nullptr;
	return resource;
}

void Resources::loadAndAssignAnimations(std::vector<std::shared_ptr<Asset<kry::Graphics::Texture> > >& assets)
{
	try
	{
		for (std::shared_ptr<Asset<kry::Graphics::Texture> >& asset : assets)
		{
			Animation<>* animation = Animation<>::createDefaultAnimation(asset->properties["global"]["resource"]);
			auto dims = animation->rawresource->getDimensions();
			for (auto& properties : animation->properties)
			{
				auto px = kry::Util::toDecimal<float>(asset->properties["global"]["relativex"]);
				auto py = kry::Util::toDecimal<float>(asset->properties["global"]["relativey"]);
				properties[properties["Skins"]["name"]]["framePivot"] = (kry::Util::Vector2f(px, py) * dims).toString();
			}

			animations.emplace_back(animation);
			asset->resource = animation;
		}
	}
	catch (const kry::Util::Exception& e)
	{
		QMessageBox::information(nullptr, "Animation Loading Error", e.what(), QMessageBox::Ok);
	}
}

void Resources::loadAndAssignSounds(std::vector<std::shared_ptr<Asset<kry::Audio::Buffer>>>& assets)
{
	try
	{
		for (auto& asset : assets)
		{

			Resource<kry::Audio::Buffer>* resource = createDefaultSound(asset->properties["global"]["resource"]);
			sounds.emplace_back(resource);
			asset->resource = resource;
		}
	}
	catch (const kry::Util::Exception& e)
	{
		QMessageBox::information(nullptr, "Sound Loading Error", e.what(), QMessageBox::Ok);
	}
}

void Resources::loadAndAssignMusic(std::vector<std::shared_ptr<Asset<kry::Audio::Source>>>& assets)
{
	try
	{
		for (auto& asset : assets)
		{
			Resource<kry::Audio::Source>* resource = createDefaultMusic(asset->properties["global"]["resource"]);
			music.emplace_back(resource);
			asset->resource = resource;
		}
	}
	catch (const kry::Util::Exception& e)
	{
		QMessageBox::information(nullptr, "Music Loading Error", e.what(), QMessageBox::Ok);
	}
}

void Resources::reassignAnimations(std::vector<std::shared_ptr<Asset<kry::Graphics::Texture>>>& assets)
{
	try
	{
		for (auto& asset : assets)
		{
			std::shared_ptr<Animation<>> animation;
			auto path = kryToQString(asset->properties["global"]["resource"]).replace('\\', '/');
			for (auto anim : animations) // look for an animation for it
			{
				if (path == anim->path)
				{
					animation = anim;
					break;
				}
			}
			if (!animation) // if there was none, create a new one for it
			{
				Animation<>* animation = Animation<>::createDefaultAnimation(asset->properties["global"]["resource"]);
				auto dims = animation->rawresource->getDimensions();
				for (auto& properties : animation->properties)
				{
					auto px = kry::Util::toDecimal<float>(asset->properties["global"]["relativex"]);
					auto py = kry::Util::toDecimal<float>(asset->properties["global"]["relativey"]);
					properties[properties["Skins"]["name"]]["framePivot"] = (kry::Util::Vector2f(px, py) * dims).toString();
				}

				animations.emplace_back(animation);
				asset->resource = animation;
			}
			else // otherwise, set it and move on
				asset->resource = animation.get();
		}
	}
	catch (const kry::Util::Exception& e)
	{
		QMessageBox::information(nullptr, "Animation Re-Assign Error", e.what(), QMessageBox::Ok);
	}
}

void Resources::reassignSounds(std::vector<std::shared_ptr<Asset<kry::Audio::Buffer>>>& assets)
{
	try
	{
		for (auto& asset : assets)
		{
			Resource<kry::Audio::Buffer>* resource;
			for (auto sound : sounds) // look for a sound for it
			{
				if (asset->path == sound->path)
				{
					resource = sound.get();
					break;
				}
			}
			if (resource == nullptr)
			{
				Resource<kry::Audio::Buffer>* resource = createDefaultSound(asset->properties["global"]["resource"]);
				sounds.emplace_back(resource);
				asset->resource = resource;
			}
			else
				asset->resource = resource;
		}
	}
	catch (const kry::Util::Exception& e)
	{
		QMessageBox::information(nullptr, "Sound Re-Assign Error", e.what(), QMessageBox::Ok);
	}
}

void Resources::reassignMusic(std::vector<std::shared_ptr<Asset<kry::Audio::Source>>>& assets)
{
	try
	{
		for (auto& asset : assets)
		{
			Resource<kry::Audio::Source>* resource;
			for (auto song : music) // look for a song for it
			{
				if (asset->path == song->path)
				{
					resource = song.get();
					break;
				}
			}
			if (resource == nullptr)
			{
				Resource<kry::Audio::Source>* resource = createDefaultMusic(asset->properties["global"]["resource"]);
				music.emplace_back(resource);
				asset->resource = resource;
			}
			else
				asset->resource = resource;
		}
	}
	catch (const kry::Util::Exception& e)
	{
		QMessageBox::information(nullptr, "Music Re-Assign Error", e.what(), QMessageBox::Ok);
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
		
		Resource<kry::Graphics::Texture>* group = new Resource<kry::Graphics::Texture>;
		group->path = QString("editor\\group.png");
		group->name = QString("group.png");
		group->type = ResourceType::TEXTURE;
		group->rawresource = new kry::Graphics::Texture(kry::Media::imageFileToTexture("editor\\group.png", Configuration::getConfig()["editor"]["mipmapping"] == "true"));

		editortextures.emplace_back(missing);
		editortextures.emplace_back(flag_red);
		editortextures.emplace_back(flag_green);
		editortextures.emplace_back(group);
	}
	catch (const kry::Util::Exception& e)
	{
		QMessageBox::information(nullptr, "Editor Loading Error", e.what(), QMessageBox::Ok);
	}
}

Resources::Resources()
{
}
