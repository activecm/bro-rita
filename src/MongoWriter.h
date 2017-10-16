#ifndef LOGGING_WRITER_MONGO_H
#define LOGGING_WRITER_MONGO_H

#include <logging/WriterBackend.h>
#include "bro_mongodb.bif.h"
#include <algorithm>

namespace logging { namespace writer {

class MongoDB : public WriterBackend {

public:
    MongoDB(WriterFrontend* frontend) : WriterBackend(frontend) {}
    ~MongoDB() {}

    static WriterBackend* Instantiate(WriterFrontend* frontend) {
        return new MongoDB(frontend);
    }

protected:
    virtual bool DoInit(const WriterInfo& info, int num_fields,
                        const threading::Field* const* fields) {
        if ( BifConst::LogMongo::debug ) {
            std::cout << "[logging::writer::None]" << std::endl;
            std::cout << "  path=" << info.path << std::endl;
            std::cout << "  rotation_interval=" << info.rotation_interval << std::endl;
            std::cout << "  rotation_base=" << info.rotation_base << std::endl;

            // Output the config sorted by keys.
            std::vector<std::pair<string, string> > keys;

            for ( WriterInfo::config_map::const_iterator i = info.config.begin(); i != info.config.end(); i++ )
                keys.push_back(std::make_pair(i->first, i->second));

            std::sort(keys.begin(), keys.end());

            for ( std::vector<std::pair<string,string> >::const_iterator i = keys.begin(); i != keys.end(); i++ )
                std::cout << "  config[" << (*i).first << "] = " << (*i).second << std::endl;

            for ( int i = 0; i < num_fields; i++ ) {
                const threading::Field* field = fields[i];
                std::cout << "  field " << field->name << ": "
                  << type_name(field->type) << std::endl;
            }

            std::cout << std::endl;
        }
        return true;
    }

    virtual bool DoWrite(int num_fields, const threading::Field* const* fields, threading::Value** vals) {
        for (int i = 0; i < num_fields; i++) {
            const threading::Value* value = vals[i];
            std::cout << fields[i]->name << ": ";
            if (!value->present) {
                std::cout << "-";
            } else {
                switch (value->type) {
                case TYPE_BOOL:
                case TYPE_INT:
                    std::cout << value->val.int_val; 
                    break;
                case TYPE_COUNT:
                case TYPE_COUNTER:
                    std::cout << value->val.uint_val;
                    break;
                case TYPE_TIME:
                case TYPE_INTERVAL:
                case TYPE_DOUBLE:
                    std::cout << value->val.double_val;
                    break;
                default:
                    std::cout << "?";
                }
            }
            std::cout << " ; ";
        }    
        std::cout << std::endl;
        return true;
    }

    virtual bool DoSetBuf(bool enabled) {
        return true;
    }

    virtual bool DoRotate(const char* rotated_path, double open, double close, bool terminating) {
        if ( ! FinishedRotation("/dev/null", Info().path, open, close, terminating)) {
            Error(Fmt("error rotating %s", Info().path));
            return false;
        }

        return true;
    }

    virtual bool DoFlush(double network_time) {
        return true;
    }

    virtual bool DoFinish(double network_time) {
        return true;
    }

    virtual bool DoHeartbeat(double network_time, double current_time) {
        return true;
    }

};

}
}

#endif
