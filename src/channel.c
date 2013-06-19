#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "globals.h"
#include "channel.h"
#include "logger.h"

/* XXX: O(n) insertion/deletion to channel subscriber list.
        O(1) access to subscriber list, which is what actually matters
        since list reads are on the critical path.

        The array list implementation must not be replaced with i.e. a linked
        list for cache coherency
*/

unsigned int channel_limit;
unsigned int subscriber_limit;
channel_t *channels;

void ch_init_all_channels() {
    channel_limit = GLOBAL->max_channel_count;
    channels = calloc(channel_limit, sizeof(channel_t));
    if(channels == NULL) {
        printf("Failed to allocate space for %d channels\n",
            channel_limit);
        /* TODO: Fail gracefully */
    }

    int i;
    for(i = 0; i < channel_limit; i++) {
        ch_init(i);
    }
}

void ch_init(int ch) {
    int i;
    
    subscriber_limit = GLOBAL->max_subscribers_per_channel;    

    channels[ch].subscribers = calloc(subscriber_limit, sizeof(int));

    if(channels[ch].subscribers == NULL) {
        printf("Failed to allocate space for %d subscribers\n",
            subscriber_limit);
        /* TODO: Fail gracefully */
    }

    for(i = 0; i < subscriber_limit; i++)
        channels[ch].subscribers[i] = 0;
}

inline int ch_subscribe(int ch, int fd) {
    /* TODO: Prevent multiple subscriptions from the same connection to the same channel */
    int i;
    for(i = 0; i < subscriber_limit; i++) {
        if(channels[ch].subscribers[i] == 0) {
            channels[ch].subscribers[i] = fd;
            return CH_SUCCESS;
        }
    }
    return CH_FULL;
}

inline void ch_unsubscribe_from_all(int fd) {
    int i;
    for(i = 0; i < channel_limit; i++) {
        ch_unsubscribe(i, fd);
    }
}

inline int ch_unsubscribe(int ch, int fd) {
    int i;
    for(i = 0; i < subscriber_limit; i++) {
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

    for(i = 0; i < subscriber_limit; i++) {
        if(channels[ch].subscribers[i] != 0) {
            sprintf(header, "msg %d\n", ch);
            srv_writeall(channels[ch].subscribers[i], header, strlen(header));
            srv_writeall(channels[ch].subscribers[i], data, size);
            srv_writeall(channels[ch].subscribers[i], "\n", 1);
        }
    }
    return CH_SUCCESS;
}

