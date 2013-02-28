#ifndef __CHANNEL_H
#define __CHANNEL_H

#define SUBSCRIBER_LIMIT  256
#define CHANNEL_LIMIT     256

#define CH_SUCCESS        0x0
#define CH_FULL           0x1
#define CH_NOT_SUBSCRIBED 0x2

typedef struct {
    int subscribers[SUBSCRIBER_LIMIT]; /* List of subscriber fd's */
} channel_t;

#endif
