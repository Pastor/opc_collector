#include <thread>

#include <open62541/config.h>
#include <open62541/server.h>
#include <open62541/server_config.h>
#include <open62541/server_config_default.h>

#include <open62541/client.h>
#include <open62541/client_config.h>
#include <open62541/client_config_default.h>
#include <open62541/client_highlevel.h>

#include <tests_base.hxx>
#include <multithreading.h>
#include "basic_types.h"

extern "C" {
void register_PointType_Variable(UA_Server *server);
}

class Basic_Types : public testing::Test {
public:
    Basic_Types() : _server(UA_Server_new()), _running(true) {}

protected:
    void SetUp() override {
        UA_ServerConfig *config = UA_Server_getConfig(_server);
        UA_ServerConfig_setMinimal(config, 6789, nullptr);

        register_basic_types(_server);
        register_PointType_Variable(_server);
        start();
    }

    void TearDown() override {
        stop();
    }

    void start() {
        std::thread start([&] {
            if (UA_Server_run(_server, &_running) == UA_STATUSCODE_GOOD) {
                u_delay(1000);
                UA_Server_delete(_server);
                _server = nullptr;
            }
        });
        start.detach();
    }

    void stop() {
        _running = false;
    }

private:
    UA_Server *_server;
    volatile UA_Boolean _running;
};

TEST_F(Basic_Types, ReadVariable) {
    UA_DataType types[1];
    types[0] = PointType;

    /* Attention! Here the custom datatypes are allocated on the stack. So they
     * cannot be accessed from parallel (worker) threads. */
    UA_DataTypeArray customDataTypes = {nullptr, 1, types};

    UA_Client *client = UA_Client_new();
    UA_ClientConfig *cc = UA_Client_getConfig(client);
    UA_ClientConfig_setDefault(cc);
    cc->customDataTypes = &customDataTypes;

    UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://localhost:6789");
    ASSERT_EQ(retval, UA_STATUSCODE_GOOD);

    UA_Variant value; /* Variants can hold scalar values and arrays of any type */
    UA_Variant_init(&value);

    UA_NodeId nodeId = UA_NODEID_STRING(1, const_cast<char *>("3D.Point"));

    retval = UA_Client_readValueAttribute(client, nodeId, &value);
    ASSERT_EQ(retval, UA_STATUSCODE_GOOD);
    auto *p = (Point *) value.data;
    printf("Point = %f, %f, %f \n", p->x, p->y, p->z);

    /* Clean up */
    UA_Variant_clear(&value);
    UA_Client_delete(client); /* Disconnects the client internally */
}

