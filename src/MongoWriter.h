#ifndef LOGGING_WRITER_MONGO_H
#define LOGGING_WRITER_MONGO_H

#include <logging/WriterBackend.h>
#include "bro_mongodb.bif.h"
#include <algorithm>

namespace logging { namespace writer {

class MongoDB : public WriterBackend {

public:
    explicit MongoDB(WriterFrontend* frontend) : WriterBackend(frontend) {}

    static WriterBackend* Instantiate(WriterFrontend* frontend) {
        return new MongoDB(frontend);
    }

protected:
    bool DoInit(const WriterInfo& info, int num_fields,
                        const threading::Field* const* fields) override;

    bool DoWrite(int num_fields, const threading::Field* const* fields, threading::Value** vals) override;

    bool DoSetBuf(bool enabled) override;

    bool DoRotate(const char* rotated_path, double open, double close, bool terminating) override;

    bool DoFlush(double network_time) override;

    bool DoFinish(double network_time) override;

    bool DoHeartbeat(double network_time, double current_time) override;

};

}
}

#endif
