#ifndef RESOURCES_H
#define RESOURCES_H

#include "Assets.h"
#include <Graphics/Texture.h>
#include <Audio/Buffer.h>
#include <Audio/Source.h>
#include <QString>
#include <vector>
#include <memory>

enum class EditorResource : unsigned
{
	MISSING_TILE,
	FLAG
};

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
struct Animation : public Resource<ResType>
{
	static const unsigned char MAX_DIRECTION_COUNT = 8;
	kry::Media::Config properties[MAX_DIRECTION_COUNT];

	static Animation<ResType>* createDefaultAnimation(const kry::Util::String& imagefile, const kry::Util::String& name = kry::Util::String());
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
		static void initEditorTextures();
		inline static const std::vector<std::shared_ptr<Resource<kry::Graphics::Texture> > >& getEditorTextures();
		inline static std::shared_ptr<Resource<kry::Graphics::Texture> > getEditorTexture(EditorResource texture);

	private:
		Resources();

		static std::vector<std::shared_ptr<Animation<> > > animations;
        static std::vector<std::shared_ptr<Resource<kry::Graphics::Texture> > > textures;
        static std::vector<std::shared_ptr<Resource<kry::Audio::Buffer> > > sounds;
        static std::vector<std::shared_ptr<Resource<kry::Audio::Source> > > music;
		static std::vector<std::shared_ptr<Resource<kry::Graphics::Texture> > > editortextures;
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

const std::vector<std::shared_ptr<Resource<kry::Graphics::Texture> > >& Resources::getEditorTextures()
{
	return editortextures;
}

std::shared_ptr<Resource<kry::Graphics::Texture> > Resources::getEditorTexture(EditorResource texture)
{
	return editortextures[unsigned(texture)];
}

#endif // ASSETS_H
