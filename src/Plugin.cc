
#include "Plugin.h"

namespace plugin { namespace OCMDev_bro_mongodb { Plugin plugin; } }

using namespace plugin::OCMDev_bro_mongodb;

plugin::Configuration Plugin::Configure()
	{
	plugin::Configuration config;
	config.name = "OCMDev::bro_mongodb";
	config.description = "<Insert description>";
	config.version.major = 0;
	config.version.minor = 1;
	return config;
	}
