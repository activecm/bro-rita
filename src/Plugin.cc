
#include "Plugin.h"
#include "MongoDBWriterBackend.h"
#include "Version.h"

namespace plugin { namespace ActiveCM_MongoDBWriter { Plugin plugin; }}

using namespace plugin::ActiveCM_MongoDBWriter;


plugin::Configuration Plugin::Configure() {
    AddComponent(new ::logging::Component("RITA", MongoDBWriterBackend::Instantiate));
    plugin::Configuration config;
    config.name = PLUGIN_NAME;
    config.description = "RITA, Bro IDS connector.";
    config.version.major = PLUGIN_MAJOR;
    config.version.minor = PLUGIN_MINOR;
    return config;
}
