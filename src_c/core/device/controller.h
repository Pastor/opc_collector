#ifndef OPC_COLLECTOR_CONTROLLER_H
#define OPC_COLLECTOR_CONTROLLER_H
#include <open62541/server.h>

#if defined(__cplusplus)
extern "C" {
#endif

bool controller_registration(UA_Server *server);
bool controller_send(UA_Server *server, const unsigned char *data, size_t data_length);
bool controller_reset(UA_Server *server);

#if defined(__cplusplus)
}
#endif

#endif //OPC_COLLECTOR_CONTROLLER_H
