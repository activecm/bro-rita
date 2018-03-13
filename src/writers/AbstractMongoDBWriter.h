//
// Created by logan on 1/11/18.
//

#ifndef BROPLUGINMONGODBWRITER_ABSTRACTMONGODBWRITER_H
#define BROPLUGINMONGODBWRITER_ABSTRACTMONGODBWRITER_H

#include <bsoncxx/document/value.hpp>
#include <mongocxx/client.hpp>
#include <memory>
namespace plugin {
    namespace ActiveCM_MongoDBWriter {
        class AbstractMongoDBWriter {
        public:
            explicit AbstractMongoDBWriter(std::shared_ptr<const mongocxx::client>);

            virtual bool Init();

            virtual bool Write(bsoncxx::document::value document) = 0;

            virtual bool Flush() = 0;

            virtual bool Rotate();

        protected:
            bool CreateMetaEntry(const std::string &targetDB);

            bool IndexLogCollection(const std::string& targetDB, const std::string & targetCollection);
            std::shared_ptr<const mongocxx::client> client;
        };
    }
}

#endif //BROPLUGINMONGODBWRITER_ABSTRACTMONGODBWRITER_H
