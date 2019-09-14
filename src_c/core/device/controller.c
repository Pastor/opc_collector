#include <open62541/plugin/log.h>
#include <open62541/plugin/log_stdout.h>
#include <types/basic_types.h>
#include <hidapi.h>
#include "controller.h"
#include "multithreading.h"

static volatile bool monitoring_enabled = false;
static volatile hid_device *handle = NULL;
static volatile bool detected = false;
static void *busy = NULL;

static unsigned char buf[1024] = {0};
static size_t buf_read = 0;

void monitoring_reset_device_information(UA_Server *server);

void buf_clean();

bool monitoring_has_device() {
    return handle != NULL;
}

static void write_id_string(UA_Server *server, UA_NodeId id, UA_String text) {
    UA_Variant value;
    UA_Variant_setScalar(&value, &text, &UA_TYPES[UA_TYPES_STRING]);
    UA_Server_writeValue(server, id, value);
}

static void write_id_number(UA_Server *server, UA_NodeId id, UA_Int32 number) {
    UA_Variant value;
    UA_Variant_setScalar(&value, &number, &UA_TYPES[UA_TYPES_INT32]);
    UA_Server_writeValue(server, id, value);
}

static void
monitoring_callback(UA_Server *server, void *data) {
    if (handle != NULL) {
        int res;

        next_read:
        res = hid_read((hid_device *) handle, buf + buf_read, sizeof(buf));
        if (res < 0) {
            hid_close((hid_device *) handle);
            handle = NULL;
            monitoring_reset_device_information(server);
        } else if (res > 0) {
            buf_read = buf_read + res;
            if (buf_read >= sizeof(buf)) {
                UA_LOG_WARNING(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Device buffer overflow");
                buf_clean();
            }
            goto next_read;
        }
    }

    if (buf_read > 0) {
        SignalEvent event = {
                0,
                0,
                0,
                0,
                0,
                0,
                0, 0,
                0, 0
        };

        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Read: %.*s", (int) buf_read, buf);
        trigger_SignalEvent(server, event);
        buf_clean();
    }

    if (handle != NULL)
        return;

    handle = hid_open(0x10c4, 0x8468, NULL);
    if (handle == NULL) {
        if (detected) {
            monitoring_reset_device_information(server);
        }
    } else if (detected == false) {
        int res;
#define MAX_STR 255
        char mstr[MAX_STR];
        wchar_t wstr[MAX_STR];

        write_id_number(server, DeviceType_DetectedDevice_USB_pid_id, 0x8468);
        write_id_number(server, DeviceType_DetectedDevice_USB_vid_id, 0x10c4);
        wstr[0] = 0x0000;
        res = hid_get_manufacturer_string((hid_device *) handle, wstr, MAX_STR);
        if (res < 0)
            UA_LOG_WARNING(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Unable to read manufacturer string");
        wcstombs(mstr, wstr, sizeof(mstr));
        write_id_string(server, DeviceType_DetectedDevice_manufacturer_id, UA_STRING(mstr));
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Manufacturer: %ls", wstr);

        // Read the Product String
        wstr[0] = 0x0000;
        res = hid_get_product_string((hid_device *) handle, wstr, MAX_STR);
        if (res < 0)
            UA_LOG_WARNING(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Unable to read product string");
        wcstombs(mstr, wstr, sizeof(mstr));
        write_id_string(server, DeviceType_DetectedDevice_product_id, UA_STRING(mstr));
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Product     : %ls", wstr);

        // Read the Serial Number String
        wstr[0] = 0x0000;
        res = hid_get_serial_number_string((hid_device *) handle, wstr, MAX_STR);
        if (res < 0)
            UA_LOG_WARNING(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Unable to read serial number string");
        wcstombs(mstr, wstr, sizeof(mstr));
        write_id_string(server, DeviceType_DetectedDevice_serial_id, UA_STRING(mstr));
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Serial      : (%d) %ls", wstr[0], wstr);

        hid_set_nonblocking((hid_device *) handle, 1);
    }
    detected = handle != NULL;
}

void monitoring_reset_device_information(UA_Server *server) {
    write_id_number(server, DeviceType_DetectedDevice_USB_pid_id, 0);
    write_id_number(server, DeviceType_DetectedDevice_USB_vid_id, 0);
    write_id_string(server, DeviceType_DetectedDevice_manufacturer_id, UA_STRING("undefined"));
    write_id_string(server, DeviceType_DetectedDevice_product_id, UA_STRING("undefined"));
    write_id_string(server, DeviceType_DetectedDevice_serial_id, UA_STRING("undefined"));
    UA_LOG_WARNING(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Device lost");
}

bool controller_registration(UA_Server *server) {
    if (monitoring_enabled == false) {
        monitoring_enabled = hid_init() == 0 &&
                             UA_Server_addRepeatedCallback(server, monitoring_callback, NULL, 50, NULL)
                             == UA_STATUSCODE_GOOD;
    }
    return monitoring_enabled;
}

bool controller_send(UA_Server *server, const unsigned char *data, size_t data_length) {
    int res;

    if (handle == NULL)
        return false;
    res = hid_write((hid_device *) handle, data, data_length);
    if (res < 0 || res != data_length) {
        UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Unable to write(). Error %ls",
                     hid_error((hid_device *) handle));
    }
    return res == data_length;
}

bool controller_reset(UA_Server *server) {
    handle = NULL;
    detected = false;
    return true;
}

void buf_clean() {
    memset(buf, 0, sizeof(buf));
    buf_read = 0;
}

