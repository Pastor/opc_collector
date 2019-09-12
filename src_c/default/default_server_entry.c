#include <signal.h>
#include <stdlib.h>

#include "default_server.h"
#include "multithreading.h"

static void stop_handler(int sign) {
    default_server_stop();
}


int main(int argc, char **argv) {
    signal(SIGINT, stop_handler);
    signal(SIGTERM, stop_handler);
    default_server_start();
    u_delay(-1);
    default_server_stop();
    return EXIT_SUCCESS;
}

