#include <string.h>
#include "globals.h"

globals_t _globals;

void globals_init() {
    bzero(&_globals, sizeof(globals_t));
}

globals_t *globals() {
    return &_globals;
}
