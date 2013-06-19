#include <stdio.h>
#include <string.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

lua_State *L;

void config_init() {
    /* TODO: Error handling */
    L = luaL_newstate();
    luaL_openlibs(L);
}

void config_deinit() {
    lua_close(L);
}

int config_load_file(char *filename) {
    int rv = 0;   
    rv |= luaL_loadfile(L, filename);
    rv |= lua_pcall(L, 0, 0, 0);

    return rv; 
}

/* Doesn't fuxor the value of 'p' unless the key is found */
int config_read_int(char *key, int *p) {
    lua_getglobal(L, key);
    if(!lua_isnumber(L, -1)) {
        return 0;
    }
    else {
        *p = (int) lua_tonumber(L, -1);
        return 1;
    }
}

