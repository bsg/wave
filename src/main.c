#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "globals.h"
#include "serv.h"
#include "channel.h"
#include "logger.h"

size_t buffer_size;
char *buffer;

int hnd_read(srv_t *ctx, int fd) {
	int nread;

    bzero(buffer, buffer_size); /* TODO: Profile this */

	nread =	srv_readall(fd, buffer, buffer_size);
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

void set_default_globals() {
    GLOBAL->port                        = 9999;
    GLOBAL->tcp_buffer_size             = 1024;
    GLOBAL->message_buffer_size         = 1024;
    GLOBAL->max_channel_count           = 256;
    GLOBAL->max_subscribers_per_channel = 32;
}

void print_globals() {
    printf("TCP read buffer size            : %d\n",
        GLOBAL->tcp_buffer_size);
    printf("Message buffer size             : %d\n",
        GLOBAL->message_buffer_size);
    printf("Maximum channel count           : %d\n",
        GLOBAL->max_channel_count);
    printf("Maximum subscribers per channel : %d\n",
        GLOBAL->max_subscribers_per_channel);
}

char *init_message_buffers(size_t size) {
    buffer_size = size;
    buffer = (char *) calloc(size, sizeof(char));
    return buffer;
}

int main() {
    char port[8];

    /* BEWARE! Deleting the following line will open the gates of hell.
     * We might get SIGPIPE when trying to write to a dropped socket.
     * This is to prevent the signal from fuxoring the whole process.
     */
    signal(SIGPIPE, sigpipe_handler);

    /* Initialize the global value store */
    globals_init();
    set_default_globals();
    
    /* libserv context */
	srv_t ctx;

    /* Initialize networking subsystem */
	srv_init(&ctx);
	srv_hnd_accept(&ctx, hnd_accept);
	srv_hnd_read(&ctx, hnd_read);

    printf("Initializing wave with the following parameters...\n");
    print_globals();

    if(init_message_buffers(GLOBAL->message_buffer_size) == NULL) {
        printf("Failed to allocate %d bytes for message buffer!\n",
            GLOBAL->message_buffer_size);
        return 0;
    }

    ch_init_all_channels();

    sprintf(port, "%d", GLOBAL->port);
    printf("Listening on port %s\n", port);
	srv_run(&ctx, NULL, port);
}

