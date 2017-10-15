
#ifndef BRO_PLUGIN_OCMDEV_BRO_MONGODB
#define BRO_PLUGIN_OCMDEV_BRO_MONGODB

#include <plugin/Plugin.h>

namespace plugin {
namespace OCMDev_bro_mongodb {

class Plugin : public ::plugin::Plugin
{
protected:
	// Overridden from plugin::Plugin.
	plugin::Configuration Configure() override;
};

extern Plugin plugin;

}
}

#endif
