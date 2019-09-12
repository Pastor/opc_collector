#include <open62541/plugin/log.h>
#include <open62541/plugin/log_stdout.h>
#include <types/basic_types.h>
#include <hidapi.h>
#include "monitoring.h"

static volatile bool monitoring_enabled = false;
static volatile hid_device *handle = NULL;

bool monitoring_has_device() {
    return handle != NULL;
}

static void
monitoring_callback(UA_Server *server, void *data) {
    static bool detected = false;
    handle = hid_open(0x10c4, 0x8468, NULL);
    if (handle == NULL) {
        if (detected) {
            UA_LOG_WARNING(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Device lost");
        }
    } else if (detected == false) {
        int res;
#define MAX_STR 255
        char mstr[MAX_STR];
        wchar_t wstr[MAX_STR];

        // Read the Manufacturer String
        wstr[0] = 0x0000;
        res = hid_get_manufacturer_string((hid_device *) handle, wstr, MAX_STR);
        if (res < 0)
            UA_LOG_WARNING(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Unable to read manufacturer string");
        wcstombs(mstr, wstr, sizeof(mstr));
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Manufacturer: %ls", wstr);

        // Read the Product String
        wstr[0] = 0x0000;
        res = hid_get_product_string((hid_device *) handle, wstr, MAX_STR);
        if (res < 0)
            UA_LOG_WARNING(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Unable to read product string");
        wcstombs(mstr, wstr, sizeof(mstr));
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Product     : %ls", wstr);

        // Read the Serial Number String
        wstr[0] = 0x0000;
        res = hid_get_serial_number_string((hid_device *) handle, wstr, MAX_STR);
        if (res < 0)
            UA_LOG_WARNING(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Unable to read serial number string");
        wcstombs(mstr, wstr, sizeof(mstr));
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Serial      : (%d) %ls", wstr[0], wstr);
    }
    detected = handle != NULL;
}

bool monitoring_registration(UA_Server *server) {
    if (monitoring_enabled == false) {
        monitoring_enabled = hid_init() == 0 &&
                             UA_Server_addRepeatedCallback(server, monitoring_callback, NULL, 500, NULL)
                             == UA_STATUSCODE_GOOD;
    }
    return monitoring_enabled;
}

