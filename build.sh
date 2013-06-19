gcc -g -o wave ./src/main.c ./src/channel.c ./src/cmd.c ./src/globals.c ./src/config.c -lserv -lm -ldl -llua5.2 -I/usr/include/lua5.2 -L/usr/lib/x86_64-linux-gnu
