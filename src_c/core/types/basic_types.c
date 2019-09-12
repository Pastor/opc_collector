#include <open62541/config.h>
#include <open62541/server_config.h>
#include "basic_types.h"

#define DeviceInformation_padding_vid      offsetof(DeviceInformation,vid) - offsetof(DeviceInformation,type) - sizeof(UA_Int32)
#define DeviceInformation_padding_pid      offsetof(DeviceInformation,pid) - offsetof(DeviceInformation,vid) - sizeof(UA_Int32)
#define DeviceInformation_padding_serial   offsetof(DeviceInformation,serial) - offsetof(DeviceInformation,pid) - sizeof(UA_Int32)
#define DeviceInformation_padding_product  offsetof(DeviceInformation,product) - offsetof(DeviceInformation,serial) - sizeof(UA_String)
#define DeviceInformation_padding_firmware offsetof(DeviceInformation,firmware) - offsetof(DeviceInformation,product) - sizeof(UA_String)

UA_DataTypeMember DeviceInformation_members[6] = {
        {
                UA_TYPENAME("type")     UA_TYPES_INT32, 0,                                   true, false
        },
        {
                UA_TYPENAME("vid")      UA_TYPES_INT32,  DeviceInformation_padding_vid,      true, false
        },
        {
                UA_TYPENAME("pid")      UA_TYPES_INT32,  DeviceInformation_padding_pid,      true, false
        },
        {
                UA_TYPENAME("serial")   UA_TYPES_STRING, DeviceInformation_padding_serial,   true, false
        },
        {
                UA_TYPENAME("product")  UA_TYPES_STRING, DeviceInformation_padding_product,  true, false
        },
        {
                UA_TYPENAME("firmware") UA_TYPES_INT32,  DeviceInformation_padding_firmware, true, false
        }
};
const UA_DataType DeviceInformationType = {
        UA_TYPENAME("DeviceInformation")
        {1, UA_NODEIDTYPE_NUMERIC, {4142}},
        sizeof(DeviceInformation),
        0,
        UA_DATATYPEKIND_STRUCTURE,
        true,
        false,
        6,
        0,
        DeviceInformation_members
};

static void
register_DeviceInformationType(UA_Server *server) {
    UA_VariableTypeAttributes attributes = UA_VariableTypeAttributes_default;
    attributes.description = UA_LOCALIZEDTEXT("ru-RU", "Информация о подключенном устройстве в систему");
    attributes.displayName = UA_LOCALIZEDTEXT("ru-RU", "Информация об устройстве");
    attributes.dataType = DeviceInformationType.typeId;
    attributes.valueRank = UA_VALUERANK_SCALAR;

    DeviceInformation di;
    di.type = UA_DEVICE_TYPE_UNKNOWN;
    di.vid = 0;
    di.pid = 0;
    di.product = UA_STRING("Unknown");
    di.firmware = 0;
    di.serial = UA_STRING("Unknown");
    UA_Variant_setScalar(&attributes.value, &di, &DeviceInformationType);

    UA_Server_addVariableTypeNode(server, DeviceInformationType.typeId,
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
                                  UA_QUALIFIEDNAME(1, "DeviceInformation"),
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                                  attributes, NULL, NULL);

}

#define SignalEvent_padding_button        offsetof(SignalEvent,button) - offsetof(SignalEvent,battery) - sizeof(UA_Int32)
#define SignalEvent_padding_group         offsetof(SignalEvent,group) - offsetof(SignalEvent,button) - sizeof(UA_Int32)
#define SignalEvent_padding_device        offsetof(SignalEvent,device) - offsetof(SignalEvent,group) - sizeof(UA_Int32)
#define SignalEvent_padding_provider_type offsetof(SignalEvent,provider_type) - offsetof(SignalEvent,device) - sizeof(UA_Int32)
#define SignalEvent_padding_timeout       offsetof(SignalEvent,timeout) - offsetof(SignalEvent,provider_type) - sizeof(UA_Int32)
#define SignalEvent_padding_provider      offsetof(SignalEvent,provider) - offsetof(SignalEvent,timeout) - sizeof(UA_String)
#define SignalEvent_padding_enter         offsetof(SignalEvent,enter) - offsetof(SignalEvent,provider) - sizeof(UA_String)

UA_DataTypeMember SignalEvent_members[8] = {
        {
                UA_TYPENAME("battery")       UA_TYPES_INT32, 0,                                  true, false
        },
        {
                UA_TYPENAME("button")        UA_TYPES_INT32,  SignalEvent_padding_button,        true, false
        },
        {
                UA_TYPENAME("group")         UA_TYPES_INT32,  SignalEvent_padding_group,         true, false
        },
        {
                UA_TYPENAME("device")        UA_TYPES_INT32,  SignalEvent_padding_device,        true, false
        },
        {
                UA_TYPENAME("provider_type") UA_TYPES_INT32,  SignalEvent_padding_provider_type, true, false
        },
        {
                UA_TYPENAME("timeout")       UA_TYPES_INT32,  SignalEvent_padding_timeout,       true, false
        },
        {
                UA_TYPENAME("provider")      UA_TYPES_STRING, SignalEvent_padding_provider,      true, false
        },
        {
                UA_TYPENAME("enter")         UA_TYPES_STRING, SignalEvent_padding_enter,         true, false
        }
};
const UA_DataType SignalEventType = {
        UA_TYPENAME("SignalEvent")
        {1, UA_NODEIDTYPE_NUMERIC, {4143}},
        sizeof(SignalEvent),
        0,
        UA_DATATYPEKIND_STRUCTURE,
        true,
        false,
        6,
        0,
        SignalEvent_members
};

static void
register_SignalEventType(UA_Server *server) {
    UA_VariableTypeAttributes attributes = UA_VariableTypeAttributes_default;
    attributes.description = UA_LOCALIZEDTEXT("ru-RU", "Обрабатываемое событие от устройства");
    attributes.displayName = UA_LOCALIZEDTEXT("ru-RU", "Событие");
    attributes.dataType = SignalEventType.typeId;
    attributes.valueRank = UA_VALUERANK_SCALAR;

    SignalEvent se;
    se.provider_type = UA_EVENT_PROVIDER_TYPE_LOCAL;
    se.provider = UA_STRING("Dummy");
    se.enter = UA_STRING("Unknown");
    se.device = 0;
    se.battery = 0;
    se.button = 0;
    se.group = 0;
    se.timeout = 0;
    UA_Variant_setScalar(&attributes.value, &se, &SignalEventType);

    UA_Server_addVariableTypeNode(server, SignalEventType.typeId,
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
                                  UA_QUALIFIEDNAME(1, "SignalEvent"),
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                                  attributes, NULL, NULL);

}

static UA_DataType types[2];
static UA_DataTypeArray array_of_types = {0, 2, types};

void
register_DeviceInformationType_Variable(UA_Server *server, DeviceInformation *di) {
    UA_VariableAttributes attributes = UA_VariableAttributes_default;
    attributes.description = UA_LOCALIZEDTEXT("ru-RU", "Добавленное вручную устройство");
    attributes.displayName = UA_LOCALIZEDTEXT("ru-RU", "Устройство");
    attributes.dataType = DeviceInformationType.typeId;
    attributes.valueRank = UA_VALUERANK_SCALAR;
    UA_Variant_setScalar(&attributes.value, di, &DeviceInformationType);

    UA_Server_addVariableNode(server, UA_NODEID_STRING(1, "Test.Device"),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                              UA_QUALIFIEDNAME(1, "Test.Device"),
                              DeviceInformationType.typeId, attributes, NULL, NULL);
}

#if 1
UA_NodeId pumpTypeId = {1, UA_NODEIDTYPE_NUMERIC, {1001}};

static void
defineObjectTypes(UA_Server *server) {
    /* Define the object type for "Device" */
    UA_NodeId deviceTypeId; /* get the nodeid assigned by the server */
    UA_ObjectTypeAttributes dtAttr = UA_ObjectTypeAttributes_default;
    dtAttr.displayName = UA_LOCALIZEDTEXT("en-US", "DeviceType");
    UA_Server_addObjectTypeNode(server, UA_NODEID_NULL,
                                UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
                                UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
                                UA_QUALIFIEDNAME(1, "DeviceType"), dtAttr,
                                NULL, &deviceTypeId);

    UA_VariableAttributes mnAttr = UA_VariableAttributes_default;
    mnAttr.displayName = UA_LOCALIZEDTEXT("en-US", "ManufacturerName");
    UA_NodeId manufacturerNameId;
    UA_Server_addVariableNode(server, UA_NODEID_NULL, deviceTypeId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(1, "ManufacturerName"),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), mnAttr, NULL, &manufacturerNameId);
    /* Make the manufacturer name mandatory */
    UA_Server_addReference(server, manufacturerNameId,
                           UA_NODEID_NUMERIC(0, UA_NS0ID_HASMODELLINGRULE),
                           UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_MODELLINGRULE_MANDATORY), true);


    UA_VariableAttributes modelAttr = UA_VariableAttributes_default;
    modelAttr.displayName = UA_LOCALIZEDTEXT("en-US", "ModelName");
    UA_Server_addVariableNode(server, UA_NODEID_NULL, deviceTypeId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(1, "ModelName"),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), modelAttr, NULL, NULL);

    /* Define the object type for "Pump" */
    UA_ObjectTypeAttributes ptAttr = UA_ObjectTypeAttributes_default;
    ptAttr.displayName = UA_LOCALIZEDTEXT("en-US", "PumpType");
    UA_Server_addObjectTypeNode(server, pumpTypeId,
                                deviceTypeId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
                                UA_QUALIFIEDNAME(1, "PumpType"), ptAttr,
                                NULL, NULL);

    UA_VariableAttributes statusAttr = UA_VariableAttributes_default;
    statusAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Status");
    statusAttr.valueRank = UA_VALUERANK_SCALAR;
    UA_NodeId statusId;
    UA_Server_addVariableNode(server, UA_NODEID_NULL, pumpTypeId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(1, "Status"),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), statusAttr, NULL, &statusId);
    /* Make the status variable mandatory */
    UA_Server_addReference(server, statusId,
                           UA_NODEID_NUMERIC(0, UA_NS0ID_HASMODELLINGRULE),
                           UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_MODELLINGRULE_MANDATORY), true);

    UA_VariableAttributes rpmAttr = UA_VariableAttributes_default;
    rpmAttr.displayName = UA_LOCALIZEDTEXT("en-US", "MotorRPM");
    rpmAttr.valueRank = UA_VALUERANK_SCALAR;
    UA_Server_addVariableNode(server, UA_NODEID_NULL, pumpTypeId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(1, "MotorRPMs"),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), rpmAttr, NULL, NULL);
}

#endif


bool register_basic_types(UA_Server *server) {
    if (0 == server)
        return false;

    UA_ServerConfig *config = UA_Server_getConfig(server);

    types[0] = DeviceInformationType;
    types[0].members = DeviceInformation_members;
    types[1] = SignalEventType;
    types[1].members = SignalEvent_members;

    array_of_types.next = config->customDataTypes;
    config->customDataTypes = &array_of_types;
    register_DeviceInformationType(server);
    register_SignalEventType(server);
    defineObjectTypes(server);
}

