
#include "Plugin.h"
#include "MongoWriter.h"

namespace plugin { namespace OCMDev_bro_mongodb { Plugin plugin; } }

using namespace plugin::OCMDev_bro_mongodb;

plugin::Configuration Plugin::Configure() {
    AddComponent(new logging::Component("MongoDB", logging::writer::MongoDB::Instantiate));
    plugin::Configuration config;
    config.name = "OCMDev::bro_mongodb";
    config.description = "Writes bro logs to MongoDB";
    config.version.major = 0;
    config.version.minor = 1;
    return config;
}
