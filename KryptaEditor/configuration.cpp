#include "Configuration.h"
#include <Utilities/BasicString.h>
#include <System/Filesystem.h>

kry::Media::Config Configuration::config;

kry::Media::Config& Configuration::loadFromFile(const kry::Util::String& filename)
{
    config.fileToConfig(filename);
    return config;
}

void Configuration::saveToFile(const kry::Util::String& filename)
{
    std::vector<kry::Util::String> lines;
    for (auto& section : config.getSectionNames())
    {
		if (section.isEmpty())
			continue;
        lines.push_back("[" + section + "]");
        for (auto& key : config[section].getKeyNames())
            lines.push_back(key + "=" + config[section][key]);
    }

    kry::System::writeAllLines(filename, lines);
}

Configuration::Configuration()
{
}
