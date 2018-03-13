//
// Created by logan on 1/11/18.
//

#ifndef BROPLUGINMONGODBWRITER_BUFFEREDMONGODBWRITER_H
#define BROPLUGINMONGODBWRITER_BUFFEREDMONGODBWRITER_H

#include <mongocxx/client.hpp>
#include <mongocxx/options/insert.hpp>
#include "AbstractMongoDBWriter.h"
#include "MongoDBBuffer.h"

namespace plugin {
    namespace ActiveCM_MongoDBWriter {
        class BufferedMongoDBWriter : public AbstractMongoDBWriter {
        public:
            explicit BufferedMongoDBWriter(const std::shared_ptr<const mongocxx::client> &client,
                                           std::string targetDB,
                                           std::string logCollection);

            bool Init() override;

            bool Write(bsoncxx::document::value document) override;

            bool Flush() override;

        protected:
            MongoDBBuffer buffer;
        };
    }
}


#endif //BROPLUGINMONGODBWRITER_BUFFEREDMONGODBWRITER_H
