#include <tests_base.hxx>
#include <multithreading.h>

extern "C" {
bool monitoring_has_device();
}

TEST(DeviceMonitoring, MonitoringDetectDevice) {
    for (int i = 0; i < 10; ++i) {
        if (monitoring_has_device())
            return;
        u_delay(500);
    }
    ASSERT_TRUE(false);
}

