#include <stdio.h>
#include <stdlib.h>

#define DEBUG

#define ARG_LIMIT 8

void cmd_execute(char *cmd) {
    char *args[ARG_LIMIT];
    char *body, *p;
    int i = 0, nargs = 0;
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
}

