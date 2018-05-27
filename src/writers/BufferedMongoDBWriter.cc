//
// Created by logan on 1/11/18.
//

#include <mongocxx/result/insert_many.hpp>
#include "BufferedMongoDBWriter.h"

using namespace plugin::ActiveCM_RITAWriter;


BufferedMongoDBWriter::BufferedMongoDBWriter(const std::shared_ptr<const mongocxx::client>& client,
                                             std::string targetDB,
                                             std::string logCollection) :
    AbstractMongoDBWriter{client}, buffer{std::move(targetDB), std::move(logCollection)} {
}

bool BufferedMongoDBWriter::Init() {
    if ( !this->CreateMetaEntry(this->buffer.targetDB) )
    {
      InternalWarning("Unable to Create MetaDatabase entry.");
      return false;
    }

    if ( !this->IndexLogCollection(this->buffer.targetDB, this->buffer.targetCollection) )
    {
      InternalWarning("Unable to index selected Database.");
      return false;
    }

    return true;
}

  /* Checks the writers buffer and if full, write to the database
   *
   * Buffered writing is more efficent over a network
   *
  */
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
