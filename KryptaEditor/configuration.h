#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <Media/ConfigFactory.h>

class Configuration
{
    public:
        static kry::Media::Config& loadFromFile(const kry::Util::String& filename);
        static void saveToFile(const kry::Util::String& filename);
		inline static kry::Media::Config& getConfig();
		inline static void updateConfig(const kry::Media::Config& config);

    private:
        Configuration();

        static kry::Media::Config config;
};



kry::Media::Config& Configuration::getConfig()
{
	return config;
}

void Configuration::updateConfig(const kry::Media::Config& config)
{
	this->config = config;
}

#endif // CONFIGURATION_H
