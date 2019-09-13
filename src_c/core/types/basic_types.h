#ifndef OPC_COLLECTOR_BASIC_TYPES_H
#define OPC_COLLECTOR_BASIC_TYPES_H

#if defined(__cplusplus)
extern "C" {
#endif
#include <open62541/types.h>
#include <open62541/types_generated.h>
#include <open62541/server.h>

#define UA_DEVICE_TYPE_UNKNOWN    0
#define UA_DEVICE_TYPE_HID        1

#define UA_EVENT_PROVIDER_TYPE_LOCAL  0
#define UA_EVENT_PROVIDER_TYPE_REMOTE 1

typedef struct {
    UA_Int32   type;
    UA_Int32   vid;
    UA_Int32   pid;
    UA_String  serial;
    UA_String  product;
    UA_Int32   firmware;
} DeviceInformation;

extern UA_DataTypeMember DeviceInformation_members[6];
extern const UA_DataType DeviceInformationType;

typedef struct {
    UA_Int32   battery;
    UA_Int32   button;
    UA_Int32   group;
    UA_Int32   device;
    UA_Int32   provider_type;
    UA_Int32   timeout;
    UA_String  provider;
    UA_String  enter;
} SignalEvent;

extern UA_DataTypeMember SignalEvent_members[8];
extern const UA_DataType SignalEventType;

bool register_builtin_types(UA_Server *server);

#if defined(__cplusplus)
}
#endif

#endif //OPC_COLLECTOR_BASIC_TYPES_H
