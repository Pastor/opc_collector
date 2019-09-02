#include <open62541/config.h>
#include <open62541/server_config.h>
#include "basic_types.h"

static UA_DataType types[1];
static UA_DataTypeArray array_of_types = {0, 1, types};

static void
register_PointType(UA_Server *server) {
    UA_VariableTypeAttributes attributes = UA_VariableTypeAttributes_default;
    attributes.description = UA_LOCALIZEDTEXT("en-US", "3D Point");
    attributes.displayName = UA_LOCALIZEDTEXT("en-US", "3D Point");
    attributes.dataType = PointType.typeId;
    attributes.valueRank = UA_VALUERANK_SCALAR;

    Point p;
    p.x = 0.0;
    p.y = 0.0;
    p.z = 0.0;
    UA_Variant_setScalar(&attributes.value, &p, &PointType);

    UA_Server_addVariableTypeNode(server, PointType.typeId,
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
                                  UA_QUALIFIEDNAME(1, "3D.Point"),
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                                  attributes, NULL, NULL);

}

void
register_PointType_Variable(UA_Server *server) {
    Point p;
    p.x = 3.0;
    p.y = 4.0;
    p.z = 5.0;
    UA_VariableAttributes attributes = UA_VariableAttributes_default;
    attributes.description = UA_LOCALIZEDTEXT("en-US", "3D Point");
    attributes.displayName = UA_LOCALIZEDTEXT("en-US", "3D Point");
    attributes.dataType = PointType.typeId;
    attributes.valueRank = UA_VALUERANK_SCALAR;
    UA_Variant_setScalar(&attributes.value, &p, &PointType);

    UA_Server_addVariableNode(server, UA_NODEID_STRING(1, "3D.Point"),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                              UA_QUALIFIEDNAME(1, "3D.Point"),
                              PointType.typeId, attributes, NULL, NULL);
}



bool register_basic_types(UA_Server *server) {
    if (0 == server)
        return false;

    UA_ServerConfig *config = UA_Server_getConfig(server);

    // create array
    types[0] = PointType;
    types[0].members = Point_members;

    // register types to stack
    array_of_types.next = config->customDataTypes;
    config->customDataTypes = &array_of_types;
    register_PointType(server);
}

