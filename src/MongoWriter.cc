#include "MongoWriter.h"
#include "bro_mongodb.bif.h"
using namespace logging;
using namespace writer;

MongoDB::MongoDB(WriterFrontend* frontend) : WriterBackend(frontend)
{
}

MongoDB::~MongoDB(){
}

bool MongoDB::DoInit(const WriterInfo& info, int num_fields, const threading::Field* const* fields) {
    return true;
}

bool MongoDB::DoWrite(int num_fields, const threading::Field* const* fields, threading::Value** vals){
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
