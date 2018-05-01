//
// Created by logan on 1/11/18.
//

#include <iomanip>
#include "RotatedBufferedMongoDBWriter.h"
using namespace plugin::ActiveCM_RITAWriter;


RotatedBufferedMongoDBWriter::RotatedBufferedMongoDBWriter(const std::shared_ptr<const mongocxx::client>& client,
                                                           std::string _targetDBBase, std::string logCollection) :
        BufferedMongoDBWriter{client, GenRotatedDBName(_targetDBBase), std::move(logCollection)},
        targetDBBase{std::move(_targetDBBase)} {

}

/** Changes the current database to the next one
 *
 * In circumstances where logs are being pulled off the wire it is 
 * advantageous to spilt the logs into different collections based on day 
 * (usually).
 * This function sets the writing collection to the next. Where next is 
 * determined by GetRotatedDBName()
 *
 */
bool RotatedBufferedMongoDBWriter::Rotate() {
    if(!Flush()) {
        return false;
    }
    this->buffer.targetDB = GenRotatedDBName(this->targetDBBase);
    return CreateMetaEntry(this->buffer.targetDB) &&
           this->IndexLogCollection(this->buffer.targetDB, this->buffer.targetCollection);
}

/** Generates a database name using date as a specifier
 *
 */
std::string RotatedBufferedMongoDBWriter::GenRotatedDBName(const std::string& targetDBBase) {
    auto t = std::time(nullptr);
    auto tm = std::gmtime(&t);
    std::ostringstream oss;
    oss << targetDBBase << std::put_time(tm, "-%Y-%m-%d-%H-%M");
    return oss.str();
}
