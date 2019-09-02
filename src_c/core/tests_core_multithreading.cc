#include <tests_base.hxx>
#include "multithreading.h"

class Signal_Object : public testing::Test {
public:
    Signal_Object() : _signal(nullptr) {}

protected:
    void SetUp() override {
        signal_init(&_signal);
        ASSERT_NE(_signal, nullptr);
    }

    void TearDown() override {
        signal_destroy(&_signal);
        ASSERT_EQ(_signal, nullptr);
    }

    volatile void *_signal;
};

class Busy_Object : public testing::Test {
public:
    Busy_Object() : _busy(nullptr) {}

protected:
    void SetUp() override {
        busy_init(&_busy);
        ASSERT_NE(_busy, nullptr);
    }

    void TearDown() override {
        busy_destroy(&_busy);
        ASSERT_EQ(_busy, nullptr);
    }

    volatile void *_busy;
};

TEST(Signal_NullPtr, DISABLED_Empty) {
    //
}

TEST(Signal_NullPtr, Init) {
    signal_init(nullptr);
}

TEST(Signal_NullPtr, Unlock) {
    volatile void *null = nullptr;

    signal_unlock(nullptr);
    signal_unlock(&null);
}

TEST(Signal_NullPtr, Do) {
    volatile void *null = nullptr;

    signal_do(nullptr);
    signal_do(&null);
}

TEST(Signal_NullPtr, Try_Lock) {
    ASSERT_FALSE(signal_try_lock(nullptr));
}

TEST(Signal_NullPtr, Wait) {
    volatile void *signal = nullptr;

    ASSERT_FALSE(signal_wait(nullptr, 1000));
    ASSERT_FALSE(signal_wait(signal, 1000));
}

TEST(Signal_NullPtr, Destroy) {
    volatile void *null = nullptr;
    signal_destroy(nullptr);
    signal_destroy(&null);
}

TEST_F(Signal_Object, Try_Lock) {
    ASSERT_TRUE(signal_try_lock(_signal));
    signal_unlock(_signal);
}

TEST_F(Signal_Object, Wait) {
    ASSERT_TRUE(signal_try_lock(_signal));
//    ASSERT_TRUE(signal_wait(_signal, 100));
    signal_unlock(_signal);
}

TEST(Busy_NullPtr, Init) {
    busy_init(nullptr);
}

TEST(Busy_NullPtr, Unlock) {
    volatile void *null = nullptr;
    busy_unlock(nullptr);
    busy_unlock(&null);
}

TEST(Busy_NullPtr, Destroy) {
    volatile void *null = nullptr;
    busy_destroy(nullptr);
    busy_destroy(&null);
}

TEST(Busy_NullPtr, Try_Lock) {
    ASSERT_FALSE(busy_try_lock(nullptr));
}

TEST_F(Busy_Object, Try_Lock) {
    ASSERT_TRUE(busy_try_lock(_busy));
    busy_unlock(_busy);
}

TEST(Multithreading, Delay) {
    u_delay(100);
}


