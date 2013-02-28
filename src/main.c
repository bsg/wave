#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "serv.h"

#define BUFFER_SIZE 1024

int hnd_read(srv_t *ctx, int fd) {
	char buffer[BUFFER_SIZE];
	int nread;

	nread =	srv_read(fd, buffer, BUFFER_SIZE);
	if(!nread) {
		printf("Connection dropped - FD: %d\n", fd);
		return 1;
	}

	printf("[%i]: %s\n", fd, buffer);
    cmd_execute(buffer);

	return 0;
}

int hnd_accept(srv_t *ctx, int fd, char *host, int port) {
	printf("Incoming connection from %s:%d\n", host, port);
	return 0;
}

void main() {
	srv_t ctx;

	srv_init(&ctx);
	srv_hnd_accept(&ctx, hnd_accept);
	srv_hnd_read(&ctx, hnd_read);

	srv_run(&ctx, NULL, "9999");
}
