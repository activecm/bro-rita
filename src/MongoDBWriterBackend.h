#ifndef MONGODB_WRITER_MONGO_WRITER_H
#define MONGODB_WRITER_MONGO_WRITER_H

#include <logging/WriterBackend.h>
#include <threading/formatters/Ascii.h>
#include <mongocxx/client.hpp>
#include <array>
#include <mongocxx/instance.hpp>
#include "writers/AbstractMongoDBWriter.h"

namespace plugin {

    namespace ActiveCM_MongoDBWriter {

      /** MongoDB Bro writer interface class
       * 
       * Inherits from Bro's internal Writer class,
       * Attaches itself to an output stream to push packet data to a MongoDB 
       * database. 
       */
        class MongoDBWriterBackend : public logging::WriterBackend {

        public:
            explicit MongoDBWriterBackend(logging::WriterFrontend *frontend);

            ~MongoDBWriterBackend() override;

            static logging::WriterBackend *Instantiate(logging::WriterFrontend *frontend) {
                return new MongoDBWriterBackend(frontend);
            }

        protected:
            bool DoInit(const WriterInfo &info, int num_fields,
                        const threading::Field *const *fields) override;

            bool DoWrite(int num_fields, const threading::Field *const *fields, threading::Value **vals) override;

            bool DoSetBuf(bool enabled) override;

            bool DoRotate(const char *rotated_path, double open, double close, bool terminating) override;

            bool DoFlush(double network_time) override;

            bool DoFinish(double network_time) override;

            bool DoHeartbeat(double network_time, double current_time) override;


        private:
            const threading::formatter::Ascii *const formatter;
            shared_ptr<const mongocxx::client> client;
            unique_ptr<AbstractMongoDBWriter> writer;

            std::string LookupParam(const WriterInfo& info, const std::string& name) const;
        };

    }
}

#endif
