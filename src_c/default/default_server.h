#ifndef OPC_COLLECTOR_DEFAULT_SERVER_H
#define OPC_COLLECTOR_DEFAULT_SERVER_H

#if defined(__cplusplus)
extern "C" {
#endif
void default_server_start();
void default_server_stop();
#if defined(__cplusplus)
}
#endif
#define UA_CLIENT_CONNECTION_STRING "opc.tcp://localhost:6789"

#define UA_CLIENT_DEFAULT_DEVICE_SERIAL       "DeviceForTesting"
#define UA_CLIENT_DEFAULT_DEVICE_PRODUCT      "I-VOTE ver 4.2 HID USB device"
#define UA_CLIENT_DEFAULT_DEVICE_QUALIFY_NAME "Test.Device"


#endif //OPC_COLLECTOR_DEFAULT_SERVER_H
