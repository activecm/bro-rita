//
// Created by logan on 1/11/18.
//

#include "MongoDBBuffer.h"

using namespace plugin::ActiveCM_RITAWriter;

MongoDBBuffer::MongoDBBuffer(std::string targetDB, std::string targetCollection) :
        targetDB{std::move(targetDB)}, targetCollection{std::move(targetCollection)} {
    this->documents.reserve(this->BUFFER_SIZE);
    this->insertOptions.ordered(false);
}

bool MongoDBBuffer::Empty() {
    return this->documents.empty();
}

bool MongoDBBuffer::Full() {
    return this->documents.size() == this->BUFFER_SIZE;
}

  /** Write to the Managed buffer without writing to the Database
   *
   * Called by the Writer Backend class when doWrite is called by Bro.
   * This buffer is only owned by the Buffered Writer classes
   * Other writers just write to the database
   */
void MongoDBBuffer::Write(bsoncxx::document::value document) {
    this->documents.push_back(std::move(document));
}

  /** Writes the buffer to the database
   *
   */
bool MongoDBBuffer::Flush(const mongocxx::client &client) {
    if (this->Empty()) {
        return true;
    }
    mongocxx::collection coll = client[this->targetDB][this->targetCollection];
    bsoncxx::stdx::optional<mongocxx::result::insert_many> result =
            coll.insert_many(this->documents, this->insertOptions);
    if (!result) {
        reporter.InternalWarning("Buffered document flush failed.");
        return false;
    }
    this->documents.clear();
    return true;
}
