
#ifndef MONGODB_WRITER_PLUGIN_H
#define MONGODB_WRITER_PLUGIN_H

#include <plugin/Plugin.h>

namespace plugin {
    namespace OCMDev_MongoDBWriter {

        class Plugin : public ::plugin::Plugin {
        protected:
            // Overridden from plugin::Plugin.
            plugin::Configuration Configure() override;
        };

        extern Plugin plugin;

    }
}

#endif
