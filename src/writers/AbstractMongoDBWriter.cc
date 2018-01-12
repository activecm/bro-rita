//
// Created by logan on 1/11/18.
//

#include <mongocxx/exception/bulk_write_exception.hpp>
#include "AbstractMongoDBWriter.h"
#include "../Version.h"

using namespace plugin::OCMDev_MongoDBWriter;


AbstractMongoDBWriter::AbstractMongoDBWriter(std::shared_ptr<const mongocxx::client> client) :
        client{std::move(client)} {

}

bool AbstractMongoDBWriter::Init() {
    return true;
}

bool AbstractMongoDBWriter::Rotate() {
    return true;
}


bool AbstractMongoDBWriter::CreateMetaEntry(const std::string& targetDB, const std::string& targetCollection) {
    mongocxx::collection coll = (*this->client)["MetaDatabase"]["databases"];

    // new MetaDB entry
    auto builder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value docValue = builder
            << "name" << targetDB
            << "analyzed" << bsoncxx::types::b_bool{false}
            << "dates" << bsoncxx::types::b_bool{false}
            << "version" << std::string("v") + std::to_string(PLUGIN_MAJOR) + "." + std::to_string(PLUGIN_MINOR) + "-" + PLUGIN_NAME
            << bsoncxx::builder::stream::finalize;

    try {
        //assume this is a new database
        coll.insert_one( docValue.view() );
    } catch (const mongocxx::bulk_write_exception& ){

        //check if the database has already been analyzed
        auto queryBuilder = bsoncxx::builder::stream::document{};
        bsoncxx::document::value queryValue = queryBuilder
                << "name" << targetDB
                << bsoncxx::builder::stream::finalize;
        auto metaDBDoc = coll.find_one(queryValue.view());

        if (!metaDBDoc || metaDBDoc.value().view()["analyzed"].get_bool()) {
            //if we couldn't query for the existing record, or the db is already analyzed, error out
            return false;
        }
    }

    return true;
}
