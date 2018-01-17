//
// Created by logan on 1/11/18.
//

#include "DailyBufferedMongoDBWriter.h"
#include <iomanip>
using namespace plugin::OCMDev_MongoDBWriter;

DailyBufferedMongoDBWriter::DailyBufferedMongoDBWriter(
        const std::shared_ptr<const mongocxx::client> &client, std::string selectedDBBase, std::string logCollection) :
AbstractMongoDBWriter{client}, targetDBBase{std::move(selectedDBBase)}, logCollection{std::move(logCollection)}
{
}

bool DailyBufferedMongoDBWriter::Write(bsoncxx::document::value document) {
    int64_t day = static_cast<int64_t>(document.view()["ts"].get_double().value);
    day = day - (day % 86400);
    MongoDBBuffer* buffer = nullptr;

    auto bufferIt = this->buffers.find(day);
    if (bufferIt != this->buffers.end()) {
        buffer = &bufferIt->second;
    } else {
        this->buffers.emplace(day, MongoDBBuffer(
                this->GenDailyDBName(this->targetDBBase, (time_t)day),
                this->logCollection
        ));
        buffer = &this->buffers.at(day);
        if (!(CreateMetaEntry(buffer->targetDB) &&
              this->IndexLogCollection(buffer->targetDB, buffer->targetCollection))) {
            return false;
        }
    }

    if (buffer->Full() && !buffer->Flush(*this->client)) {
        return false;
    }
    buffer->Write(std::move(document));
    return true;
}

bool DailyBufferedMongoDBWriter::Flush() {
    for (std::pair<int64_t, MongoDBBuffer> bufferPair : this->buffers) {
        if (!bufferPair.second.Flush(*this->client)) {
            return false;
        }
    }
    return true;
}

std::string DailyBufferedMongoDBWriter::GenDailyDBName(const std::string &targetDBBase, time_t dayTS) {
    auto tm = std::gmtime(&dayTS);
    std::ostringstream oss;
    oss << targetDBBase << std::put_time(tm, "-%Y-%m-%d-%H-%M");
    return oss.str();
}
