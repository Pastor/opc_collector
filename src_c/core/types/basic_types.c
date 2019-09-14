#include <open62541/config.h>
#include <open62541/server_config.h>
#include <open62541/plugin/log_stdout.h>
#include <include/open62541/types.h>
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

UA_NodeId SignalEventType_id;

static void
register_SignalEventType(UA_Server *server) {
    UA_ObjectTypeAttributes attributes = UA_ObjectTypeAttributes_default;
    attributes.description = UA_LOCALIZEDTEXT("ru-RU", "Обрабатываемое событие от устройства");
    attributes.displayName = UA_LOCALIZEDTEXT("ru-RU", "Событие");

    UA_StatusCode ret = UA_Server_addObjectTypeNode(server, UA_NODEID_NULL,
                                                    UA_NODEID_NUMERIC(0, UA_NS0ID_BASEEVENTTYPE),
                                                    UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
                                                    UA_QUALIFIEDNAME(1, "SignalEvent"),
                                                    attributes, NULL, &SignalEventType_id);
    if (ret != UA_STATUSCODE_GOOD) {
        UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_SERVER,
                     "Can't create event %s", UA_StatusCode_name(ret));
    }
}

static UA_DataType types[2];
static UA_DataTypeArray array_of_types = {0, 2, types};

void
register_DeviceInformationType_Variable(UA_Server *server, DeviceInformation *di) {
    UA_VariableAttributes attributes = UA_VariableAttributes_default;
    attributes.description = UA_LOCALIZEDTEXT("ru-RU", "Добавленное вручную устройство");
    attributes.displayName = UA_LOCALIZEDTEXT("ru-RU", "Устройство для провеки");
    attributes.dataType = DeviceInformationType.typeId;
    attributes.valueRank = UA_VALUERANK_SCALAR;
    UA_Variant_setScalar(&attributes.value, di, &DeviceInformationType);

    UA_StatusCode ret = UA_Server_addVariableNode(server, UA_NODEID_STRING(1, "Test.Device"),
                                                  UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                                                  UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                                                  UA_QUALIFIEDNAME(1, "Test.Device"),
                                                  DeviceInformationType.typeId, attributes, NULL, NULL);
    if (ret != UA_STATUSCODE_GOOD) {
        UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_SERVER,
                     "Can't add DeviceInformation variable. %s", UA_StatusCode_name(ret));
    }
}

#if 1
UA_NodeId DeviceType_id;
UA_NodeId DeviceType_type_id;
UA_NodeId DeviceType_USB_id;
UA_NodeId DeviceType_USB_vid_id;
UA_NodeId DeviceType_USB_pid_id;
UA_NodeId DeviceType_manufacturer_id;
UA_NodeId DeviceType_product_id;
UA_NodeId DeviceType_serial_id;

UA_NodeId DeviceType_DetectedDevice_id;
UA_NodeId DeviceType_DetectedDevice_USB_id;

UA_NodeId DeviceType_DetectedDevice_type_id;
UA_NodeId DeviceType_DetectedDevice_USB_vid_id;
UA_NodeId DeviceType_DetectedDevice_USB_pid_id;
UA_NodeId DeviceType_DetectedDevice_manufacturer_id;
UA_NodeId DeviceType_DetectedDevice_product_id;
UA_NodeId DeviceType_DetectedDevice_serial_id;

static UA_NodeId find_device_type_id(UA_Server *server, UA_NodeId starting_node, char *search_name) {
    UA_RelativePathElement rpe;
    UA_RelativePathElement_init(&rpe);
    rpe.referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY);
    rpe.isInverse = false;
    rpe.includeSubtypes = false;
    rpe.targetName = UA_QUALIFIEDNAME(1, search_name);

    UA_BrowsePath bp;
    UA_BrowsePath_init(&bp);
    bp.startingNode = starting_node;
    bp.relativePath.elementsSize = 1;
    bp.relativePath.elements = &rpe;

    UA_BrowsePathResult bpr = UA_Server_translateBrowsePathToNodeIds(server, &bp);
    if (bpr.statusCode != UA_STATUSCODE_GOOD || bpr.targetsSize < 1) {
        UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "ID %s not found", search_name);
        return UA_NODEID_NULL;
    }
    return bpr.targets[0].targetId.nodeId;
}

static void
register_object_values(UA_Server *server) {
    {
        UA_ObjectAttributes device_attributes = UA_ObjectAttributes_default;
        device_attributes.displayName = UA_LOCALIZEDTEXT("ru-RU", "Устройство");
        UA_Server_addObjectNode(server, UA_NODEID_NULL,
                                UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                                UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                                UA_QUALIFIEDNAME(1, "DetectedDevice"),
                                DeviceType_id,
                                device_attributes, NULL, &DeviceType_DetectedDevice_id);
    }
    {
        UA_ObjectAttributes usb_attributes = UA_ObjectAttributes_default;
        usb_attributes.displayName = UA_LOCALIZEDTEXT("ru-RU", "USB");
        UA_Server_addObjectNode(server, UA_NODEID_NULL,
                                DeviceType_DetectedDevice_id,
                                UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                                UA_QUALIFIEDNAME(1, "DetectedDevice.USB"),
                                DeviceType_USB_id,
                                usb_attributes, NULL, &DeviceType_DetectedDevice_USB_id);
    }
    {
        DeviceType_DetectedDevice_type_id = find_device_type_id(server, DeviceType_DetectedDevice_id, "Type");
        DeviceType_DetectedDevice_manufacturer_id = find_device_type_id(server, DeviceType_DetectedDevice_id,
                                                                        "Manufacturer");
        DeviceType_DetectedDevice_product_id = find_device_type_id(server, DeviceType_DetectedDevice_id, "Product");
        DeviceType_DetectedDevice_serial_id = find_device_type_id(server, DeviceType_DetectedDevice_id, "Serial");

        DeviceType_DetectedDevice_USB_vid_id = find_device_type_id(server, DeviceType_DetectedDevice_USB_id, "VID");
        DeviceType_DetectedDevice_USB_pid_id = find_device_type_id(server, DeviceType_DetectedDevice_USB_id, "PID");
    }
}

static void
register_object_types(UA_Server *server) {
    UA_ObjectTypeAttributes device_type_attributes = UA_ObjectTypeAttributes_default;
    device_type_attributes.displayName = UA_LOCALIZEDTEXT("ru-RU", "Устройство");
    UA_Server_addObjectTypeNode(server, UA_NODEID_NULL,
                                UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
                                UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
                                UA_QUALIFIEDNAME(1, "DeviceType"), device_type_attributes,
                                NULL, &DeviceType_id);

    // type
    {
        UA_VariableAttributes type_attributes = UA_VariableAttributes_default;
        type_attributes.displayName = UA_LOCALIZEDTEXT("ru-RU", "Тип устройства");
        type_attributes.valueRank = UA_VALUERANK_SCALAR;
        type_attributes.dataType = UA_TYPES[UA_TYPES_INT32].typeId;
        UA_Server_addVariableNode(server, UA_NODEID_NULL, DeviceType_id,
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
                                  UA_QUALIFIEDNAME(1, "Type"),
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), type_attributes, NULL,
                                  &DeviceType_type_id);
        /* Make the manufacturer name mandatory */
        UA_Server_addReference(server, DeviceType_type_id,
                               UA_NODEID_NUMERIC(0, UA_NS0ID_HASMODELLINGRULE),
                               UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_MODELLINGRULE_MANDATORY), true);
    }
    //USB
    {
        UA_ObjectTypeAttributes usb_attributes = UA_ObjectTypeAttributes_default;
        usb_attributes.displayName = UA_LOCALIZEDTEXT("ru-RU", "USB");
        UA_Server_addObjectTypeNode(server, UA_NODEID_NULL,
                                    UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
                                    UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
                                    UA_QUALIFIEDNAME(1, "USB"), usb_attributes,
                                    NULL, &DeviceType_USB_id);
        // vid
        {
            UA_VariableAttributes vid_attributes = UA_VariableAttributes_default;
            vid_attributes.displayName = UA_LOCALIZEDTEXT("ru-RU", "Идентификатор версии");
            vid_attributes.valueRank = UA_VALUERANK_SCALAR;
            vid_attributes.dataType = UA_TYPES[UA_TYPES_INT32].typeId;
            UA_Server_addVariableNode(server, UA_NODEID_NULL, DeviceType_USB_id,
                                      UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
                                      UA_QUALIFIEDNAME(1, "VID"),
                                      UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), vid_attributes, NULL,
                                      &DeviceType_USB_vid_id);
            /* Make the vid name mandatory */
            UA_Server_addReference(server, DeviceType_USB_vid_id,
                                   UA_NODEID_NUMERIC(0, UA_NS0ID_HASMODELLINGRULE),
                                   UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_MODELLINGRULE_MANDATORY), true);
        }
        // pid
        {
            UA_VariableAttributes pid_attributes = UA_VariableAttributes_default;
            pid_attributes.displayName = UA_LOCALIZEDTEXT("ru-RU", "Идентификатор производителя");
            pid_attributes.valueRank = UA_VALUERANK_SCALAR;
            pid_attributes.dataType = UA_TYPES[UA_TYPES_INT32].typeId;
            UA_Server_addVariableNode(server, UA_NODEID_NULL, DeviceType_USB_id,
                                      UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
                                      UA_QUALIFIEDNAME(1, "PID"),
                                      UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), pid_attributes, NULL,
                                      &DeviceType_USB_pid_id);
            /* Make the pid name mandatory */
            UA_Server_addReference(server, DeviceType_USB_pid_id,
                                   UA_NODEID_NUMERIC(0, UA_NS0ID_HASMODELLINGRULE),
                                   UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_MODELLINGRULE_MANDATORY), true);
        }
    }
    // manufacturer
    {
        UA_VariableAttributes manufacturer_attributes = UA_VariableAttributes_default;
        manufacturer_attributes.displayName = UA_LOCALIZEDTEXT("ru-RU", "Производитель");
        manufacturer_attributes.valueRank = UA_VALUERANK_SCALAR;
        manufacturer_attributes.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
        UA_Server_addVariableNode(server, UA_NODEID_NULL, DeviceType_id,
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
                                  UA_QUALIFIEDNAME(1, "Manufacturer"),
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), manufacturer_attributes, NULL,
                                  &DeviceType_manufacturer_id);
        /* Make the manufacturer name mandatory */
        UA_Server_addReference(server, DeviceType_manufacturer_id,
                               UA_NODEID_NUMERIC(0, UA_NS0ID_HASMODELLINGRULE),
                               UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_MODELLINGRULE_MANDATORY), true);
    }
    // product
    {
        UA_VariableAttributes product_attributes = UA_VariableAttributes_default;
        product_attributes.displayName = UA_LOCALIZEDTEXT("ru-RU", "Модель");
        product_attributes.valueRank = UA_VALUERANK_SCALAR;
        product_attributes.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
        UA_Server_addVariableNode(server, UA_NODEID_NULL, DeviceType_id,
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
                                  UA_QUALIFIEDNAME(1, "Product"),
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), product_attributes, NULL,
                                  &DeviceType_product_id);
        /* Make the product name mandatory */
        UA_Server_addReference(server, DeviceType_product_id,
                               UA_NODEID_NUMERIC(0, UA_NS0ID_HASMODELLINGRULE),
                               UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_MODELLINGRULE_MANDATORY), true);
    }
    // serial
    {
        UA_VariableAttributes serial_attributes = UA_VariableAttributes_default;
        serial_attributes.displayName = UA_LOCALIZEDTEXT("ru-RU", "Серийный номер");
        serial_attributes.valueRank = UA_VALUERANK_SCALAR;
        serial_attributes.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
        UA_Server_addVariableNode(server, UA_NODEID_NULL, DeviceType_id,
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
                                  UA_QUALIFIEDNAME(1, "Serial"),
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), serial_attributes, NULL,
                                  &DeviceType_serial_id);
        /* Make the serial name mandatory */
        UA_Server_addReference(server, DeviceType_serial_id,
                               UA_NODEID_NUMERIC(0, UA_NS0ID_HASMODELLINGRULE),
                               UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_MODELLINGRULE_MANDATORY), true);
    }
}

#endif

bool
trigger_SignalEvent(UA_Server *server, SignalEvent event) {
    UA_NodeId event_id;
    UA_StatusCode ret = UA_Server_createEvent(server, SignalEventType_id, &event_id);
    if (ret != UA_STATUSCODE_GOOD) {
        UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_SERVER,
                     "UA_Server_createEvent failed. StatusCode %s", UA_StatusCode_name(ret));
        return ret == UA_STATUSCODE_GOOD;
    }

    {
        UA_DateTime event_time = UA_DateTime_now();
        UA_Server_writeObjectProperty_scalar(server, event_id, UA_QUALIFIEDNAME(0, "Time"), &event_time,
                                             &UA_TYPES[UA_TYPES_DATETIME]);

        UA_UInt16 event_severity = 100;
        UA_Server_writeObjectProperty_scalar(server, event_id, UA_QUALIFIEDNAME(0, "Severity"),
                                             &event_severity, &UA_TYPES[UA_TYPES_UINT16]);

        UA_LocalizedText event_message = UA_LOCALIZEDTEXT("ru-RU", "Сигнальное событие");
        UA_Server_writeObjectProperty_scalar(server, event_id, UA_QUALIFIEDNAME(0, "Message"),
                                             &event_message, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]);

        UA_String event_source_name = UA_STRING("Device");
        UA_Server_writeObjectProperty_scalar(server, event_id, UA_QUALIFIEDNAME(0, "SourceName"),
                                             &event_source_name, &UA_TYPES[UA_TYPES_STRING]);

        UA_Server_writeObjectProperty_scalar(server, event_id, UA_QUALIFIEDNAME(0, "EventType"),
                                             &SignalEventType.typeId, &UA_TYPES[UA_TYPES_NODEID]);
    }

    ret = UA_Server_triggerEvent(server, event_id, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER), NULL, UA_TRUE);
    if (ret != UA_STATUSCODE_GOOD) {
        UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_SERVER,
                     "UA_Server_triggerEvent failed. StatusCode %s", UA_StatusCode_name(ret));
    }
    return ret == UA_STATUSCODE_GOOD;
}

bool register_builtin_types(UA_Server *server) {
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
    register_object_types(server);
    register_object_values(server);
    return true;
}

