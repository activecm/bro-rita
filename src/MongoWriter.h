#ifndef MONGODB_WRITER_MONGO_WRITER_H
#define MONGODB_WRITER_MONGO_WRITER_H

#include <logging/WriterBackend.h>
#include <threading/formatters/Ascii.h>
#include <mongocxx/client.hpp>
#include <array>
#include <mongocxx/instance.hpp>
#include "MongoDBWriter.bif.h"

namespace plugin {
    namespace OCMDev_MongoDBWriter {

    const static int PLUGIN_MAJOR = 0;
    const static int PLUGIN_MINOR = 1;
    const static std::string PLUGIN_NAME = "OCMDev::MongoDBWriter";

        class MongoWriter : public logging::WriterBackend {

        public:
            explicit MongoWriter(logging::WriterFrontend *frontend);

            ~MongoWriter() override;

            static logging::WriterBackend *Instantiate(logging::WriterFrontend *frontend) {
                return new MongoWriter(frontend);
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
            const mongocxx::client * client;
            std::string selectedDBBase;
            std::string selectedDB;
            std::string logCollection;
            bool shouldRotate;

            static const unsigned long BUFFER_SIZE = 1000;
            std::vector<bsoncxx::document::value> buffer;
            mongocxx::options::insert insertOptions;

            bool SetConfig( const WriterInfo& info);
            std::string LookupParam(const WriterInfo& info, const std::string name) const;
            void RotateDBName();
        };

    }
}

#endif
