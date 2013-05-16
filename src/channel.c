#include <stdio.h>
#include <string.h>

#include "channel.h"
#include "logger.h"

/* XXX: O(n) insertion/deletion to channel subscriber list.
        O(1) access to subscriber list, which is what actually matters
        since list reads are on the critical path.

        The array list implementation must not be replaced with i.e. a linked
        list for cache coherency
*/
channel_t channels[CHANNEL_LIMIT];

void ch_init_all_channels() {
    int i;
    for(i = 0; i < CHANNEL_LIMIT; i++) {
        ch_init(i);
    }
}

void ch_init(int ch) {
    int i;
    for(i = 0; i < SUBSCRIBER_LIMIT; i++)
        channels[ch].subscribers[i] = 0;
}

inline int ch_subscribe(int ch, int fd) {
    /* TODO: Prevent multiple subscriptions from the same connection to the same channel */
    int i;
    for(i = 0; i < SUBSCRIBER_LIMIT; i++) {
        if(channels[ch].subscribers[i] == 0) {
            channels[ch].subscribers[i] = fd;
            return CH_SUCCESS;
        }
    }
    return CH_FULL;
}

inline void ch_unsubscribe_from_all(int fd) {
    int i;
    for(i = 0; i < CHANNEL_LIMIT; i++) {
        ch_unsubscribe(i, fd);
    }
}

inline int ch_unsubscribe(int ch, int fd) {
    int i;
    for(i = 0; i < SUBSCRIBER_LIMIT; i++) {
        if(channels[ch].subscribers[i] == fd) {
            channels[ch].subscribers[i] = 0;
            return CH_SUCCESS;
        }
    }
    return CH_NOT_SUBSCRIBED;
}

inline int ch_publish(int ch, int fd, char *data, int size) {
    /* TODO: Iterate only until the last subscriber */
    int i;
    char header[32];

    for(i = 0; i < SUBSCRIBER_LIMIT; i++) {
        if(channels[ch].subscribers[i] != 0) {
            sprintf(header, "msg %d\n", ch);
            srv_writeall(channels[ch].subscribers[i], header, strlen(header));
            srv_writeall(channels[ch].subscribers[i], data, size);
            srv_writeall(channels[ch].subscribers[i], "\n", 1);
        }
    }
    return CH_SUCCESS;
}

