//
// Created by logan on 10/24/17.
//

#include "MongoConv.h"

using namespace threading;
using namespace formatter;
using namespace std;

std::tuple<bool, string> plugin::OCMDev_MongoDBWriter::broToString(const threading::formatter::Ascii *const &formatter,
                                                             const threading::Value *const &value) {
    if (!value->present ||
        (value->type == TYPE_VECTOR && value->val.vector_val.size == 0) ||
        (value->type == TYPE_TABLE && value->val.set_val.size == 0)) {
        return make_tuple(false, "-");
    }
    string representation;
    switch (value->type) {
        case TYPE_BOOL:
        case TYPE_INT:
            representation = to_string(value->val.int_val);
            break;
        case TYPE_COUNT:
        case TYPE_COUNTER:
            representation = to_string(value->val.uint_val);
            break;
        case TYPE_TIME:
        case TYPE_INTERVAL:
        case TYPE_DOUBLE:
            representation = to_string(value->val.double_val);
            break;
        case TYPE_PORT:
            representation = to_string(value->val.port_val.port);
            break;
        case TYPE_STRING:
        case TYPE_ENUM:
        case TYPE_FILE:
        case TYPE_FUNC: {
            const auto length = static_cast<unsigned long>(
                    value->val.string_val.length
            );
            representation = string(value->val.string_val.data, length);
        }
            break;
        case TYPE_SUBNET:
            representation = formatter->Render(value->val.subnet_val);
            break;
        case TYPE_ADDR:
            representation = formatter->Render(value->val.addr_val);
            break;
        case TYPE_TABLE: // TODO: Test this
        case TYPE_VECTOR: {
            // get the details of the collection
            bro_int_t size = 0;
            Value **vals = nullptr;

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
            representation = "{";

            for (bro_int_t i = 0; i < size; i++) {
                tie(innerSuccess, innerRep) = broToString(formatter, vals[i]);
                representation += innerRep;
                if (i != size - 1) {
                    representation += ", ";
                }
            }
            representation += "}";
        }
            break;
        default:
            return make_tuple(false, "?");
    }
    return make_tuple(true, representation);
}