#include <sstream>
#include <iomanip>
#include <unistd.h>
#include <mongocxx/exception/bulk_write_exception.hpp>
#include "MongoDBWriterBackend.h"
#include "DocBuilder.h"
#include "writers/BufferedMongoDBWriter.h"
#include "writers/RotatedBufferedMongoDBWriter.h"
#include "writers/DailyBufferedMongoDBWriter.h"

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::open_document;


using namespace logging;
using namespace threading;
using namespace plugin::OCMDev_MongoDBWriter;
using namespace threading;
using namespace formatter;

MongoDBWriterBackend::MongoDBWriterBackend(WriterFrontend *frontend) :
        WriterBackend(frontend),
        formatter(new formatter::Ascii(this, formatter::Ascii::SeparatorInfo())) {
}

MongoDBWriterBackend::~MongoDBWriterBackend() {
    delete this->formatter;
}

bool MongoDBWriterBackend::DoInit(const WriterInfo &info, int num_fields,
                         const Field *const *fields) {
    //URI, DB, COLLECTION
    mongocxx::instance& instance = mongocxx::instance::current();

    std::string uriInfo = LookupParam(info, "uri");
    if (uriInfo.empty()) {
        return false;
    }

    mongocxx::uri uri(uriInfo);

    std::string selectedDB = LookupParam(info, "selectedDB");
    if (selectedDB.empty()) {
        return false;
    }

    std::string logCollection = info.path;


    //SECURITY
    if (uri.ssl()) {
        mongocxx::options::client client_opts{};
        mongocxx::options::ssl ssl_opts{};

        string CAFile = LookupParam(info, "CAFile");
        if (!CAFile.empty()) {
            if (access(CAFile.c_str(), F_OK) == 0) {
                ssl_opts.ca_file(CAFile);
                ssl_opts.allow_invalid_certificates(false);
            } else {
                return false;
            }
        }

        string verifyCert = LookupParam(info, "verifyCert");
        if (verifyCert == "false") {
            ssl_opts.allow_invalid_certificates(true);
        }

        string pemFile = LookupParam(info, "x509ClientCert");
        if (!pemFile.empty()) {
            if (access(pemFile.c_str(), F_OK) == 0) {
                ssl_opts.pem_file(pemFile);
                ssl_opts.allow_invalid_certificates(false);
            } else {
                return false;
            }
        }

        client_opts.ssl_opts(ssl_opts);
        this->client = make_shared<const mongocxx::client>(uri, client_opts);
    } else {
        this->client = make_shared<const mongocxx::client>(uri);
    }


    //LOG ORGANIZATION
    std::string rotate = LookupParam(info, "rotate");
    std::transform(rotate.begin(), rotate.end(), rotate.begin(), ::tolower);

    std::string splitOnDay = LookupParam(info, "splitByDate");
    std::transform(splitOnDay.begin(), splitOnDay.end(), splitOnDay.begin(), ::tolower);

    if (rotate == "true" || rotate == "t") {
        this->writer = make_unique<RotatedBufferedMongoDBWriter>(client, selectedDB, logCollection);
    } else if (splitOnDay == "true" || splitOnDay == "t") {
        this->writer = make_unique<DailyBufferedMongoDBWriter>(client, selectedDB, logCollection);
    } else {
        this->writer = make_unique<BufferedMongoDBWriter>(client, selectedDB, logCollection);
    }

    return this->writer->Init();

}

std::string MongoDBWriterBackend::LookupParam(const WriterInfo &info, const std::string& name) const {
    auto it = info.config.find(name.c_str());
    if (it == info.config.end())
        return std::string();
    else
        return it->second;
}

bool MongoDBWriterBackend::DoWrite(int num_fields, const Field *const *fields, Value **vals) {
    auto builder = DocBuilder(this->formatter);

    for (int i = 0; i < num_fields; i++) {
        builder.addField(fields[i], vals[i]);
    }

    return this->writer->Write(builder.finalize());
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
bool MongoDBWriterBackend::DoSetBuf(bool enabled) {
    //ignore. Always buffer
    return true;
}

bool MongoDBWriterBackend::DoRotate(const char *rotated_path, double open, double close, bool terminating) {
     if (!(this->writer->Rotate() && FinishedRotation("/dev/null", Info().path, open, close, terminating))) {
        Error(Fmt("error rotating %s", Info().path));
        return false;
    }

    return true;
}

bool MongoDBWriterBackend::DoFlush(double network_time) {
    return this->writer->Flush();
}

bool MongoDBWriterBackend::DoFinish(double network_time) {
    return this->writer->Flush();
}

bool MongoDBWriterBackend::DoHeartbeat(double network_time, double current_time) {
    return true;
}
#pragma clang diagnostic pop

