#include "channel.h"

/* XXX: O(n) insertion/deletion to channel subscriber list.
        O(1) access to subscriber list, which is what actually matters
        since list reads are on the critical path.

        The array list implementation must not be replaced with i.e. a linked
        list for cache coherency
*/
channel_t channels[CHANNEL_LIMIT];

void ch_init(int ch) {
    int i;
    for(i = 0; i < SUBSCRIBER_LIMIT; i++)
        channels[ch].subscribers[i] = 0;
}

inline int ch_subscribe(int ch, int fd) {
    int i;
    for(i = 0; i < SUBSCRIBER_LIMIT; i++) {
        if(channels[ch].subscribers[i] == 0) {
            channels[ch].subscribers[i] = fd;
            return CH_SUCCESS;
        }
        return CH_FULL;
    }
}

inline int ch_unsubscribe(int ch, int fd) {
    int i;
    for(i = 0; i < SUBSCRIBER_LIMIT; i++) {
        if(channels[ch].subscribers[i] == fd) {
            channels[ch].subscribers[i] = 0;
            return CH_SUCCESS;
        }
        return CH_NOT_SUBSCRIBED;
    }
}

inline int ch_publish(int ch, int (*func_write)(int, char *, int), char *data, int size) {
    /* TODO: Iterate only until the last subscriber */
    int i;
    int n_published = 0;

    for(i = 0; i < SUBSCRIBER_LIMIT; i++) {
        if(channels[ch].subscribers[i] != 0) {
            func_write(channels[ch].subscribers[i], data, size);
            n_published++;
        }
    }

    return n_published;
}

