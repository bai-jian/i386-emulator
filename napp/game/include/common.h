#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <stdint.h>

#include "trap.h"
#include "debug.h"

typedef uint8_t bool;

#define true         1
#define false        0

#define BIN_SCALE

#ifdef BIN_SCALE
typedef int32_t FLOAT;
#else
typedef float FLOAT;
#endif

#endif
