#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <Media/ConfigFactory.h>

class Configuration
{
    public:
        static kry::Media::Config& loadFromFile(const kry::Util::String& filename);
        static void saveToFile(const kry::Util::String& filename);
        static kry::Media::Config& getConfig();

    private:
        Configuration();

        static kry::Media::Config config;
};

#endif // CONFIGURATION_H
