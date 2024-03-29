#ifndef OPC_COLLECTOR_CORE_MULTITHREADING_H
#define OPC_COLLECTOR_CORE_MULTITHREADING_H

#include <stdbool.h>

#if defined(__cplusplus)
extern "C" {
#endif


void signal_init(volatile void **busy);

void signal_destroy(volatile void **busy);

void signal_unlock(volatile void *busy);

void signal_do(volatile void *busy);

bool signal_try_lock(volatile void *busy);

bool signal_wait(volatile void *busy, int wait_milliseconds);


void busy_init(volatile void **busy);

void busy_destroy(volatile void **busy);

bool busy_try_lock(volatile void *busy);

void busy_unlock(volatile void *busy);

void u_delay(long milliseconds);

void *thread_create(void *(*cb)(void *), void *args, bool detached);

void thread_destroy(void *handle);

#if defined(__cplusplus)
}
#endif


#endif //OPC_COLLECTOR_CORE_MULTITHREADING_H
