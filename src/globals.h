#ifndef __GLOBALS_H
#define __GLOBALS_H

typedef struct {
    unsigned int port;
    unsigned int tcp_buffer_size;
    unsigned int message_buffer_size;
    unsigned int max_channel_count;
    unsigned int max_subscribers_per_channel;
    /* TODO: max_subscriber_count */
} globals_t;

void globals_init();
globals_t *globals();

#define GLOBAL globals()

#endif
