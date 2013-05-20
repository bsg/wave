#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "channel.h"
#include "logger.h"

#define ARG_LIMIT 8

void cmd_pub(int, int, char *[], char *);
void cmd_sub(int, int, char *[], char *);

#define DEBUG 

struct command {
    char *name;
    void (*func)(int, int, char *[], char *);
};

struct command commands[] = {
                                {"pub", cmd_pub},
                                {"sub", cmd_sub},
                                {NULL,  NULL}
                            };

void cmd_execute(int fd, char *cmd, unsigned int size) {
    char *args[ARG_LIMIT];
    char *body = NULL, *p;
    int i = 0, nargs = 0, is_parsing_header = 1;

    struct command *p_cmd = commands;
    
    p = cmd;

    /* HERE BE DRAGONS! */
    while(*cmd != 0) {
        if(is_parsing_header && (*cmd == ' ' || *cmd == '\n')) {
            if(*cmd == '\n') is_parsing_header = 0;
            *cmd = 0;
            args[i++] = p;
            p = cmd + 1;
            nargs++;  
        }
        /* Everything after the first newline is 'body' */
        else if(!is_parsing_header) {
            body = cmd;
            while(*cmd !=0 && *cmd !='\n') cmd++;
            if(*cmd != '\n') {
                LOG_ERROR("Invalid format.\n");
                return;
            }
            *(cmd) = 0;
            break;
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
           (*(*p_cmd).func)(fd, nargs, args, body);
        }
        p_cmd++;
    }
}

void cmd_pub(int fd, int argc, char *argv[], char *body) {
    if(body == NULL) {
        LOG_ERROR("cmd_pub failed. body=%p\n", body);
        return;
    }

    if(ch_publish(atoi(argv[1]), fd, body, strlen(body)) == CH_SUCCESS) {
        LOG_INFO("[%d] Published to CH%s\n", fd, argv[1]);
    }
    /* TODO: Handle publish failure */
}

void cmd_sub(int fd, int argc, char *argv[], char *body) {
    int rv;    
    rv = ch_subscribe(atoi(argv[1]), fd);

    switch(rv) {
        case CH_SUCCESS:
            LOG_INFO("[%d] Subscribed to CH%s\n", fd, argv[1]);
            break;
        case CH_FULL:
            LOG_ERROR("[%d] Cannot subscribe. Subscriber limit reached.\n", fd);
            break;
        default:
            LOG_ERROR("WE SHOULD NEVER GET HERE. FIX THE FUCKING CODE\n");
    }
}
