#include "MongoWriter.h"

using namespace logging::writer;

MongoDB::MongoDB(WriterFrontend* frontend){
}

MongoDB::~MongoDB(){
}

bool MongoDB::DoInit(const WriterInfo& info, int num_fields, const threading::Field* const* fields) {
    return true;
}

bool MongoDB::DoWrite(int num_fields, const threading::Field* const* fields, threading::Value** vals){
    return true;
}

bool MongoDB::DoSetBuf(bool enabled){
    return true;
}

bool MongoDB::DoRotate(const char* rotated_path, double open, double close, bool terminating){
    FinishedRotation();
    return true;
}

bool MongoDB::DoFlush(double network_time){
    return true;
}

bool MongoDB::DoFinish(double network_time){
    return true;
}

bool MongoDB::DoHeartbeat(double network_time, double current_time){
    return true;
}

}
