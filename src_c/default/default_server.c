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

static UA_Boolean _running = true;
static UA_Server *_server;

void register_DeviceInformationType_Variable(UA_Server *server, DeviceInformation *di);

static void *monitoring_loop(void *args) {
    DeviceInformation gdi = {
            UA_DEVICE_TYPE_HID, 0, 0,
            UA_STRING(UA_CLIENT_DEFAULT_DEVICE_SERIAL),
            UA_STRING(UA_CLIENT_DEFAULT_DEVICE_PRODUCT), 0
    };
    UA_ServerConfig *config = UA_Server_getConfig(_server);
    UA_ServerConfig_setMinimal(config, 6789, NULL);
    register_builtin_types(_server);
    controller_registration(_server);
    function_registration(_server);
    register_DeviceInformationType_Variable(_server, &gdi);
    if (UA_Server_run(_server, &_running) == UA_STATUSCODE_GOOD) {
        u_delay(1000);
        UA_Server_delete(_server);
        _server = NULL;
    }
    return NULL;
}

void default_server_start() {
    _server = UA_Server_new();
    thread_create(monitoring_loop, _server, true);
}

void default_server_stop() {
    _running = false;
}
