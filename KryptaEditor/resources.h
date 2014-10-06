#ifndef RESOURCES_H
#define RESOURCES_H

#include "Assets.h"
#include <Graphics/Texture.h>
#include <Audio/Buffer.h>
#include <Audio/Source.h>
#include <QString>
#include <vector>
#include <memory>

enum class ResourceType : unsigned char
{
	ANIMATION,
    TEXTURE,
    SOUND,
    MUSIC
};

template <typename ResType>
struct Resource
{
    QString name;
    QString path;
    ResType* rawresource;
    ResourceType type;

	inline virtual ~Resource();
};

template <typename ResType = kry::Graphics::Texture>
struct Animation : public Resource<ResType> // create an anim type, fill a vector with anims from a new folder,
{
	kry::Media::Config properties;
};

class Resources
{
    public:
        static void loadResources(const QString& rootdir);
		static void loadAndAssignAnimations(std::vector<std::shared_ptr<Asset<kry::Graphics::Texture> > >& assets);
		static void loadAndAssignTextures(std::vector<std::shared_ptr<Asset<kry::Graphics::Texture> > >& assets); /** #TODO(incomplete) add loadSounds/Music as well */
		inline static std::vector<std::shared_ptr<Animation<> > >& getAnimations();
		inline static const std::vector<std::shared_ptr<Resource<kry::Graphics::Texture> > >& getTextures(); /** #TODO(note) should this be replaced with Anim's? */
        inline static const std::vector<std::shared_ptr<Resource<kry::Audio::Buffer> > >& getSounds();
        inline static const std::vector<std::shared_ptr<Resource<kry::Audio::Source> > >& getMusic();

    private:
        Resources();

		static std::vector<std::shared_ptr<Animation<> > > animations;
        static std::vector<std::shared_ptr<Resource<kry::Graphics::Texture> > > textures;
        static std::vector<std::shared_ptr<Resource<kry::Audio::Buffer> > > sounds;
        static std::vector<std::shared_ptr<Resource<kry::Audio::Source> > > music;
};


template <typename ResType>
Resource<ResType>::~Resource()
{
    delete rawresource;
}

std::vector<std::shared_ptr<Animation<> > >& Resources::getAnimations()
{
	return animations;
}

const std::vector<std::shared_ptr<Resource<kry::Graphics::Texture> > >& Resources::getTextures()
{
    return textures;
}

const std::vector<std::shared_ptr<Resource<kry::Audio::Buffer> > >& Resources::getSounds()
{
    return sounds;
}

const std::vector<std::shared_ptr<Resource<kry::Audio::Source> > >& Resources::getMusic()
{
    return music;
}

#endif // ASSETS_H
