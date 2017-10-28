//
// Created by logan on 10/24/17.
//

#ifndef MONGODB_WRITER_MONGOCONV_H
#define MONGODB_WRITER_MONGOCONV_H

#include <string>
#include <memory>
#include <tuple>
#include <threading/formatters/Ascii.h>
#include <threading/SerialTypes.h>

namespace plugin {
    namespace OCMDev_MongoDBWriter {


        string getTag( Field *const fields);
        void addField( builder::stream::builder, const Field *const *fields, Value * value );
        std::tuple<bool, string>
        broToString(const threading::formatter::Ascii *const &formatter, const threading::Value *const &value);
    }
}
#endif //BROPLUGINBRO_MONGODB_MONGOCONV_H
