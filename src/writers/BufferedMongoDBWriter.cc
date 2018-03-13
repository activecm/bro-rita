//
// Created by logan on 1/11/18.
//

#include <mongocxx/result/insert_many.hpp>
#include "BufferedMongoDBWriter.h"

using namespace plugin::ActiveCM_MongoDBWriter;


BufferedMongoDBWriter::BufferedMongoDBWriter(const std::shared_ptr<const mongocxx::client>& client,
                                             std::string targetDB,
                                             std::string logCollection) :
    AbstractMongoDBWriter{client}, buffer{std::move(targetDB), std::move(logCollection)} {
}

bool BufferedMongoDBWriter::Init() {
    return this->CreateMetaEntry(this->buffer.targetDB) &&
            this->IndexLogCollection(this->buffer.targetDB, this->buffer.targetCollection);
}

bool BufferedMongoDBWriter::Write(bsoncxx::document::value document) {
    if (buffer.Full() && !this->buffer.Flush(*this->client)) {
        return false;
    }

    this->buffer.Write(std::move(document));
    return true;
}

bool BufferedMongoDBWriter::Flush() {
    return this->buffer.Flush(*this->client);
}
