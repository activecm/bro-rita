//
// Created by logan on 1/11/18.
//

#ifndef BROPLUGINMONGODBWRITER_ROTATEDBUFFEREDMONGODBWRITER_H
#define BROPLUGINMONGODBWRITER_ROTATEDBUFFEREDMONGODBWRITER_H


#include <mongocxx/client.hpp>
#include "BufferedMongoDBWriter.h"
namespace plugin {
    namespace OCMDev_MongoDBWriter {
        class RotatedBufferedMongoDBWriter : public BufferedMongoDBWriter {
        public:
            explicit RotatedBufferedMongoDBWriter(const std::shared_ptr<const mongocxx::client> &client,
                                                  std::string _targetDBBase,
                                                  std::string logCollection);

            bool Rotate() override;

        private:
            std::string GenRotatedDBName(const std::string& targetDBBase);

            std::string targetDBBase;
        };
    }
}

#endif //BROPLUGINMONGODBWRITER_ROTATEDBUFFEREDMONGODBWRITER_H
