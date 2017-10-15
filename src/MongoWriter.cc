#include "MongoWriter.h"

namespace logging { namespace writer {


    MongoDB::MongoDB(WriterFrontend* frontend){

    }

    MongoDB~MongoDB(){

    }


    bool MongoDB::DoInit(const WriterInfo& info, int num_fields, const threading::Field* const* fields) {

    }
    bool MongoDB::DoWrite(int num_fields, const threading::Field* const* fields, threading::Value** vals){

    }
    bool MongoDB::DoSetBuf(bool enabled){

    }
    bool MongoDB::DoRotate(const char* rotated_path, double open, double close, bool terminating){

    }

    bool MongoDB::DoFlush(double network_time){

    }
    bool MongoDB::DoFinish(double network_time){

    }
    bool MongoDB::DoHeartbeat(double network_time, double current_time){

    }

}

}
