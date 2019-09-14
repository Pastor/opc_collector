#include "types/basic_types.h"
#include "tests_base.hxx"
#include "default_server.h"

extern "C" {
extern UA_Server *_default_server;
extern volatile UA_Boolean _default_server_running;

void register_DeviceInformationType_Variable(UA_Server *server, DeviceInformation *di);
}

int main(int argc, char **argv) {
    DeviceInformation gdi = {
            UA_DEVICE_TYPE_HID, 0, 0,
            UA_STRING((char *)UA_CLIENT_DEFAULT_DEVICE_SERIAL),
            UA_STRING((char *)UA_CLIENT_DEFAULT_DEVICE_PRODUCT), 0
    };

    ::testing::InitGoogleTest(&argc, argv);
#if defined(MEMORY_LEAK_DETECT)
    _CrtMemState _checkpoint_start;
    _CrtMemState _checkpoint_end;
    _CrtMemState _checkpoint_diff;

    fprintf(stderr, "Memory leak detection enabled\n");
     _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
     _CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE );
     _CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDERR );
    _CrtMemCheckpoint(&_checkpoint_start);

#endif
    default_server_start();
    while (_default_server_running == false);
    register_DeviceInformationType_Variable(_default_server, &gdi);
    int ret = RUN_ALL_TESTS();
#if defined(MEMORY_LEAK_DETECT)
    _CrtMemCheckpoint(&_checkpoint_end);
    if (_CrtMemDifference(&_checkpoint_diff, &_checkpoint_start, &_checkpoint_end))
        _CrtMemDumpStatistics( &_checkpoint_diff );
#endif
    default_server_stop();
    return ret;
}
