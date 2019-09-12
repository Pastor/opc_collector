#include "tests_base.hxx"
#include "default_server.h"

int main(int argc, char **argv) {
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
    int ret = RUN_ALL_TESTS();
#if defined(MEMORY_LEAK_DETECT)
    _CrtMemCheckpoint(&_checkpoint_end);
    if (_CrtMemDifference(&_checkpoint_diff, &_checkpoint_start, &_checkpoint_end))
        _CrtMemDumpStatistics( &_checkpoint_diff );
#endif
    default_server_stop();
    return ret;
}
