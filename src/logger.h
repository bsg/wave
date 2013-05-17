#ifndef __LOGGER_H
#define __LOGGER_H

#include <stdio.h>

#define LOG_INFO(format, args...) printf("[INFO] " format, ## args);
#define LOG_ERROR(format, args...) printf("[ERROR] " format, ## args);

#endif
