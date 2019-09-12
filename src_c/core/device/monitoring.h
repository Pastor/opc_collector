#ifndef OPC_COLLECTOR_MONITORING_H
#define OPC_COLLECTOR_MONITORING_H
#include <open62541/server.h>

#if defined(__cplusplus)
extern "C" {
#endif

bool monitoring_registration(UA_Server *server);

#if defined(__cplusplus)
}
#endif

#endif //OPC_COLLECTOR_MONITORING_H
