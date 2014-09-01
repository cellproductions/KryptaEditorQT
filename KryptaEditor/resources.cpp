#include "resources.h"
#include <System/Filesystem.h>
#include <Media/ImageFactory.h>
#include <Media/AudioFactory.h>
#include <QDebug>

std::vector<std::shared_ptr<Resource<kry::Graphics::Texture> > > Resources::textures;
std::vector<std::shared_ptr<Resource<kry::Audio::Buffer> > > Resources::sounds;
std::vector<std::shared_ptr<Resource<kry::Audio::Source> > > Resources::music;

void Resources::loadResources(const QString& rootdir)
{
    auto strrootdir = rootdir.toStdString();
    auto resources = strrootdir + "\\images";
    for (kry::Util::String& path : kry::System::getAllFiles(kry::Util::String(resources.c_str(), resources.length())))
    {
        if (path[path.getLength() - 1] == '.')
            continue;

        Resource<kry::Graphics::Texture>* resource = new Resource<kry::Graphics::Texture>;
        resource->type = ResourceType::TEXTURE;
        resource->rawresource = new kry::Graphics::Texture(std::move(kry::Media::pngFileToTexture(path)));
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

void Resources::loadAndAssignTextures(std::vector<std::shared_ptr<Asset<kry::Graphics::Texture> > >& assets)
{
    for (std::shared_ptr<Asset<kry::Graphics::Texture> >& asset : assets)
    {
        Resource<kry::Graphics::Texture>* resource = new Resource<kry::Graphics::Texture>;
        resource->type = ResourceType::TEXTURE;
        auto path = asset->properties["global"]["resource"];
        resource->rawresource = new kry::Graphics::Texture(std::move(kry::Media::pngFileToTexture(path)));
        resource->path = QString::fromStdString(std::string(path.getData(), path.getLength()));
        auto name = path.substring(path.lastIndexOf('\\') + 1);
        resource->name = QString::fromStdString(std::string(name.getData(), name.getLength()));

        textures.emplace_back(resource);
        asset->resource = resource;
    }
}

Resources::Resources()
{
}
