#include <thread>

#include <open62541/server.h>

#include <open62541/client.h>
#include <open62541/client_config.h>
#include <open62541/client_config_default.h>
#include <open62541/client_highlevel.h>

#include <tests_base.hxx>
#include <default_server.h>
#include "basic_types.h"

TEST(BasicTypes, ReadVariable) {
    UA_DataType types[1];
    types[0] = DeviceInformationType;

    /* Attention! Here the custom datatypes are allocated on the stack. So they
     * cannot be accessed from parallel (worker) threads. */
    UA_DataTypeArray customDataTypes = {nullptr, 1, types};

    UA_Client *client = UA_Client_new();
    UA_ClientConfig *cc = UA_Client_getConfig(client);
    UA_ClientConfig_setDefault(cc);
    cc->customDataTypes = &customDataTypes;

    UA_StatusCode ret = UA_Client_connect(client, UA_CLIENT_CONNECTION_STRING);
    ASSERT_EQ(ret, UA_STATUSCODE_GOOD);

    UA_Variant value;
    UA_Variant_init(&value);

    UA_NodeId nodeId = UA_NODEID_STRING(1, const_cast<char *>(UA_CLIENT_DEFAULT_DEVICE_QUALIFY_NAME));

    ret = UA_Client_readValueAttribute(client, nodeId, &value);
    ASSERT_EQ(ret, UA_STATUSCODE_GOOD);
    auto *di = (DeviceInformation *) value.data;
    printf("DeviceInformation = %d, %d, %d, %.*s, %.*s, %d \n", di->type, di->pid, di->vid, di->serial.length,
           di->serial.data, di->product.length, di->product.data, di->firmware);
    ASSERT_EQ(strlen(UA_CLIENT_DEFAULT_DEVICE_SERIAL), di->serial.length);
    ASSERT_TRUE(memcmp(di->serial.data, UA_CLIENT_DEFAULT_DEVICE_SERIAL, strlen(UA_CLIENT_DEFAULT_DEVICE_SERIAL)) == 0);
    ASSERT_EQ(strlen(UA_CLIENT_DEFAULT_DEVICE_PRODUCT), di->product.length);
    ASSERT_TRUE(
            memcmp(di->product.data, UA_CLIENT_DEFAULT_DEVICE_PRODUCT, strlen(UA_CLIENT_DEFAULT_DEVICE_PRODUCT)) == 0);
    UA_Variant_clear(&value);
    UA_Client_delete(client);
}

