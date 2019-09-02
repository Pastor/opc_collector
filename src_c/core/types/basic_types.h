#ifndef OPC_COLLECTOR_BASIC_TYPES_H
#define OPC_COLLECTOR_BASIC_TYPES_H

#if defined(__cplusplus)
extern "C" {
#endif
#include <open62541/types.h>
#include <open62541/types_generated.h>
#include <open62541/server.h>

typedef struct {
    UA_Float x;
    UA_Float y;
    UA_Float z;
} Point;

/* The datatype description for the Point datatype */
#define Point_padding_y offsetof(Point,y) - offsetof(Point,x) - sizeof(UA_Float)
#define Point_padding_z offsetof(Point,z) - offsetof(Point,y) - sizeof(UA_Float)

extern UA_DataTypeMember Point_members[3];
extern const UA_DataType PointType;

bool register_basic_types(UA_Server *server);

#if defined(__cplusplus)
}
#endif

#endif //OPC_COLLECTOR_BASIC_TYPES_H
