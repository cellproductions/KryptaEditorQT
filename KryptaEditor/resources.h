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
	MISSING_TILE = 0,
	FLAG_RED,
	FLAG_GREEN,
	GROUP
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
	std::vector<kry::Media::Config> frames[MAX_DIRECTION_COUNT];

	static Animation<ResType>* createDefaultAnimation(const kry::Util::String& imagefile, const kry::Util::String& name = kry::Util::String());
};

Resource<kry::Audio::Buffer>* createDefaultSound(const kry::Util::String& soundfile);
Resource<kry::Audio::Source>* createDefaultMusic(const kry::Util::String& musicfile);

class Resources
{
    public:
		static void loadAndAssignAnimations(std::vector<std::shared_ptr<Asset<kry::Graphics::Texture>>>& assets);
		static void loadAndAssignSounds(std::vector<std::shared_ptr<Asset<kry::Audio::Buffer>>>& assets);
		static void loadAndAssignMusic(std::vector<std::shared_ptr<Asset<kry::Audio::Source>>>& assets);
		static void reassignAnimations(std::vector<std::shared_ptr<Asset<kry::Graphics::Texture>>>& assets);
		static void reassignSounds(std::vector<std::shared_ptr<Asset<kry::Audio::Buffer>>>& assets);
		static void reassignMusic(std::vector<std::shared_ptr<Asset<kry::Audio::Source>>>& assets);
		inline static std::vector<std::shared_ptr<Animation<>>>& getAnimations();
		inline static const std::vector<std::shared_ptr<Resource<kry::Graphics::Texture>>>& getTextures(); /** #TODO(note) should this be replaced with Anim's? */
        inline static const std::vector<std::shared_ptr<Resource<kry::Audio::Buffer>>>& getSounds();
        inline static const std::vector<std::shared_ptr<Resource<kry::Audio::Source>>>& getMusic();
		inline static size_t getAnimationIndex(std::shared_ptr<Animation<>> animation);
		static void initEditorTextures();
		inline static const std::vector<std::shared_ptr<Resource<kry::Graphics::Texture>>>& getEditorTextures();
		inline static std::shared_ptr<Resource<kry::Graphics::Texture>> getEditorTexture(EditorResource texture);
		inline static void eraseAll();

	private:
		Resources();

		static std::vector<std::shared_ptr<Animation<>>> animations;
        static std::vector<std::shared_ptr<Resource<kry::Audio::Buffer>>> sounds;
        static std::vector<std::shared_ptr<Resource<kry::Audio::Source>>> music;
		static std::vector<std::shared_ptr<Resource<kry::Graphics::Texture>>> editortextures;
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
	return editortextures[static_cast<std::underlying_type<EditorResource>::type>(texture)];
}

size_t Resources::getAnimationIndex(std::shared_ptr<Animation<>> animation)
{
	size_t index = 0;
	for (auto& anim : animations)
	{
		if (anim == animation)
			return index;
		++index;
	}
	assert(index != 0);
	return index;
}

void Resources::eraseAll()
{
	animations.clear();
	sounds.clear();
	music.clear();
}

#endif // ASSETS_H
