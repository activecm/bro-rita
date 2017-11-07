#ifndef BROPLUGINMONGODBWRITER_DOCBUILDER_H
#define BROPLUGINMONGODBWRITER_DOCBUILDER_H

#include <threading/formatters/Ascii.h>
#include <threading/SerialTypes.h>
#include <bsoncxx/builder/stream/document.hpp>

namespace plugin {
    namespace OCMDev_MongoDBWriter {
        class DocBuilder {
        public:
            explicit DocBuilder(const threading::formatter::Ascii *const formatter);
            void addField(const threading::Field * const field, const threading::Value * const value );
            bsoncxx::document::view finalize();
        private:
            bsoncxx::builder::stream::document builder;
            const threading::formatter::Ascii *const formatter;
        };
    }
}


#endif //BROPLUGINMONGODBWRITER_DOCBUILDER_H
