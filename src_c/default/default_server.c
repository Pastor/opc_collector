#include <open62541/server.h>

#include <open62541/server_config.h>
#include <open62541/server_config_default.h>
#include <open62541/client.h>

#include <open62541/client_config.h>
#include <open62541/client_config_default.h>
#include <open62541/client_highlevel.h>
#include "types/basic_types.h"
#include "device/controller.h"
#include "device/function.h"

#include "default_server.h"
#include "multithreading.h"

volatile UA_Boolean _default_server_running = false;
UA_Server *_default_server;

static void *monitoring_loop(void *args) {

    UA_ServerConfig *config = UA_Server_getConfig(_default_server);
    UA_ServerConfig_setMinimal(config, 6789, NULL);
    register_builtin_types(_default_server);
    controller_registration(_default_server);
    function_registration(_default_server);
    _default_server_running = true;
    if (UA_Server_run(_default_server, &_default_server_running) == UA_STATUSCODE_GOOD) {
        u_delay(1000);
        UA_Server_delete(_default_server);
        _default_server = NULL;
    }
    return NULL;
}

void default_server_start() {
    _default_server = UA_Server_new();
    thread_create(monitoring_loop, _default_server, true);
}

void default_server_stop() {
    _default_server_running = false;
}
