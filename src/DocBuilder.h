#ifndef BROPLUGINMONGODBWRITER_DOCBUILDER_H
#define BROPLUGINMONGODBWRITER_DOCBUILDER_H

#include <threading/formatters/Ascii.h>
#include <threading/SerialTypes.h>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>

namespace plugin {
    namespace ActiveCM_MongoDBWriter {
        class DocBuilder {
        public:
            explicit DocBuilder(const threading::formatter::Ascii *formatter);
            void addField(const threading::Field *field, const threading::Value *value );
            void addArrayField( bsoncxx::builder::stream::array& arr, const threading::Value *value );
            bsoncxx::document::value finalize();

        private:
            bsoncxx::builder::stream::document builder;
            const threading::formatter::Ascii *const formatter;
            ODesc escaper;
        };
    }
}


#endif //BROPLUGINMONGODBWRITER_DOCBUILDER_H
