#ifndef MAPPROJECT_H
#define MAPPROJECT_H

#include <Utilities/String.h>
#include <memory>

class MapProject
{
    public:
        //MapProject* createMapProject();

    private:
        MapProject();

        kry::Util::String name;

        //static std::unique_ptr<MapProject> single;
};

#endif // MAPPROJECT_H
