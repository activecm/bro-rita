//
// Created by logan on 1/11/18.
//

#include <iomanip>
#include "RotatedBufferedMongoDBWriter.h"
using namespace plugin::ActiveCM_MongoDBWriter;


RotatedBufferedMongoDBWriter::RotatedBufferedMongoDBWriter(const std::shared_ptr<const mongocxx::client>& client,
                                                           std::string _targetDBBase, std::string logCollection) :
        BufferedMongoDBWriter{client, GenRotatedDBName(_targetDBBase), std::move(logCollection)},
        targetDBBase{std::move(_targetDBBase)} {

}

bool RotatedBufferedMongoDBWriter::Rotate() {
    if(!Flush()) {
        return false;
    }
    this->buffer.targetDB = GenRotatedDBName(this->targetDBBase);
    return CreateMetaEntry(this->buffer.targetDB) &&
           this->IndexLogCollection(this->buffer.targetDB, this->buffer.targetCollection);
}

std::string RotatedBufferedMongoDBWriter::GenRotatedDBName(const std::string& targetDBBase) {
    auto t = std::time(nullptr);
    auto tm = std::gmtime(&t);
    std::ostringstream oss;
    oss << targetDBBase << std::put_time(tm, "-%Y-%m-%d-%H-%M");
    return oss.str();
}
