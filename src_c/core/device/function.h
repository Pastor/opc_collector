#ifndef OPC_COLLECTOR_FUNCTION_H
#define OPC_COLLECTOR_FUNCTION_H
#include <open62541/server.h>

#if defined(__cplusplus)
extern "C" {
#endif

bool function_registration(UA_Server *server);

#if defined(__cplusplus)
}
#endif
#endif //OPC_COLLECTOR_FUNCTION_H
