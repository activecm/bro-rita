
#include "Plugin.h"
#include "MongoWriter.h"

namespace plugin { namespace OCMDev_MongoDBWriter { Plugin plugin; }}

using namespace plugin::OCMDev_MongoDBWriter;


plugin::Configuration Plugin::Configure() {
    AddComponent(new ::logging::Component("MongoDB", MongoWriter::Instantiate));
    plugin::Configuration config;
    config.name = PLUGIN_NAME;
    config.description = "Writes bro logs to MongoDB";
    config.version.major = PLUGIN_MAJOR;
    config.version.minor = PLUGIN_MINOR;
    return config;
}
