#include "MongoWriter.h"
#include "DocBuilder.h"
#include <mongocxx/instance.hpp>

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

MongoWriter::MongoWriter(WriterFrontend *frontend) :
    WriterBackend(frontend),
    formatter(new formatter::Ascii(this, formatter::Ascii::SeparatorInfo())) {
        this->buffer.reserve(this->BUFFER_SIZE);
    }

    bool MongoWriter::DoInit(const WriterInfo &info, int num_fields,
            const Field *const *fields) {

        if (BifConst::LogMongo::debug) {
            std::cout << "[logging::writer::MongoDB]" << std::endl;
            std::cout << "  path=" << info.path << std::endl;
            std::cout << "  rotation_interval=" << info.rotation_interval << std::endl;
            std::cout << "  rotation_base=" << info.rotation_base << std::endl;

            for (const auto &i : info.config)
            {
                std::cout << "  config[" << i.first << "] = " << i.second << std::endl;
            }

            for (int i = 0; i < num_fields; i++) {
                const Field *field = fields[i];
                std::cout << "  field " << field->name << ": "
                    << type_name(field->type) << std::endl;
            }

            std::cout << std::endl;
        }

        this->logCollection = info.path;
        this->insertOptions.ordered(false);

        if ( !SetConfig( info ) )
        {
            return false;
        }

        mongocxx::instance instance{};
        return true;
    }

bool MongoWriter::SetConfig( const WriterInfo& info )
{
    string uriInfo = LookupParam( info, "uri");
    if( !uriInfo.empty() ){
        mongocxx::uri uri( uriInfo );
        this->client = new mongocxx::client(uri);
    }
    else{
        return false;
    }

    string dbInfo = LookupParam( info, "selectedDB");
    if( !uriInfo.empty() ){
        selectedDB = dbInfo;
    }
    else{
        return false;
    }
    return true;
}

string MongoWriter::LookupParam(const WriterInfo& info, const string name) const
{
    map<const char*, const char*>::const_iterator it = info.config.find(name.c_str());
    if ( it == info.config.end() )
        return string();
    else
        return it->second;
}

bool MongoWriter::DoWrite(int num_fields, const Field *const *fields, Value **vals) {
    mongocxx::collection coll = (*this->client)[this->selectedDB][this->logCollection];

    auto builder = plugin::OCMDev_MongoDBWriter::DocBuilder(this->formatter);

    for (int i = 0; i < num_fields; i++) {
        builder.addField(fields[i], vals[i]);
    }

    if (this->buffer.size() == this->BUFFER_SIZE) {
        bsoncxx::stdx::optional<mongocxx::result::insert_many> result =
            coll.insert_many(this->buffer, this->insertOptions);
        this->buffer.clear();
    }

    this->buffer.push_back(builder.finalize());
    return true;
}

bool MongoWriter::DoSetBuf(bool enabled) {
    return true;
}

bool MongoWriter::DoRotate(const char *rotated_path, double open, double close, bool terminating) {
    if (!FinishedRotation("/dev/null", Info().path, open, close, terminating)) {
        Error(Fmt("error rotating %s", Info().path));
        return false;
    }

    return true;
}

bool MongoWriter::DoFlush(double network_time) {
    //guaranteed bufferIdx > 0
    mongocxx::collection coll = (*this->client)[this->selectedDB][this->logCollection];

    bsoncxx::stdx::optional<mongocxx::result::insert_many> result =
        coll.insert_many(this->buffer, this->insertOptions);
    this->buffer.clear();
    return true;
}

bool MongoWriter::DoFinish(double network_time) {
    DoFlush(network_time);
    return true;
}

bool MongoWriter::DoHeartbeat(double network_time, double current_time) {
    return true;
}

MongoWriter::~MongoWriter() {
    delete this->formatter;
    delete this->client;
}
