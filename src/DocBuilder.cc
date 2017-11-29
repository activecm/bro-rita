#include "DocBuilder.h"
#include <bsoncxx/builder/stream/array.hpp>

using namespace plugin::OCMDev_MongoDBWriter;

void DocBuilder::addField(const threading::Field *const field, const threading::Value *const value) {

            static void *types[] =
            {
                &&case_void,
                &&case_bool,      // 1
                &&case_int,       // 2
                &&case_count,     // 3
                &&case_counter,   // 4
                &&case_double,    // 5
                &&case_time,      // 6
                &&case_interval,  // 7
                &&case_string,    // 8
                &&case_pattern,   // 9
                &&case_enum,      // 10
                &&case_timer,     // 11
                &&case_port,      // 12
                &&case_addr,      // 13
                &&case_subnet,    // 14
                &&case_any,       // 15
                &&case_table,     // 16
                &&case_union,     // 17
                &&case_record,    // 18
                &&case_list,      // 19
                &&case_func,      // 20
                &&case_file,      // 21
                &&case_vector,    // 22
                &&case_opaque,    // 23
                &&case_type,      // 24
                &&case_error      // 25
            };

    if( value->present )
    {
        string tag = field->name;
        std::replace( tag.begin(), tag.end(), '.', '_');

        goto * types[(value->type)];
            case_bool:
                builder << tag << bsoncxx::types::b_bool{value->val.int_val};
                goto switch_end;
             case_int:
                builder << tag << bsoncxx::types::b_int64{value->val.int_val};
                goto switch_end;
             case_count:
             case_counter:
                //this is large problem fix
                //todo: inspect rita
                builder << tag << bsoncxx::types::b_int64{(int64)value->val.uint_val};
                goto switch_end;
             case_double:
             case_time:
             case_interval:
                builder << tag << bsoncxx::types::b_double{value->val.double_val};
                goto switch_end;
             case_port:
                builder << tag << bsoncxx::types::b_int32{(int)value->val.port_val.port};
                goto switch_end;
             case_addr:
                builder << tag << formatter->Render(value->val.addr_val);
                goto switch_end;
             case_subnet:
                builder << tag << formatter->Render(value->val.subnet_val);
                goto switch_end;
             case_enum:
             case_string:
             case_file:
             case_func: {
                                const auto length = static_cast<unsigned long>(
                                        value->val.string_val.length
                                        );
                                builder << tag << string(value->val.string_val.data, length);
                            }
                            goto switch_end;
             case_table:
             case_vector:
                            {
                                // get the details of the collection
                                bro_int_t size = 0;
                                threading::Value **vals = nullptr;

                                if (value->type == TYPE_TABLE) {
                                    size = value->val.set_val.size;
                                    vals = value->val.set_val.vals;
                                } else /*if (value->type == case_vector)*/ {
                                    size = value->val.vector_val.size;
                                    vals = value->val.vector_val.vals;
                                }

                                // build the representation recursively
                                bool innersuccess;
                                string innerrep;

                                using bsoncxx::builder::stream::array;
                                auto arr = array{};

                                for (bro_int_t i = 0; i < size; i++) {
                                    addArrayField( arr,  vals[i] );
                                }
                                builder << tag << arr;
                            }
                            goto switch_end;
             case_void:
             case_pattern:
             case_any:
             case_union:
             case_record:
             case_list:
             case_timer:
             case_opaque:
             case_type:
             case_error:
                            std::cout << "Unhandled BroType" << std::endl ;
    }
            switch_end:
    return;
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
                                const auto length = static_cast<unsigned long>(
                                        value->val.string_val.length
                                        );
                                arr << string(value->val.string_val.data, length);
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
}

bsoncxx::document::value DocBuilder::finalize() {
    return builder.extract();
}



