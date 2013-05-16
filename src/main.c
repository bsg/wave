#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "serv.h"
#include "channel.h"

#include "logger.h"

#define BUFFER_SIZE 1024

int hnd_read(srv_t *ctx, int fd) {
	char buffer[BUFFER_SIZE];
	int nread;

    bzero(buffer, BUFFER_SIZE); /* TODO: Profile this */

	nread =	srv_readall(fd, buffer, BUFFER_SIZE);
	if(!nread) {
		printf("Connection dropped - FD: %d\n", fd);
        ch_unsubscribe_from_all(fd);
		return 1;
	}

    cmd_execute(fd, buffer, nread);

	return 0;
}

int hnd_accept(srv_t *ctx, int fd, char *host, int port) {
	printf("Incoming connection from %s:%d\n", host, port);
	return 0;
}

void sigpipe_handler(int signum) {
    LOG_INFO("Caught SIGPIPE\n");
}

void main() {
    /* BEWARE! Deleting the following line will open the gates of hell.
     * We might get SIGPIPE when trying to write to a dropped socket.
     * This is to prevent the signal from fuxoring the whole process.
     */
    signal(SIGPIPE, sigpipe_handler);

	srv_t ctx;

	srv_init(&ctx);
	srv_hnd_accept(&ctx, hnd_accept);
	srv_hnd_read(&ctx, hnd_read);

    ch_init_all_channels();

	srv_run(&ctx, NULL, "9999");
}
