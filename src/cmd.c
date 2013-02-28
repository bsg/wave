#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "channel.h"

#define ARG_LIMIT 8

void cmd_pub(int, char *[], char *);
void cmd_sub(int, char *[], char *); 

struct command {
    char *name;
    void (*func)(int, char *[], char *);
};

struct command commands[] = {
                            {"pub", cmd_pub},
                            {"sub", cmd_sub},
                            {NULL,  NULL}
                          };

void cmd_execute(int fd, char *cmd) {
    char *args[ARG_LIMIT];
    char *body, *p;
    int i = 0, nargs = 0;

    struct command *p_cmd = commands;
    
    p = cmd;

    while(*cmd != 0) {
        if(*cmd == ' ') {
            *cmd = 0;
            args[i++] = p;
            p = cmd + 1;
            nargs++;
        }
        else if(*cmd == '\n') {
            if(*(cmd + 1) == 0) {
                *cmd = 0;
                body = p;
                p = cmd + 1;
            }
            else {
                *cmd = 0;
                args[i++] = p;
                p = cmd + 1;
                nargs++;
                body = NULL;
            }
        }
        cmd++;
    }

#ifdef DEBUG
    for(i = 0; i < nargs; i++) {
        printf("arg[%d]: %s\n", i, args[i]);
    }
    if(body != NULL)
        printf("body: %s\n", body); 
#endif
    
    while((*p_cmd).name != NULL) {
        if(strcmp((*p_cmd).name, args[0]) == 0) {  
           (*(*p_cmd).func)(fd, args, body);
        }
        p_cmd++;
    }
}

void cmd_pub(int fd, char *args[], char *body) {
    if(ch_publish(atoi(args[1]), fd, body, strlen(body)) == CH_SUCCESS) {
        printf("[%d] Published to CH%s\n", fd, args[1]);
    }
}

void cmd_sub(int fd, char *args[], char *body) {
    if(ch_subscribe(atoi(args[1]), fd) == CH_SUCCESS) {
        printf("[%d] Subscribed to CH%s\n", fd, args[1]);
    }
}
