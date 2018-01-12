//
// Created by logan on 1/11/18.
//

#ifndef BROPLUGINMONGODBWRITER_DAILYBUFFEREDMONGODBWRITER_H
#define BROPLUGINMONGODBWRITER_DAILYBUFFEREDMONGODBWRITER_H
#include "AbstractMongoDBWriter.h"
#include "MongoDBBuffer.h"
#include <unordered_map>

namespace plugin {
    namespace OCMDev_MongoDBWriter {
        class DailyBufferedMongoDBWriter : public AbstractMongoDBWriter {
        public:
            explicit DailyBufferedMongoDBWriter(const std::shared_ptr<const mongocxx::client> &client,
                                                  std::string targetDBBase,
                                                  std::string logCollection);

            bool Write(bsoncxx::document::value document) override;

            bool Flush() override;

        private:
            std::string GenDailyDBName(const std::string &targetDBBase, time_t dayTS);

            const std::string targetDBBase;
            const std::string logCollection;
            std::unordered_map<int64_t, MongoDBBuffer> buffers;
        };
    }
}


#endif //BROPLUGINMONGODBWRITER_DAILYBUFFEREDMONGODBWRITER_H
