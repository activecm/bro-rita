#include <sstream>
#include <iomanip>
#include <unistd.h>
#include <mongocxx/exception/bulk_write_exception.hpp>
#include "MongoDBWriterBackend.h"
#include "DocBuilder.h"
#include "writers/BufferedMongoDBWriter.h"
#include "writers/RotatedBufferedMongoDBWriter.h"

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::open_document;


using namespace logging;
using namespace threading;
using namespace plugin::ActiveCM_MongoDBWriter;
using namespace threading;
using namespace formatter;

/** MongoDBWriterBackend Constructor, initiates super and the ascii formatter.
 *
 * Even though this is the constructor, the majority of the initiation is done 
 * in the DoInit() function, which is called by Bro lazily, before first call 
 * to DoWrite().
 */
MongoDBWriterBackend::MongoDBWriterBackend(WriterFrontend *frontend) :
        WriterBackend(frontend),
        formatter(new formatter::Ascii(this, formatter::Ascii::SeparatorInfo())) {
}

/** MongoDBWriterBackend Destructor.
 *
 * Deletes class and ascii formatter
 */
MongoDBWriterBackend::~MongoDBWriterBackend() {
    delete this->formatter;
}

/** Sets up the Database, Security and Rotation
 *
 * Performs the majority of the initialization including:
 * - Connecting to the MongoDB database.
 * - Choosing which collection to write to.
 * - Setting up Authentiction types and files.
 * - Setting up Encryption.
 * - Setting when/how to rotate database exports.
 *
 * All data that this function works on is contained in the WriterInfo class.
 * The WriterInfo argument is populated in the initiating broscript which can be
 * found in <plugin-base dir>/scripts/rita.bro .
 *
 * \param[in] info        Contains configuration variables from rita.bro
 * \param[in] num_fields  Not Used
 * \param[in] fields      Not Used
 *
 * \return True: if all expected variables were present
 */
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

        string pemFile = LookupParam(info, "clientCert");
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


    if (rotate == "true" || rotate == "t") {
        this->writer = make_unique<RotatedBufferedMongoDBWriter>(client, selectedDB, logCollection);
    } else {
        this->writer = make_unique<BufferedMongoDBWriter>(client, selectedDB, logCollection);
    }

    return this->writer->Init();

}

/** Helper function that finds a parameter in a WriterInfo instance by its name.
 *
 * Performs linear search of a given info class for a parameter with name name.
 *
 * \param[in] info        Contains configuration variables from rita.bro
 * \param[in] name        Parameter name to search for.
 *
 * \return Value of found field if parameter was present, and empty string otherwise
 */
std::string MongoDBWriterBackend::LookupParam(const WriterInfo &info, const std::string& name) const {
    auto it = info.config.find(name.c_str());
    if (it == info.config.end())
        return std::string();
    else
        return it->second;
}

/** Perform record write to database.
 *
 * Passes each field through the document builder for conversion from Brotypes to
 * bsoncxx types. The values are then added to the document to write.
 *
 *
 */
bool MongoDBWriterBackend::DoWrite(int num_fields, const Field *const *fields, Value **vals) {
    auto builder = DocBuilder(this->formatter);

    for (int i = 0; i < num_fields; i++) {
        builder.addField(fields[i], vals[i]);
    }

    return this->writer->Write(builder.finalize());
}

/** Unused, Buffering is always set
 *
 */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
bool MongoDBWriterBackend::DoSetBuf(bool enabled) {
    //ignore. Always buffer
    return true;
}

/** Performs daily database rotation
 *
 * Called by Bro to change the collection Bro writes to programatically
 *
 */
bool MongoDBWriterBackend::DoRotate(const char *rotated_path, double open, double close, bool terminating) {
     if (!(this->writer->Rotate() && FinishedRotation("/dev/null", Info().path, open, close, terminating))) {
        Error(Fmt("error rotating %s", Info().path));
        return false;
    }

    return true;
}

/** Flush the Buffer
 *
 *
 */
bool MongoDBWriterBackend::DoFlush(double network_time) {
    return this->writer->Flush();
}

/** Finish writing by Flushing the Buffer
 *
 *
 */
bool MongoDBWriterBackend::DoFinish(double network_time) {
    return this->writer->Flush();
}

/** Maintain connection with Database (Not Used)
 *
 *
 */
bool MongoDBWriterBackend::DoHeartbeat(double network_time, double current_time) {
    return true;
}
#pragma clang diagnostic pop

