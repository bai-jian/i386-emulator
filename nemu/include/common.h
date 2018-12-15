#ifndef __COMMON_H__
#define __COMMON_H__

#include "debug.h"
#include "macro.h"

#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

typedef uint8_t bool;

typedef uint32_t hwaddr_t;
typedef uint32_t lnaddr_t;
typedef uint32_t swaddr_t;

typedef uint16_t ioaddr_t;

#define false 0
#define true 1

#endif
