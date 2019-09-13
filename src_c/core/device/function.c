#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

#include <signal.h>
#include <stdlib.h>
#include <types/basic_types.h>
#include "controller.h"
#include "function.h"

static UA_StatusCode
method_device_send_callback(UA_Server *server,
                            const UA_NodeId *sessionId, void *sessionHandle,
                            const UA_NodeId *methodId, void *methodContext,
                            const UA_NodeId *objectId, void *objectContext,
                            size_t inputSize, const UA_Variant *input,
                            size_t outputSize, UA_Variant *output) {

    UA_String *data = (UA_String *) input[0].data;
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Send %.*s", (int) data->length, data->data);
    return controller_send(server, data->data, data->length) ? UA_STATUSCODE_GOOD : UA_STATUSCODE_BADDATALOST;
}

static UA_StatusCode
method_device_reset_callback(UA_Server *server,
                             const UA_NodeId *sessionId, void *sessionHandle,
                             const UA_NodeId *methodId, void *methodContext,
                             const UA_NodeId *objectId, void *objectContext,
                             size_t inputSize, const UA_Variant *input,
                             size_t outputSize, UA_Variant *output) {

    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Reset device");
    return controller_reset(server) ? UA_STATUSCODE_GOOD : UA_STATUSCODE_BADDATALOST;
}

static void
add_method_device_send(UA_Server *server) {
    UA_Argument input_arguments;
    UA_Argument_init(&input_arguments);
    input_arguments.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
    input_arguments.description = UA_LOCALIZEDTEXT("ru-RU", "Данные для передачи");
    input_arguments.name = UA_STRING("Data");
    input_arguments.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes method_attributes = UA_MethodAttributes_default;
    method_attributes.description = UA_LOCALIZEDTEXT("ru-RU", "Передача строки в устройство");
    method_attributes.displayName = UA_LOCALIZEDTEXT("ru-RU", "Передача данных");
    method_attributes.executable = true;
    method_attributes.userExecutable = true;
    UA_Server_addMethodNode(server, UA_NODEID_NULL,
                            DeviceType_DetectedDevice_id,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASORDEREDCOMPONENT),
                            UA_QUALIFIEDNAME(1, "Send"),
                            method_attributes, &method_device_send_callback,
                            1, &input_arguments, 0, NULL, NULL, NULL);
}

static void
add_method_device_reset(UA_Server *server) {
    UA_MethodAttributes method_attributes = UA_MethodAttributes_default;
    method_attributes.description = UA_LOCALIZEDTEXT("ru-RU", "Сброс и обновление устройства");
    method_attributes.displayName = UA_LOCALIZEDTEXT("ru-RU", "Сброс устройства");
    method_attributes.executable = true;
    method_attributes.userExecutable = true;
    UA_Server_addMethodNode(server, UA_NODEID_NULL,
                            DeviceType_DetectedDevice_id,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASORDEREDCOMPONENT),
                            UA_QUALIFIEDNAME(1, "Reset"),
                            method_attributes, &method_device_reset_callback,
                            0, NULL, 0, NULL, NULL, NULL);
}

bool function_registration(UA_Server *server) {
    add_method_device_send(server);
    add_method_device_reset(server);
    return true;
}
