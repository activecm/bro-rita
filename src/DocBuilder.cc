#include "DocBuilder.h"
#include <bsoncxx/builder/stream/array.hpp>
#include <threading/SerialTypes.h>

using namespace plugin::OCMDev_MongoDBWriter;

void DocBuilder::addField(const threading::Field *const field, const threading::Value *const value) {

    string tag = field->name;
    std::replace( tag.begin(), tag.end(), '.', '_');
    if (!value->present) {
        switch (value->type)
        {

            case TYPE_BOOL:
                builder << tag << false;
                break;
            case TYPE_PORT:
                builder << tag << bsoncxx::types::b_int32{0};
                break;
            case TYPE_INT:
            case TYPE_COUNT:
            case TYPE_COUNTER:
                builder << tag << bsoncxx::types::b_int64{0};
                break;
            case TYPE_DOUBLE:
            case TYPE_TIME:
            case TYPE_INTERVAL:
                if (tag == "ts") {
                    //idiosyncrasy with RITA: timestamps are integers
                    builder << tag << bsoncxx::types::b_int64{0};
                } else {
                    builder << tag << bsoncxx::types::b_double{0.0};
                }
                break;
            case TYPE_ADDR:
            case TYPE_SUBNET:
            case TYPE_ENUM:
            case TYPE_STRING:
            case TYPE_FILE:
            case TYPE_FUNC: {
                const char* empty = "";
                builder << tag << empty;
            }
                break;
            case TYPE_TABLE:
            case TYPE_VECTOR:
            {
                builder << tag <<  bsoncxx::builder::stream::array{};
            }
                break;
            case TYPE_VOID:
            case TYPE_PATTERN:
            case TYPE_ANY:
            case TYPE_UNION:
            case TYPE_RECORD:
            case TYPE_LIST:
            case TYPE_TIMER:
            case TYPE_OPAQUE:
            case TYPE_TYPE:
            case TYPE_ERROR:
                std::cout << "Unhandled BroType" << std::endl ;
                break;
        }
        return;
    }
    switch (value->type)
    {
        case TYPE_BOOL:
            builder << tag << bsoncxx::types::b_bool{value->val.int_val};
            break;
        case TYPE_INT:
            builder << tag << bsoncxx::types::b_int64{value->val.int_val};
            break;
        case TYPE_COUNT:
        case TYPE_COUNTER:
            //NOTE: this casts a uint64 to a int64. Overflow may occur
            builder << tag << bsoncxx::types::b_int64{(int64)value->val.uint_val};
            break;
        case TYPE_DOUBLE:
        case TYPE_TIME:
        case TYPE_INTERVAL:
            if (tag == "ts") {
                //idiosyncrasy with RITA: timestamps are integers
                builder << tag << bsoncxx::types::b_int64{(int64)value->val.double_val};
            } else {
                builder << tag << bsoncxx::types::b_double{value->val.double_val};
            }
            break;
        case TYPE_PORT:
            builder << tag << bsoncxx::types::b_int32{(int)value->val.port_val.port};
            break;
        case TYPE_ADDR:
            builder << tag << formatter->Render(value->val.addr_val);
            break;
        case TYPE_SUBNET:
            builder << tag << formatter->Render(value->val.subnet_val);
            break;
        case TYPE_ENUM:
        case TYPE_STRING:
        case TYPE_FILE:
        case TYPE_FUNC: {
                            escaper.Clear();
                            escaper.AddN(value->val.string_val.data, value->val.string_val.length);
                            builder << tag << escaper.Description();
                        }
                        break;
        case TYPE_TABLE:
        case TYPE_VECTOR:
                        {
                            // get the details of the collection
                            bro_int_t size = 0;
                            threading::Value **vals = nullptr;

                            if (value->type == TYPE_TABLE) {
                                size = value->val.set_val.size;
                                vals = value->val.set_val.vals;
                            } else /*if (value->type == TYPE_VECTOR)*/ {
                                size = value->val.vector_val.size;
                                vals = value->val.vector_val.vals;
                            }

                            // build the representation recursively
                            bool innerSuccess;
                            string innerRep;

                            //TODO: Quirks mode
                            //quirks: Escape commas in arrays
                            //https://github.com/bro/bro/blob/master/src/threading/formatters/Ascii.cc#L153
                            escaper.AddEscapeSequence(",");

                            using bsoncxx::builder::stream::array;
                            auto arr = array{};

                            for (bro_int_t i = 0; i < size; i++) {
                                addArrayField( arr,  vals[i] );
                            }
                            escaper.RemoveEscapeSequence(",");

                            builder << tag << arr;
                        }
                        break;
        case TYPE_VOID:
        case TYPE_PATTERN:
        case TYPE_ANY:
        case TYPE_UNION:
        case TYPE_RECORD:
        case TYPE_LIST:
        case TYPE_TIMER:
        case TYPE_OPAQUE:
        case TYPE_TYPE:
        case TYPE_ERROR:
                        std::cout << "Unhandled BroType" << std::endl ;
                        break;
    }
}

void DocBuilder::addArrayField(bsoncxx::builder::stream::array & arr, const threading::Value *const value) {
    if( value->present )
    {
        switch (value->type)
        {
            case TYPE_BOOL:
                arr << bsoncxx::types::b_bool{value->val.int_val};
                break;
            case TYPE_INT:
                arr << bsoncxx::types::b_int64{value->val.int_val};
                break;
            case TYPE_COUNT:
            case TYPE_COUNTER:
                //THIS IS LARGE PROBLEM FIX
                //TODO: Inspect RITA
                arr << bsoncxx::types::b_int64{(int64)value->val.uint_val};
                break;
            case TYPE_DOUBLE:
            case TYPE_TIME:
            case TYPE_INTERVAL:
                arr << bsoncxx::types::b_double{value->val.double_val};
                break;
            case TYPE_PORT:
                arr << bsoncxx::types::b_int32{(int)value->val.port_val.port};
                break;
            case TYPE_ADDR:
                arr << formatter->Render(value->val.addr_val);
                break;
            case TYPE_SUBNET:
                arr << formatter->Render(value->val.subnet_val);
                break;
            case TYPE_ENUM:
            case TYPE_STRING:
            case TYPE_FILE:
            case TYPE_FUNC: {
                                escaper.Clear();
                                escaper.AddN(value->val.string_val.data, value->val.string_val.length);
                                arr << escaper.Description();
                            }
                            break;
            case TYPE_TABLE:
            case TYPE_VECTOR:{
                                 // get the details of the collection
                                 bro_int_t size = 0;
                                 threading::Value **vals = nullptr;

                                 if (value->type == TYPE_TABLE) {
                                     size = value->val.set_val.size;
                                     vals = value->val.set_val.vals;
                                 } else //if (value->type == TYPE_VECTOR)
                                 {
                                     size = value->val.vector_val.size;
                                     vals = value->val.vector_val.vals;
                                 }

                                 // build the representation recursively
                                 bool innerSuccess;
                                 string innerRep;

                                 auto arrInArr = bsoncxx::builder::stream::array{};
                                 for (bro_int_t i = 0; i < size; i++) {
                                     addArrayField( arrInArr, vals[i] );
                                 }

                                 arr << arrInArr;
                             }
                             break;
            case TYPE_VOID:
            case TYPE_PATTERN:
            case TYPE_ANY:
            case TYPE_UNION:
            case TYPE_RECORD:
            case TYPE_LIST:
            case TYPE_TIMER:
            case TYPE_OPAQUE:
            case TYPE_TYPE:
            case TYPE_ERROR:
                             std::cout << "Unhandled BroType" << std::endl ;
                             break;
        }
    }
}

DocBuilder::DocBuilder(const threading::formatter::Ascii *const formatter) : formatter(formatter) {
    escaper.EnableEscaping();
}

bsoncxx::document::value DocBuilder::finalize() {
    return builder.extract();
}



